Phoenix Top Level Design
========================

Group Members
-------------

__Leader__: Nick Guertin  
Wei Yao Lin  
Curtis J. Aube  

__Contact Email__: n\_guertin@yahoo.com, nguertin@vtc.vsc.edu

User Interface Specifics
------------------------

### User Oriented

The user oriented interface will be the keyboard, or maybe a game pad for inputs
connected to Phoenix. For output it will use the monitor connected to Phoenix.
[It sounds like you are talking about the user interface of a Phoenix
application. That's really a somewhat different matter.]

### Developer Oriented

The developer oriented interface will be the kernel. If a developer wants to
extend the OS they will be interacting with the hardware of Phoenix. [Here you
can talk about how the required interface to Phoenix (as described in the
requirements) will be provided to the programmer.]

System Architecture
-------------------

![System Architecture](./system_architecture.svg)

### Phoenix Kernel

The kernel will handle all messages passing between modules. It will also
provide thread capability. [How will it do these things?]

Input Devices
-------------

The input devices module provides the base level access to all input devices.
This allows a developer to access the input devices in a generic way. It holds a
concept of two players. The game pad for player one will have a set of keys
mapped on the keyboard that function the same. The game pad for player two would
have a different set of keys mapped on the keyboard. If the game uses this
module it will only see two players, not individual devices. [This sounds more
like requirements. In the top level design you should describe how these
features are going to be implemented.]

### Game Pad

The game pad module is a input device driver for a game pad connected to the
game port. It will send messages to the kernel and the input devices module.

### Keyboard

The keyboard module is a input device driver for a keyboard connected to the
PS/2 port. It will send messages to the kernel and the input devices module.

### Video

The video module will receives messages from the kernel which will print
messages on the monitor. This may also be enhanced to display simple graphics
such as shapes with pixel coordinates.

### Sound

The sound module will receives messages from the kernel which will sound the
speaker. This will first only be able to use the internal speaker but may be
enhanced to use the sound card.

### Cartridge

The cartridge module represents the Phoenix application itself. This module
would normally be provided by the application programmer and thus is not
properly part of the Phoenix system. It will, however, interact with the other
modules in the system using the same message passing mechanism as used by the
rest of Phoenix. The name "cartridge module" is in honor of the old console
systems that held the game on a cartridge.

### NIC

The NIC module is a devices driver that will receives messages from the kernel
and send messages through the NIC to communicate with another Phoenix console.

### Identified Risks

* Boot Loader: This is a concern because we never written a boot loader in x86
  Assembly.
* Protect mode: Real mode will likely provide us with enough resources, but if
  time permits, we may try switching to protected mode. So this is a risk that
  we have currently removed from the requirements.
* [There are also risks associated with programming the low level hardware.]

### Problem Partition

Wei Yao Lin:

* Threading in the kernel and clock.
* Keyboard module.
* Input devices module.

Curtis J. Aube:

* Basic Video module.
* The boot loader.
* Sound module.

Nick Guertin:

* Message passing in the kernel.
* Basic test cartridge module.
* NIC module.
