Phoenix Requirements
====================

Curtis Aube, Wei Lin, Nick Guertin  
Last Revision: 2008-05-05  

Phoenix is a bootable, operating system (OS) driven game console that runs on
standard computer hardware. The operating system is specifically designed to run
a single program or game. A storage device (currently supports floppy disks) is
used as a game 'cartridge'. The cartridge can hold multiple files, each
containing the game and the OS in a single executable. These files can be
selected and launched during the computer's booting process.

Functional Requirements
-----------------------

We have several use cases: 'Start a game', 'Play a game' and 'Create a game'.
There are two types of users that will be using this system. Players will be the
ones playing the games and programmers will be the ones developing games. 'Start
a game' and 'play a game' are both use cases for players. 'Create a game' is a
use case for programmers.

### Use Cases

#### Start a game

##### User

Player

##### User Actions

Place a floppy disk in the system.

Restart the system or turn it on.

Tell system to boot from the disk.

##### Product Actions

Boot into the game and give user control.

##### Extensions

Use a CD instead of a floppy.

Present the user with a list of games on the disk and allow them to choose one
to start.

#### Play a game

##### User

Player

##### User Actions

Input data/movement with a keyboard.

View the screen to see what's happening.

##### Product Actions

Display the game.

Read the user's actions and interpret them based on the game itself.

Make game appropriate sounds.

##### Extensions

Allow use of a gameport or serial based input device.

#### Create a game

##### User

Programmer

##### User Actions

Write a game or application using the API provided.

Edit global header file for the specifics of their program.

Link their program into Phoenix's source.

Modify the make file to use their program.

Put the resulting file on a disk.

Boot the computer from the disk.

Verify the program is running as expected.

##### Product Actions

Will boot into the linked program.

Will allow user to print error messages for debugging.

### Extension Use Cases

#### Save a high score

##### User

Player

##### User Actions

End a game.

Input name.

##### Product Actions

Send name and score to server.

Display scores to the user.

#### Play a networked game.

##### User Actions

Physically connect to the other system.

Tell game to play networked.

##### Product Actions

Connect to the other machine.

Handle communication to play two systems against each other.

### API

#### Messages

    typedef struct {
        int size;                  // the actual number of filled bytes, not MESSAGE_SIZE
                                   // unless the buffer is full
        process src;               // thread ID where this message was created
        process dest;              // thread ID where this message is being sent
        char buffer[MESSAGE_SIZE]; // the message itself
    } message;
    void message_send(message *request, message *response)

The `message_send` function will send a message to the destination address. The
message structure contains the message buffer and the size of the data. Included
are message structures for the request and a response. When the function
returns, the sender will be able to read the response. The structure and
contents of the message and response will be defined for each module type added
to the system.

    void message_receive(message *request)

The `message_receive` function will cause a thread to block until a message is
sent to it. When another thread sends a message to the waiting thread the
message will be placed at the request location and the waiting thread will
resume operation, presumably doing some kind of work on the message.

    void message_reply(message *response)

The `message_reply` function is used to respond to a message sent from
destination. This will cause the destination to return from its call to
`message_send` and process the response.

#### Threading

    int xthread_create(char id, void (*start_routine)(void))

Spawns a new thread with an ID number of id and threads the function pointed at
by `start_routine`.

    int xthread_suspend(char id)

Causes a thread to block until it is told to unblock. This should mostly be used
in the kernel but may be used outside the kernel if desired.

    int xthread_resume(char id)

Unblocks a blocked thread. Should also be used mostly in the kernel.

    int xthread_mutex_init(xthread_mutex_t *mutex)
    int xthread_mutex_lock(xthread_mutex_t *mutex)
    int xthread_mutex_unlock(xthread_mutex_t *mutex)
    int xthread_cond_init(xthread_cond_t *cond)
    int xthread_cond_wait(xthread_cond_t *cond, xthread_mutex_t *mutex)
    int xthread_cond_signal(xthread_cond_t *cond)

The `xthread_mutex` and `xthread_cond` functions should work similarly to the
`pthread_mutex` and `pthread_cond` functions in the POSIX API.

Non-functional Requirements
---------------------------

### Platform

* No operating system is required. The Phoenix OS will be included with each game.
* Real mode 16 bit x86 processor architecture.
* 1MB of RAM.
* Display at a minimum of 30 frames per second.
* Floppy disk drive for booting. No disk I/O after booting.
* Standardized NIC chipset.
* PS/2 port for keyboard.

### Performance

* capable of 30 frames for second.
* Under 200ms response to any input.
* Under 200ms transfer of a message over the network.

### Security

* There are no security concerns for this project.

### Reliability

* The OS doesn't need to recover from errors.

### Users

* Gamers playing with the system. No experience.
* Programmers creating new games.

### Scale

* A maximum of two users on a console at a time.
* A maximum of two consoles on a network.

### Data

* Inputs from keyboard and/or gamepad.
* Network packets.

### Internationalization

* Supports IBM ASCII.

### Environmental

* This console with only run indoors.
* Possible heat concerns for the hardware.

### Possible modifications

* 32 bit protected mode.
* Boot from CD.
* Game port or serial port for gamepad or joystick support.
* UDP for cross-network.

### Requirement date

* April 2008
