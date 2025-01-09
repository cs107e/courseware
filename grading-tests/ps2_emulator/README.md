---
layout: base
permalink: /ps2
---
# PS2 Emulator 

### Motivation and design
To grade a student's `ps2` module, we need to send scancodes to the program under test. 
Manually typing on a keyboard would work if we only had a few students to test, but that doesn't scale. 
Instead we wrote emulator program that pretends to be a keyboard.  The emulator program feeds a fixed sequence of scancodes over GPIO to the student program. The student's code doesn't know (and should not matter!) that scancodes are not coming from an actual keyboard. We then test that the student program can correctly read the sequence.

The emulator program runs on a separate Pi.  Sending a scancode is fairly simple: 
it is a packet of 11 bits, for each bit, set data level and make falling clock edge. 
The tricky issues are in ensuring correct timing, negotiating between the two Pis, and error handling/recovery.

### Configuration
You will need Mango Pi + Raspberry Pi: The Mango Pi is usual test "driver" (i.e. running student's code to test), RPi is running the emulator program. Three GPIO pins (one each for clock, data, and handshake) of the two are connected  The test driver program talks to emulator RPi on handshake pin to request a sequence. The emulator RPi sends the scancode sequence to the driver MPi over the clock and data pins.

1. Prepare __SD card__ for emulator RPi:
   - `make install` will compile `ps2_emulator.bin` and copy to the SD card
```console
$ RPI_CS107E=<location of rpi cs107e directory> make install
```
   - Eject SD card and insert it into emulator RPi.
1. Connect the emulator to driver
   - Clock:  __GPIO pin 3__ on emulator RPi <-> PD12 on Mango Pi
   - Data:  __GPIO pin 4__ on emulator RPi  <-> PD13 on Mango Pi
   - Handshake: __GPIO pin 17__ on emulator RPi  <-> PD11 on Mango Pi
   handshake.
   - Daisy-chain __5V power__ and __ground__ from driver to emulator.
1. Plug driver into laptop to power up both Pi's.
1. Confirm configuration is working using `test_driver` from assign5. See [Testing](#testing) below for info on test driver.
   
The emulator Pi uses the ACT(green) and PWR(red) LEDs to tell you what it is doing.  Here is how to read the tea leaves:
- Red on, green slow toggle means emulator RPi is waiting for handshake. This is where emulator RPi spends most of its time.
- During handshake, slow pulse both green and red N times where N is the sequence number requested.
- During send of scancode sequence, both green and red stay on solid. 
- After sending, emulator goes back to waiting for handshake (red on, green slow toggle)
- Fast blinking of red indicates something unexpected has gone wrong and emulator RPi needs to be reset.

### Usage
The emulator currently can send 5 different fixed scancode sequences, numbered from 2 to 6. To request a sequence, the driver MPi pulses the handshake pin (high-low, 30 us per pulse) the
number of times corresponding to selected sequence number.

Sequence 2 tests core functionality of `ps2_read`. It sends every scancode from `0xff` down to `0x1` in decreasing sequential order. Sequences 3,4,5,6 are various malformed sequences that test error handling/resynch. Read code in `ps2_emulator.c` for specific details of each sequence.

See `assign5/agtest_ps2*.c` for ps2 autograder tests. The `assign7/agtest_ps2*.c` are very similar tests, but in context of interrupts.

### Testing
If you ever need to change the emulator or scancode sequences, be sure to thoroughly test!
The timing is sensitive and details matter. There is a `test_driver` program in `assign5` directory
 that you can use for testing to confirm that your changes haven't broken anything.

The `test_driver` exercises the `ps2` module from ref library. It
verifies handshake with emulator, requests each available sequence
and narrates activity.

April 2022: Note that the latest version of the ps2 autograder tests have become aggressive about verifying the setup. The previous tests would silently report a pass even when there was no activity on emulator. The revised tests will not continue if cannot confirm that the emulator is active. If you need to test your configuration, use the `test_driver` program to troubleshoot.
