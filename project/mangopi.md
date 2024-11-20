---
permalink: /project/mangopi
title: 'Mango Pi peripherals'
---

There is a lot of functionality packed within the Mango Pi board that you may want to explore. A few of these features have been successfully used in past (i2c, spi, i2s), the others are challenges open for pioneers!

### D-1 peripherals
The hardware peripherals listed below built into the D1 chip. Refer to chapter references in [D-1 User Manual](/readings/d1-h_user_manual_v1.0.pdf) for more info. We have support code available for some of these, but many are yet to be explored.

* CCU (clock controller) Ch 3.2
    - many peripherals require clock config as part of init
    - source for our ccu reference module in `$CS107E/src`
* DMA (direct memory access) Ch 3.9
    - DMA used for fast data transfer to/from peripherals, offload from CPU
* THS (thermal sensor) Ch 3.10
    - monitor for temperature of chip
* RTC (real-time clock) Ch 3.14
    - Mango Pi does not maintain independent power for RTC domain
    - if backup power needed, consider instead battery-backed external RTC, e.g. DS3231
* Video graphics Ch 4
    - Display Engine, G2D (hardware acceleration 2-D graphics), Video Engine
    - our de reference module in `$CS107E/src`
    - here is spec for [Display Engine 2.0](/readings/Allwinner_DE2.0_Spec_V1.0.pdf)
* Video output Ch 5
    - pipeline of pixels to display hardware (HDMI, LCD, MIPI, TCON)
    - our hdmi/tcon reference module in `$CS107E/src`
* SMHC (read/write sd card) Ch 7.2
    - we have draft version of brand-new code to load boot program from sd card FAT filesystem, needs further testing
* Audio (I2S/PCM, DMIC, OWA) Ch 8
    - code for I2S driver in `$CS107E/src`
    - draft version, newish code, needs further testing
* TWI (Two-Wire,i.e. i2c)  Ch 9.1
    - code for I2S driver in `$CS107E/src`
    - driver successfully used in past, minor recent updates to code
* UART (async receiver/transmitter) Ch 9.2
    - our uart reference module in `$CS107E/src`
* SPI (serial peripheral) Ch 9.3
    - code for draft I2S driver in `$CS107E/src`
    - driver successfully used in past, minor recent updates to code
* USB (universal serial bus) Ch 9.6
* ADC (analog-digital conversion) Ch 9.8-9.10
    - several ADCs avail on-chip, but no analog inputs  broken out on Mango Pi board, sigh
    - consider instead external ADC, e.g. MCP3008
* PWM (pulse width modulation) Ch 9.11
    - code for draft PWM driver in `$CS107E/src`
    - code is brand-new, needs further testing
* LEDC (led controller) Ch 9.12
    - supports precise timing for neopixel-like rgb led strips, offload from CPU to hw peripheral
* EMAC (ethernet) Ch 9.13
* IR (infrared receiver/transmitter) Ch 9.14-9.15
    - hw peripheral for encoding/decoding protocol, use in conjuction with IR detector and/or emitter
* Security system (crypto engine) Ch 10

### Mango Pi wifi/bluetooth
There is a RealTek rtl8723ds wifi and bluetooth module on-board the Mango Pi. We would love to have either/both working bare-metal but as yet have not cracked the code. Perhaps this will be your contribution!
- Recent linux distros support both wifi and bluetooth on Mango Pi, which gives us hope.
- Documentation is hard to find and not that helpful (Realtek proprietary/secret?)
- If you're not up for blazing trails and just want client-use of wireless communication, consider instead external module (e.g. ESP32)?

### Vector unit
The RISC-V processor of the MangoPI includes a vector unit that can be used for SIMD operations. Former student Yifang Yang wrote up a [starter guide](vector_unit).


