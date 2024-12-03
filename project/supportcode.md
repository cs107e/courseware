---
permalink: /project/supportcode
title: 'Code extras'
---

### Libmango and code extras
You are encouraged to use/adapt/learn from any of our published code as part of your project.
- `libmango`
    - Library of our reference modules is `$CS107E/lib/libmango.a`
        - Can also use your modules or your full library `libmymango.a`
    - Source code for `libmango` support modules available in the `$CS107E/src` directory
        - `ccu` `de` `font` `hdmi` `hstimer` `gpio_interrupt` `interrupts ` `mango` `ps2_keys` `ringbuffer` `uart`
- `spi` and `i2c`
    - source in <https://github.com/cs107e/cs107e.github.io/tree/master/lectures/Output/code/extras>
    - these drivers have been successfully used in past, minor recent updates to code
- `pwm` and `i2s`
    - Source in <https://github.com/cs107e/cs107e.github.io/tree/master/lectures/Output/code/extras>
    - code for these modules is new and needs further testing

### Student extras
The `$CS107E/extras` directory contains contributions shared by former students and bits of example code. Some of this code was written for Mango Pi, older code is for Raspberry Pi. Be aware the code may be incomplete or incorrect and the effort to port can range from minor nuisance to singificant rewrite.
- `math_lib`: student's implementation of math functions
- `neopixel`: sample code for Neopixel/WS8212 rgb led

### Code found out in the wild
Maker sites such as Adafruit, SparkFun, and DFRobot publish great tutorials and lots of example code. Their libraries tend to be well-tested and very complete, good resources to learn from!  Broad web search will unearth all kinds of random code on github and blogs; expect a hodge-podge in terms of quality and completeness. Note that code you find in the wild is very unlikely to be direct drop-in to our environment (RISC-V, Mango Pi, bare-metal), but studying the code can provide useful insights and a starting point for a port.

### Citation
If your project adopts from any resources you did not author yourself, they should be properly cited!
- Citation should include author, url, what was used of their work
- Explain what changes/extensions you contributed on top of the original (be specific)
- Your project will be primarily evaluated the portion of the work that you contributed

{% comment %}
- Code to read/write files on the [SD card](extras/sd_library)
- Using 2 ESP-32 devices to build a [Uart Wifi bridge](extras/uart-wifi-bridge) between 2 Raspberry Pis
- Using ESP-32 to [serve a web page](extras/webpage)
{% endcomment %}