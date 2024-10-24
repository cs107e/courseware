---
title: "Notes on Assign2 extension"
---

This is the first time we are trying this new extension for the clock assignment. You will be our pioneers!
Our goal is to give you a fun way to build on the work you put into your gpio/timer modules and perhaps
sprout a few inspirations that resurface in final projects.


Here are some notes on the devices that you might find helpful.  The datasheet from the manufacturer sometimes leaves a
lot to be desired, so we have some suggested tutorials as well.


### Inputs
- __Rotary encoder knob__
  - Can turn clockwise or counterclockwise, and press. You can also turn while pressing. Satisfying clicky mechanical feel.
  - Possibilities: turn slow one-click-at-time to fine tune, quick sweep to make large change? use press in conjunction with turn?
  - Will seat in breadboard with a little bit of prodding.
  - We have lots of these.  I also have a handful of a different type without center press where shalft has LED that turns on/off.
  - The manufacturer [datasheet](../datasheets/EC11.pdf) is pretty primitive, lighted shaft [version](../datasheets/LightedPEL12S.pdf) is easier to read (has similar design). An [overview of how encoder works](https://learn.adafruit.com/rotary-encoder/overview) from Adafruit.
- __5-way navigation switch__
  - Like a mini-joystick, rock to compass points (NSEW), push in center. Buttons are exclusive (can only activate one at a time).
  - Possibilities: press and release once to fine-tune, press and hold to make large change? move between minutes/seconds and set separately?
  - I have just a few of these and their sad, spindly legs don't seat well in a breadboard - Boo!
  - However, I ordered some from different supplier to arrive on Friday that may be better. Also building a breakout on perfboard might help. Stay tuned.
    - Friday update: new parts are better construction, though joystick is extra tiny. This switch needs mount on perfboard. Here is a photo of the one I made.
    ![nav breakout](../images/5way_breakout.jpg){: .zoom .w-25}
    Fun soldering practice to build your own! When you pick up a switch, also get perfboard and headers from us. Lab has soldering iron and supplies. Ask one of the staff for a demo if you are new to soldering!
  - Manufacturer [datasheet](../datasheets/SKQUCAA010-ALPS.pdf). This is basically 5 push buttons bundled into one component.
- __Membrane keypad__
  - Reports press of single button and multiple concurrent buttons.
  - Possibilities: uh, it's a number pad... clear affordance, fewer options for creativity. Error-check user input (too large, out of bounds)?
  - Has dupont connector, easy to run jumpers to breadboard.
  - I have a half-dozen of these.
  - The datasheet for this particular keypad was nowhere to be found, but here is a [summary](https://protosupplies.com/product/membrane-keypad-4x3-matrix/) provided by the supplier (ProtoSupplies).

![input devices](../images/ext_input.jpeg){: .zoom}

### Outputs

- __Passive buzzer__
  - Tiny little speaker, sound quality not bad.
  - Possibilities: play notes/melody, make sound effects (sci fi laser gun, sad trombone), fun!
  - Seats like a champ on a breadboard
  - I have a ton of these
  -  [Specs](https://www.amazon.com/dp/B07GBTJP9F) from supplier, an [overview of passive buzzer](https://protosupplies.com/product/passive-buzzer-5v-module/) from ProtoSupplies
- __Common cathode RGB LED__
  - Possibilities: dim->bright, color cycle ?
  - Perfect on a breadboard
  - Tweaking resistor value per individual RGB may help with achieving a more balanced white
  - I have plenty of these
  - Manufacturer [datasheet](../datasheets/YSL-R1047CR4G3BW-F8.pdf), an [overview of RGB led](https://protosupplies.com/product/led-rgb-5mm-clear-cc/) from ProtoSupplies
- __Vibrating disc motor__
  - Possibilities: buzzy patterns, ramp up/down, S-O-S ?
  - Tiny wires not good for direct connect to breadboard (wiggles out when vibrating). Recommend soldering a graft to wire/jumper for sturdier connection.
  - Motor is plucky for its size. Try attaching to a coin and sit on a desk to further amplify the motion.
  - I have about 20 of these
  - Manufacturer [datasheet](../datasheets/P1012.pdf), general [info](https://www.adafruit.com/product/1201) from vendor

![output devices](../images/ext_output.jpeg){: .zoom}

Come find me (Julie) to pick up a device. We ask that you return the input device to us so we can reuse them, but you are welcome to keep the output device.

If you have a different device of your own that you want to use instead, let's talk!
