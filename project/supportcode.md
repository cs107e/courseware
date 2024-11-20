---
permalink: /project/supportcode
title: 'Code extras'
---

### Code available in `$CS107E/src` directory
This directory contains our class code for Mango Pi. You are encouraged to use/adapt/learn from this code as part of your project.
- Source code for all `libmango` support modules
    - `ccu` `de` `font` `hdmi` `hstimer` `gpio_interrupt` `interrupts ` `mango` `ps2_keys` `ringbuffer` `uart`
- Source for `spi` and `i2c` modules
    - these drivers have been successfully used in past, minor recent updates to code
- Source for `pwm` and `i2s` modules
    - code for these peripherals is new and needs further testing

### Extras

The `$CS107E/extras` directory contains contributions shared by former students. Some of this code was written for Mango Pi, older code is for Raspberry Pi. These resources are not fully fleshed out, and we have not vetted the code.
- `math_lib`: student's implementation of math functions
- `neopixel`: sample code for Neopixel/WS8212 rgb led

Your project is welcome to use/build on these resources, but be aware the code may be incomplete or incorrect and the effort to port can range from minor nuisance to singificant rewrite.  Be sure to cite the original and be clear about what the additional work you contributed when adopting it. Your project will be primarily evaluated on the portion of the work that is original.

{% comment %}
- Code to read/write files on the [SD card](extras/sd_library)
- Using 2 ESP-32 devices to build a [Uart Wifi bridge](extras/uart-wifi-bridge) between 2 Raspberry Pis
- Using ESP-32 to [serve a web page](extras/webpage)
{% endcomment %}