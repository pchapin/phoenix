/****************************************************************************
FILE          : loader.c
LAST REVISION : 2008-04-16
SUBJECT       : boot loader
PROGRAMMER    : Curtis Aube, Nick Guertin, Wei Yao Lin

(C) Copyright 2008  The Phoenix Team.

Lists exe file names in a menu. Loads selected file.

Please send comments or bug reports to

     cjaube@yahoo.com
****************************************************************************/

#include <i86.h>
//#include <stdio.h>
#include <conio.h>

/*! \file loader.c Loads an exe file and launches it.
 *
 * This program will search through a FAT file system on a disk and display all the exe files in
 * a menu. If there is only one exe, it will load that file. If there is more than one exe it
 * will wait for the user to input a number for the file that they want. The exe file will then
 * be loaded in to memory and executed.
 */

static char FAT12_buffer[4608]; //Max size for FAT12
static int print_row = 0;
static unsigned int selected_file_size;

//Boot sector info structure
struct boot_sector_info {
    short bytes_per_sector;
    char sectors_per_cluster;
    short number_of_FATs;
    short number_of_root_directory_entries;
    short total_sectors_in_logical_volume;
    short number_of_sectors_per_FAT;
    short sectors_per_track;
    short number_of_heads;
    // Calculated information...
    int bytes_per_entry;
    int FAT;
    int root_directory;
    int number_of_root_sectors;
    int files_area;
};

struct exe_header_info {
    short part_pag;  // Length of file MOD 512.
    short page_cnt;  // Size of file in 512-byte pages.
    short relo_cnt;  // Number of relocation-table items.
    short hdr_size;  // Size of header in paragraphs.
    short min_mem;   // Minimum number of paragraphs needed above program.
    short max_mem;   // Maximum number of paragraphs desired above program.
    short relo_SS;   // Segment offset of stack segment.
    short exe_SP;    // Contents of SP register at entry.
    short chk_sum;   // Word checksum.
    short exe_IP;    // Contents of IP register at entry.
    short relo_CS;   // Segment offset of code segment.
    short table_off; // Offset of first relocation item in file.
    char Overlay;    // Overlay number.
};

// Cylinder-head-sector structure.
struct cylinder_head_sector {
    int cylinder;
    int head;
    int sector;
};

//! Prints a character string to the screen
/*!
 * The given string must be null terminated. The null byte is not considered part of the
 * string's length.
 *
 * \param The string to measure.
 */
void print( const char *message )
{
    char far *video_buffer = MK_FP( 0xB800, 0x0000 );
    int offset, count = 0;
  
    offset = 2 * ( 80 * print_row );
    while( *message ) {
        video_buffer[offset] = *message++;
        video_buffer[offset+1] = 0x07;
        offset += 2;
        count++;
    }
    set_cursor_position( print_row, count );
    print_row++;
}

//! Sets the cursor position
/*!
 * \param The row position.
 * \param The column position.
 */
void set_cursor_position( int row, int col )
{
    union REGS regs;

    regs.h.ah = 0x02; // Set Cursor position.
    regs.h.bh = 0x00; // Page.
    regs.h.dh = row;  // Row.
    regs.h.dl = col;  // Column.
    int86(0x10, &regs, &regs); // Video int.
}

//! Clears the screen
void clear_screen( )
{
    union REGS regs;
    char mode;

    regs.h.ah = 0x0F; // Get mode.
    mode = int86( 0x10, &regs, &regs ); // Video int.
    regs.h.ah = 0x00; // Set Mode.
    regs.h.al = mode;
    int86( 0x10, &regs, &regs ); // Video int.
}

//! Get boot sector information
/*!
 * Fills in a boot_sector_info structure with all the information gathered from the boot sector.
 *  
 * \param pointer to a boot_sector_info structure that will be filled.
 * \return Zero if unsuccessful.
 */
int get_boot_sector_info(struct boot_sector_info *bsi)
{
    union  REGS  r;
    struct SREGS s;
    char buffer[512];
    int tries = 0;
  
    do {
        r.h.ah = 0x02; // Read sector function.
        r.h.al = 0x01; // Number of sectors.
        r.h.ch = 0x00; // cylinder.
        r.h.cl = 0x01; // sector (0x01: logical sector 0).
        r.h.dh = 0x00; // head.
        r.h.dl = 0x00; // drive (0x00 - 0x7F in floppy).
        s.es = FP_SEG( (char far *)buffer ); // segment.
        r.x.bx = (unsigned short)buffer;     // offset of buffer.
        int86x( 0x13, &r, &r, &s );
        tries++;
    } while( r.h.ah != 0x00 && tries < 3 );
    
    if( r.h.ah != 0x00 ) {
        // fprintf( stderr, "Floppy read was unsuccessful after %d tries: %d\n", tries, r.h.ah );
        print( "Floppy read was unsuccessful" );
        return 0;
    }  
    else {
        bsi->bytes_per_sector = (buffer[0x0C] << 8) + buffer[0x0B];
        bsi->sectors_per_cluster = buffer[0x0D];
        bsi->number_of_FATs = buffer[0x10];
        bsi->number_of_root_directory_entries = (buffer[0x12] << 8) + buffer[0x11];
        bsi->total_sectors_in_logical_volume = (buffer[0x14] << 8) + buffer[0x13];
        bsi->number_of_sectors_per_FAT = (buffer[0x17] << 8) + buffer[0x16];
        bsi->sectors_per_track = (buffer[0x19] << 8) + buffer[0x18];
        bsi->number_of_heads = (buffer[0x1B] << 8) + buffer[0x1A];
        // Calculated information...
        bsi->bytes_per_entry = 32; // Assumed.
        bsi->FAT = 1;              // Assumed.
        bsi->root_directory = bsi->FAT + bsi->number_of_FATs * bsi->number_of_sectors_per_FAT;  
        bsi->number_of_root_sectors =
            bsi->number_of_root_directory_entries * bsi->bytes_per_entry / bsi->bytes_per_sector;
        if ((bsi->number_of_root_directory_entries * bsi->bytes_per_entry) % bsi->bytes_per_sector != 0) {
            bsi->number_of_root_sectors += 1;
        }
        bsi->files_area = bsi->root_directory + bsi->number_of_root_sectors;
        /*
          printf("Bytes per sector: %d\n", bsi->bytes_per_sector);
          printf("Sectors per cluster: %d\n", bsi->sectors_per_cluster);
          printf("Number of FATs: %d\n", bsi->number_of_FATs);
          printf("Number of root-directory entries: %d\n", bsi->number_of_root_directory_entries);
          printf("Total sectors in logical volume: %d\n", bsi->total_sectors_in_logical_volume);
          printf("Number of sectors per FAT: %d\n", bsi->number_of_sectors_per_FAT );
          printf("Sectors per track: %d\n", bsi->sectors_per_track);
          printf("Number of heads: %d\n", bsi->number_of_heads);
          printf("Bytes per entry: %d\n", bsi->bytes_per_entry);
          printf("FAT: %d\n", bsi->FAT);
          printf("Root directory: %d\n", bsi->root_directory);
          printf("Number of root sectors: %d\n", bsi->number_of_root_sectors);
          printf("Files area: %d\n", bsi->files_area);
        */
    }
     return 1;
}

//! Reads sectors and stores them in a segment:offset
/*!
 * Used to get information from the disk and store it in a buffer or somewhere in memory.
 *  
 * \param The segment of the location to write to.
 * \param The offset of the location to write to.
 * \param The cylinder-head-sector of the location to read from the disk.
 * \param The number of sectors to read.
 * \return Zero if unsuccessful.
 */
int read_write_sector(
    int segment,
    unsigned short offset,
    struct cylinder_head_sector chs,
    int number_of_sectors)
{
    union REGS r;
    struct SREGS s;
    int tries = 0;
  
    do {
        r.h.ah = 0x02;              // Read sector function.
        r.h.al = number_of_sectors; // Number of sectors.
        r.h.ch = chs.cylinder;      // cylinder.
        r.h.cl = chs.sector;        // sector.
        r.h.dh = chs.head;          // head.
        r.h.dl = 0x00;              // drive (0x00 - 0x7F in floppy).
        s.es = segment;             // FP_SEG((char far *)buffer); //segment.
        r.x.bx = offset;            // (unsigned short)buffer;     //offset of buffer.
        int86x( 0x13, &r, &r, &s );
        tries++;
    } while( r.h.ah != 0x00 && tries < 3 );
    
    if( r.h.ah != 0x00 ) {
        // fprintf( stderr, "Floppy read was unsuccessful after %d tries: %d\n", tries, r.h.ah );
        print( "Floppy read was unsuccessful" );
        return 0;
    }
    return 1;
}

//! Gets a disk location from a logical sector number
/*!
 * Fills in a disk_location structure with all the information needed to read from the disk at a
 * specific logical sector.
 *  
 *  \param A pointer to a cylinder_head_sector structure that is to be filled.
 *  \param The logical sector number.
 *  \param A filled boot_sector_info structure.
 */
void get_disk_location(
    struct cylinder_head_sector *chs,
    int logical_sector,
    struct boot_sector_info bsi )
{
    int sectors_per_cylider = bsi.number_of_heads * bsi.sectors_per_track;

    chs->cylinder = logical_sector / sectors_per_cylider;
    logical_sector = logical_sector % sectors_per_cylider;
  
    chs->head = logical_sector / bsi.sectors_per_track;
    logical_sector = logical_sector % bsi.sectors_per_track;
  
    chs->sector = logical_sector + 1;
    // printf("\ncylinder: %d\nhead: %d\nsector: %d\n", chs->cylinder, chs->head, chs->sector );
}

//! Displays a menu of exe files
/*!
 * Searches the root directory for exe files and displays them in a menu for the use to choose
 * from. If there is only one exe file, it will be returned automatically.
 *  
 * \param A filled in boot_sector_info structure.
 * \return the start cluster of the selected file or zero if not found.
 */
short search_root( struct boot_sector_info bsi )
{
    char buffer[512];
    char current_filename[12];
    int i, j, k, count = 0, key;
    short files[10];
    unsigned int files_size[10];
    struct cylinder_head_sector chs;

    print( "Choose a file to load:" );
    for( i = bsi.root_directory; i < bsi.files_area; i++ ) {
        get_disk_location( &chs, i, bsi );
        read_write_sector( FP_SEG( (char far *)buffer ), (unsigned short)buffer, chs, 1 );
        for( j = 0; j < 512; j+=bsi.bytes_per_entry ) {
            if( buffer[j] != 0x00 && buffer[j] != 0x2E && buffer[j] != 0xE5 ) {
                if( buffer[j+8] == 'E' && buffer[j+9] == 'X' && buffer[j+10] == 'E' && count < 10 ) {
                    current_filename[0] = count + '0';
                    current_filename[1] = ')';
                    current_filename[2] = ' ';
                    // Copy filename from buffer.
                    for( k = 0; k < 8; k++ ) {
                        current_filename[k+3] = buffer[j+k];
                    }
                    if( buffer[j] == 0x05 ) {
                        // First character of filename is actually E5H.
                        current_filename[3] = 0xE5;
                    }
                    current_filename[11] = '\0';  
                    if( count < 10 ) {
                        print( current_filename );
                        // Save start cluster.
                        files[count] = (buffer[j + 0x1B] << 8) + buffer[j + 0x1A];
                        files_size[count] = (buffer[j + 0x1D] << 8) + buffer[j + 0x1C];
                    }
                    count++;
                }
            }
        }
    }
    if( count == 1 ) {
        selected_file_size = files_size[0];
        return files[0];
    }
    print( ":" );
    while( 1 ) {
        key = inp( 0x60 );
        if( key == 11 ) { // 11 is the scan code for 0.
            key = 0;
        }
        else {
            key -= 1;
        }
        if( key >= 0 && key < count ) {
            selected_file_size = files_size[key];
            return files[key];
            break;
        }
    }
    return 0;
}


//! Reads the FAT and loads the file into memory
/*!     
 * \param The starting cluster of the file to be loaded.
 * \param The files target memory location.
 * \param A filled in boot_sector_info structure.
 * \return Zero if unsuccessful.
 */
int load_file( short starting_cluster, int file_start, struct boot_sector_info bsi )
{
    short cluster;
    int   i = 0;
    struct cylinder_head_sector chs;
  
    // Check number of clusters on disk to determine field size.
    if( bsi.total_sectors_in_logical_volume / bsi.sectors_per_cluster < 4087 ) {
        // Fill buffer.
        get_disk_location( &chs, 1, bsi );
        read_write_sector( FP_SEG( (char far *)FAT12_buffer ), (unsigned short)FAT12_buffer, chs, bsi.number_of_sectors_per_FAT );
        // Read through FAT.
        cluster = starting_cluster;
        do {
            // printf("%04X\n", cluster);
            // Load sector into memory.
            get_disk_location( &chs, (cluster - 2) * bsi.sectors_per_cluster + bsi.files_area, bsi );
            read_write_sector( file_start, (unsigned short)(i * 512), chs, 1 );
            i++;
            if( (cluster * 3) % 2 == 0 ) {
                cluster = ( (FAT12_buffer[cluster*3/2 + 1] << 8) +
                             FAT12_buffer[cluster*3/2]) & 0x0FFF;
            }
            else {
                cluster = ( ( (FAT12_buffer[cluster*3/2 + 1] << 8) + 
                              FAT12_buffer[cluster*3/2]) >> 4) & 0x0FFF;
            }
        } while( cluster < 0xFF8 );
        // printf("%04X\n", cluster);
    }
    else {
        // FAT16 not supported.
        return 1;
    }
    return 0;
}

//! Choose a starting location for the file
/*!
 * \return The file start target memory location.
 */
// Choose a starting location for the file.
// Returns the file start int.
int get_file_start( )
{
    int file_start = 0;
  
    // Get cs.
    _asm {
        mov ax, cs
        mov file_start, ax
    }
    file_start = ( file_start + 4096 + 4096 ) & 0xF000;
  
    return file_start;
}

//! Get the exe header information
/*!
 * Fills in a exe_header_info structure with all the information gathered from the boot sector.
 *  
 * \param The files memory location.
 * \param pointer to a exe_header_info structure that will be filled.
 * \return Zero if unsuccessful.
 */
int get_exe_header_info( int file_start, struct exe_header_info *ehi )
{
    char far *buffer = MK_FP( file_start, 0x0000 );

    if( buffer[0x00] == 'M' && buffer[0x01] == 'Z' ) {
        ehi->part_pag  = (buffer[0x03] << 8) + buffer[0x02];
        ehi->page_cnt  = (buffer[0x05] << 8) + buffer[0x04];
        ehi->relo_cnt  = (buffer[0x07] << 8) + buffer[0x06];
        ehi->hdr_size  = (buffer[0x09] << 8) + buffer[0x08];
        ehi->min_mem   = (buffer[0x0B] << 8) + buffer[0x0A];
        ehi->max_mem   = (buffer[0x0D] << 8) + buffer[0x0C];
        ehi->relo_SS   = (buffer[0x0F] << 8) + buffer[0x0E];
        ehi->exe_SP    = (buffer[0x11] << 8) + buffer[0x10];
        ehi->chk_sum   = (buffer[0x13] << 8) + buffer[0x12];
        ehi->exe_IP    = (buffer[0x15] << 8) + buffer[0x14];
        ehi->relo_CS   = (buffer[0x17] << 8) + buffer[0x16];
        ehi->table_off = (buffer[0x19] << 8) + buffer[0x18];
        ehi->Overlay = buffer[0x1A];
        /*
          printf("Length of file MOD 512: %u\n", ehi->part_pag);
          printf("Size of file in 512-byte pages: %u\n", ehi->page_cnt);
          printf("Number of relocation-table items: %u\n", ehi->relo_cnt);
          printf("Size of header in paragraphs: %u\n", ehi->hdr_size);
          printf("Minimum number of paragraphs needed above program: %u\n", ehi->min_mem);
          printf("Maximum number of paragraphs desired above program: %u\n", ehi->max_mem);
          printf("Segment displacement of stack module: %04X\n", ehi->relo_SS);
          printf("Contents of SP register at entry: %04X\n", ehi->exe_SP);
          printf("Word checksum: %u\n", ehi->chk_sum);
          printf("Contents of IP register at entry: %04X\n", ehi->exe_IP);
          printf("Segment displacement of code module: %04X\n", ehi->relo_CS);
          printf("Offset of first relocation item in file: %u\n", ehi->table_off);
          printf("Overlay number: %u\n", ehi->Overlay);
        */
    }
    else {
        print( "File is not a valid exe file." );
        return 1;
    }
    return 0;
}


//! Sets up the PSP(Program Segment Prefix)
/*!
 * The PSP might not be needed, but is partially filled just in case it needs to be accessed.
 *  
 * \param The location that the psp will be placed.
 */
void setup_PSP( int location )
{
    char far *buffer = MK_FP( location, 0x0000 );
  
    buffer[0x00] = 0xCD; // INT 20.
    buffer[0x01] = 0x20;
    buffer[0x02] = 0xFF; // Memory size in paragraphs.
    buffer[0x03] = 0xFF;
    buffer[0x04] = 0x00; // reserved for DOS, usually 0.
    buffer[0x2C] = 0x00; // Environment segment.
    buffer[0x2D] = 0x00;
    buffer[0x80] = 0x01; // Number of characters on command-line.
    buffer[0x81] = 0x0D; // Command-line (terminated by a 0Dh).
}

//! Set up file using the relocation table
/*!
 * All the pointers in the file are fixed to point to their actual memory locations.
 *  
 * \param The files memory location.
 * \param A filled exe_header_info structure.
 * \return Zero if unsuccessful.
 */
int setup_exe( int file_start, struct exe_header_info ehi )
{
    char far *buffer = MK_FP( file_start, 0x0000 );
    char far *write_buffer;
    int i, pos, header;
    short i_off, i_seg, cur_addr, new_addr;
    
    header = ehi.hdr_size * 16;
    for( i = 0; i < ehi.relo_cnt; i++ ) {
        pos = ehi.table_off + (i * 4);
        i_off = (buffer[pos + 1] << 8) + buffer[pos + 0];
        i_seg = (buffer[pos + 3] << 8) + buffer[pos + 2];
    
        write_buffer = MK_FP( file_start + i_seg, header + i_off );
        cur_addr = (write_buffer[1] << 8) + write_buffer[0];
        new_addr = cur_addr + file_start + ehi.hdr_size;
        write_buffer[0] = (char)new_addr;
        write_buffer[1] = (char)(new_addr >> 8);
        // printf("%04X:%04X %04X -> %04X\n", i_seg, i_off, cur_addr, new_addr);
    }
    return 0;
}

//! Check if the embeded exe's id is the same as the embeded boot sector's id
/*!
 * This is to be used for copy protection.
 *
 * \param The files memory location.
 * \param A filled exe_header_info structure.
 * \param A filled in boot_sector_info structure.
 * \return Zero if not a match.
 */
int check_copied( int file_start, struct exe_header_info ehi, struct boot_sector_info bsi )
{
    char far *file_buffer = MK_FP( file_start, 0x0000 );
    char boot_buffer[512];
    struct cylinder_head_sector chs;
    unsigned int end_of_file, end_of_boot;
    int i = 0;
    char *hex = "0123456789ABCDEF";
    char compare[20]; 
  
    get_disk_location( &chs, 0, bsi );
    read_write_sector( FP_SEG( (char far *)boot_buffer ), (unsigned short)boot_buffer, chs, 1 );
  
    end_of_file = selected_file_size;
    end_of_boot = 512;
  
    // DECRYPTION!!!!
    for( i = 0; i < end_of_file; i++ ) {
        file_buffer[i] ^= boot_buffer[end_of_boot-1];
    }
  
    for( i = 0; i < end_of_file; i++ ) {
        file_buffer[i] ^= boot_buffer[end_of_boot-2];
    }
  
    for( i = 0; i < end_of_file; i++ ) {
        file_buffer[i] ^= boot_buffer[end_of_boot-3];
    }
  
    for( i = 0; i < end_of_file; i++ ) {
        file_buffer[i] ^= boot_buffer[end_of_boot-4];
    }
 
    // CHECK KEYS!!!
 
    if(file_buffer[end_of_file-1] != boot_buffer[end_of_boot-1] ||
       file_buffer[end_of_file-2] != boot_buffer[end_of_boot-2] ||
       file_buffer[end_of_file-3] != boot_buffer[end_of_boot-3] ||
       file_buffer[end_of_file-4] != boot_buffer[end_of_boot-4] ) {

        print( "This file has been determined to be a copy!" );
        print( "The program has been terminated..." );
        compare[ 0] = hex[ (short)(file_buffer[end_of_file-4] >> 4) & 0x0F];
        compare[ 1] = hex[ (short)(file_buffer[end_of_file-4]     ) & 0x0F];
        compare[ 2] = hex[ (short)(file_buffer[end_of_file-3] >> 4) & 0x0F];
        compare[ 3] = hex[ (short)(file_buffer[end_of_file-3]     ) & 0x0F];
        compare[ 4] = hex[ (short)(file_buffer[end_of_file-2] >> 4) & 0x0F];
        compare[ 5] = hex[ (short)(file_buffer[end_of_file-2]     ) & 0x0F];
        compare[ 6] = hex[ (short)(file_buffer[end_of_file-1] >> 4) & 0x0F];
        compare[ 7] = hex[ (short)(file_buffer[end_of_file-1]     ) & 0x0F];
        compare[ 8] = ' ';
        compare[ 9] = '=';
        compare[10] = ' ';
        compare[11] = hex[ (short)(boot_buffer[end_of_boot-4] >> 4) & 0x0F];
        compare[12] = hex[ (short)(boot_buffer[end_of_boot-4]     ) & 0x0F];
        compare[13] = hex[ (short)(boot_buffer[end_of_boot-3] >> 4) & 0x0F];
        compare[14] = hex[ (short)(boot_buffer[end_of_boot-3]     ) & 0x0F];
        compare[15] = hex[ (short)(boot_buffer[end_of_boot-2] >> 4) & 0x0F];
        compare[16] = hex[ (short)(boot_buffer[end_of_boot-2]     ) & 0x0F];
        compare[17] = hex[ (short)(boot_buffer[end_of_boot-1] >> 4) & 0x0F];
        compare[18] = hex[ (short)(boot_buffer[end_of_boot-1]     ) & 0x0F];
        compare[19] = '\0';
        print(compare);
        return 0;
    }
    return 1;
}

int main( void )
{
    char far *jump;
    struct boot_sector_info bsi;
    struct exe_header_info ehi;
    short starting_cluster, _SS, _SP, _CS, _IP;
    int file_start, psp;

    clear_screen( );
    get_boot_sector_info( &bsi );  
    if( ( starting_cluster = search_root( bsi ) ) == 0 ) {
        print( "Error: file not found!" );
        return 1;
    }
    file_start = get_file_start( );
    psp = file_start - 0x10;  // psp is 256 bytes (16 paragraphs), positioned before the file.
  
    print( "Loading file into memory..." );
    load_file( starting_cluster, file_start, bsi );
  
    print( "Getting exe header..." );
    get_exe_header_info( file_start, &ehi );
  
    print( "Checking ids..." );
    if( check_copied( file_start, ehi, bsi ) ) {
        print( "Relocating pointers..." );
        setup_exe( file_start, ehi );
    
        print( "Setting up psp..." );
        setup_PSP( psp );
    
        print( "Jumping to file..." );
        // Set registers and jump.
        _SS = ( file_start + ehi.relo_SS );
        _SP = ehi.exe_SP;
        _CS = file_start + ehi.hdr_size + ehi.relo_CS;
        _IP = ehi.exe_IP;
        jump = MK_FP( _CS, _IP ); 
        _asm {
            mov DS, psp
            mov ES, psp
          /*mov SS, _SS
            mov SP, _SP*/
           jmpf jump
        };
        print( "Failed to jump!" );
    }
    while( 1 ) {}
    return 0;
}
