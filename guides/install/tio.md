---
title: 'Guide: Install Tio'
---

## What is Tio?

To communicate with your Pi over the USB-serial, you wil need to run a terminal communication program on your laptop. There are a number of programs that support communication with a tty device to choose from (`screen`, `minicom`, `putty`, etc.)  The `screen` program is the lowest common denominator; its one advantage is being universally available without having to install anything extra, but it provides only the most basic functionality and has a beastly UI.  We think you'll do better with something more user-friendly. Our favorite of the bunch is `tio` for its simple interface and nice features, so this is our strong recommendation. (If you already very comfortable with another program, ok to keep using instead).

## Install tio
Use your package manager to install `tio`

-  On __macOS__:
    ```console
    $ brew install tio
    ```
- On __WSL/Ubuntu__:
    ```console
    $ sudo apt install tio
    ```

## Documentation on tio
The tio maintainer has a lovely github page introducing tio and its features [https://github.com/tio/tio](https://github.com/tio/tio?tab=readme-ov-file#tio---a-serial-device-io-tool)

## Configuring tio

You invoke tio with the device to connect to and various flags to configure the communication settings:

```console 
$ tio -b 115200 -d 8 -s 1 -p none  /dev/YOUR_DEVICE_PATH
```

The above command connects to a device and sets the baud rate to 115200 baud, 8 data bits, 1 stop bit, and no parity.

You can use our [find-dev.sh script](/guides/install/cp2102#find-dev) to get the device path
for your CP2102 device.

To exit tio, use the sequence `control-t q`.

Rather than re-type all these flags each time, you can store these settings in your tio configuration file and then retrieve them by name. Create a new file `~/.tioconfig`. Open the file in your editor and paste in the text below which defines a new configuration named `mango`.

```
[mango]
device = /dev/tty.usbserial-0001
baudrate = 115200
databits = 8
parity = none
stopbits = 1
color = 10
```

Save the file and exit your editor. Now when you invoke `tio mango`, tio will look in your `.tioconfig` file to find the the settings for `mango`, no additional flags needed!

> __Pro-tip__
Open a separate window/tab to run `tio` and always keep it running. No need to exit and restart `tio`. When you disconnect or reset your Pi, it will simply pause (prints "Disconnected, Waiting for tty device"). When your Pi resets, tio will automatically reconnect and resume communication.
{: .callout-info}

## Loopback test

Grab your USB-serial adapter and a female-female jumper to test out tio now. Connect TX to RX on the USB-serial adapter in loop back mode, as shown in this photo:

![loop back](/labs/lab3/images/loopback.jpg){: .zoom}

Now start tio and type characters in the tio window to see that they are echoed back.
```console
$ tio /dev/YOUR_DEVICE_PATH
```
In loop back mode, the signals sent out on the TX pin are wired straight to the RX pin. Reading from the RX pin will read the characters sent over TX.

When you connect the TX and RX to the Mango Pi, tio is bridging the communication from your laptop to the Pi and back. Tio sends what you type to the Pi and displays what is received from the Pi. Neat!