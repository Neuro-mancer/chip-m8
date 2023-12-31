# CHIP-M8 a CHIP-8 Emulator

This is my repository for CHIP-M8, a CHIP-8 emulator written from scratch in C
with [SDL (Simple Direct Media Layer)](https://www.libsdl.org/). It uses SDL to
manage the graphics, sound, and keyboard input, Ncurses for a simple debugging
TUI, the standard C library. 

I plan on adding more features to this in the
future and optimizing it as much as possible. It has been a great way to
exercise my skills and also a pleasure to create as someone who is intrigued
about computer engineering, computer science, and programming in general.

I undertook this project because I wanted an excuse to brush up on my C skills and
finally give myself an excuse to learn the basics of SDL and emulation. I am
this repository's sole maintainer.


![CHIP-M8 Screenshot](https://github.com/Neuro-mancer/chip-m8/blob/main/docs/emulator.png?raw=true "CHIP-M8")


## What is CHIP-8?


### CHIP-8 Overview and History

[CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) is an interpreted programming
language by Joseph Weisbecker originally designed for the COSMAC VIP and Telmac
microcomputers in the 1970s. These computers used the same 8-bit processor, the
[RCA 1802](https://en.wikipedia.org/wiki/RCA_1802) (also designed by
Weisbecker). It was intended to make programming video games easier for these
computers, but continues to be used by hobbyists because of its simplicity. The
programs run on a "virtual machine"/"emulator" and functioned as a friendlier
abstraction to the RCA 1802's machine language. 

People have also made
extensions/changes to the CHIP-8 language, so various similar but distinct
versions exist, including (but not limited to) SUPER-CHIP and XO-CHIP. The
various quirks of the language help to make game development easier, especially
with the logic of the display. The language experienced a revival of sorts in
the 90s in graphing calculators.

### How does a CHIP-8 emulator/interpeter "hardware" generally work?

CHIP-8 program files (sometimes referred to as "ROMS") contain a series of hex
opcodes and data that are loaded into program memory. In total there are 35
opcodes [(read about them
here)](https://en.wikipedia.org/wiki/CHIP-8#Opcode_table). The virtual machine
has 4KB of byte-sized memory locations allocated for its purposes (12-bits of
addressable memory). The first 512 addresses were originally reserved for the
interpreter and font sprites, with the rest being used to hold program memory
and data. I believe some of the upper addresses were also reserved for the stack
and display in the original CHIP-8. CHIP-8 contains a 16-bit program counter to
hold the address of the next instruction to be executed (in practice only 12
bits are ever used, although I've heard some original CHIP-8 games do access
more memory). Program control flow is generally altered via an unconditional
jump to a 12-bit address specified. 

The only other jumps are "skip"
instructions, which simply increment the PC an extra address if certain
conditions/flags are met. The emulation follows the general flow of a CPU, that
is, it fetches the instruction from the address in the PC, the instruction is
decoded, and the instruction is executed, and then PC is incremented. There are
16 general purpose registers (V0-VF) that can each hold a byte (with VF being
used to store flags), one 16-bit address register used to access memory
locations (generally for data like sprites and fonts), and a stack used to hold
16-bit return addresses (which varies in total size from implementation to
implementation). 

The keyboard is 16 keys, corresponding to the hex keypad on the
original COSMAC VIP. The CHIP-8 also has two 8-bit timer registers which
decrement at a rate of 60 Hertz, one for sound (which beeps unless zero) and a
"delay" timer for timing in programs. The display is simply 64x32 pixels and
monochrome. The pixels are kept track of in a buffer which contains pixel color
data; when a pixel is placed on the screen an XOR operation is performed with
the sprite data and the current buffer data. If a pixel to be placed is "on" and
the pixel in the current buffer location is "on", then the pixel drawn will be
"off" and a flag set in register VF indicating this. This, consequently, makes
sprite collision detection much easier for the game developer!


## CHIP-M8 Features and Specifications


### Virtual Machine Specifications

- 16 general purpose 8-bit registers
- 32 bytes of stack memory for subroutine calls
- ~4 kilobytes of general program memory
- 60Hz sound timer and delay timer
- 64x32 monochrome display

### Features

- Written in C with SDL
- Pause/Play emulation
- Adjust emulation speed on the fly
- Change the two monochrome display colors
- Simple debugging mode interface in ncurses
- Will compile and run on GNU/Linux
- 1024x512 Resolution


## Planned Features


- Add support for common CHIP-8 extensions (SUPER-CHIP and XO-CHIP)
- ~~Add adjustable emulation speed~~
- ~~Ability for the user to change the monochrome display colors to suit their
  fancy~~
- Support for additional resolutions and resizing
- Multithreading
- ~~Debugging Mode~~
- Windows support?


## Installation


### Dependencies

Currently, CHIP-M8 only supports GNU/Linux. This project
requires SDL2, SDL2_Mixer, Ncurses, GCC, and Make to build, below details the 
process for acquiring the dependencies on your distribution:

#### Arch Linux 

`sudo pacman -S gcc make sdl2 sdl2_mixer ncurses`

#### Debian and Ubuntu 

`sudo apt install libsdl2-dev libsdl2-mixer-dev libncurses-dev gcc make`

### Cloning and Running

Before running it, you need to clone and make the project: 

`git clone https://github.com/Neuro-mancer/chip-m8`

`cd chip-m8 && make`

To run, simply type: 

`./emulator <CHIP-8 ROM File>`


## How to Use


### Finding CHIP-8 "ROMs"

First, find some CHIP-8 ROMs to play on the internet. It should be
noted that some may be unstable or adhere to certain quirks that are anachronistic
to a CHIP-8 emulator like this. That is to say, not every game will adhere
to the rules of the original CHIP-8 standards (said standards are not well
documented and inconsistent over the many generations of emulators) and some
roms will likely not work. 

Next, go into the directory where the project was built and run the emulator
passing the ROM filepath as an argument. This will start the emulation.

### Emulator Control Scheme

Because the original COSMAC VIP had a hexadecimal keypad, certain games may
give the controls as certain hexadecimal values. For my emulator the bindings
are analogous to the COSMAC keypad positions. Here is a diagram of the controls
on a QWERTY keyboard and its analogous COSMAC hex keypad:

Controls:
```
EMULATOR                      COSMAC
-----------------  -----------------
| 1 | 2 | 3 | 4 |  | 1 | 2 | 3 | C |
-----------------  -----------------
| Q | W | E | R |  | 4 | 5 | 6 | D |
-----------------  -----------------
| A | S | D | F |  | 7 | 8 | 9 | E |
-----------------  -----------------
| Z | X | C | V |  | A | 0 | B | F |
-----------------  -----------------
```

Here is a list of bindings specific to my emulator control flow:

- ESCAPE - Pause/Unpause emulation
- SPACE - Stop emulator
- UP ARROW/DOWN ARROW - Speed up/slow down the execution of emulator instructions 
  by ~50Hz, respectively

### Emulator Options

#### Change Colors

Chip-M8 has several command line options. Among these is support for 
changing the colors used on the monochrome display. In order to do this,
pass the flags `-r [RGB Decimal Red Value here]` to change foreground color.
For example, if I wanted to change the foreground color to a light pink:

`./emulator <Chip-8 ROM> -r 255 -g 204 -b 229`

If I wanted to change the background color, I simply pass the flag
`-R [RGB Decimal Red Value Here]`. So, if I wanted to change both
the background and foreground color to pinkish colors, I simply type:

`./emulator <Chip-8 ROM> -r 255 -g 204 -b 229 -R 255 -G 153 -B 204`

Here is what it should look like:

![CHIP-M8 Color Change](https://github.com/Neuro-mancer/chip-m8/blob/main/docs/emulatorcolor.png?raw=true "CHIP-M8 Color")

#### Open the debugging interface

If you want to see the values of registers and other abstracted data of the
emulator, simply type:

`./emulator <Chip-8 ROM> -d`

This will open an Ncurses buffer in the terminal window you opened it with
and will update the values pertaining to program execution as it runs. This is
what the interface looks like:

![CHIP-M8 Debugging Interface](https://github.com/Neuro-mancer/chip-m8/blob/main/docs/debug.png?raw=true "CHIP-M8 Debugging Interface")
