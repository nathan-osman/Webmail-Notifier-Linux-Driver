## Webmail-Notifier-Linux-Driver

This project intends to create a working Linux device driver for the [Dream Cheeky USB Webmail Notifier](http://www.dreamcheeky.com/webmail-notifier).

### The Device

The device itself consists of a small plastic shell in the shape of an envelope. According to the website, its physical dimensions are approximately 4.7cm x 7.5cm x 3 cm.

![](http://www.dreamcheeky.com/sites/default/files/imagecache/product_big/sites/default/files/webmailnotifier3.jpg)

On a more technical note, the USB vendor and product ID are as follows:

**Vender:** `1D34`  
**Product:** `0004`

### The Protocol

This device employs an extremely simple protocol. Using a USB logging tool, I discovered that the device consists of three LEDs - one red, one green, and one blue. They can each be set to one of 0x40 different intensities.

A sample block of data sent to the device to make it appear blue would look like this:

    00 00 40 00 00 00 1F 05

I have no idea what the `1F 05` is for, but it seems to exist in every command dispatched, so I include it when sending commands to the device. Perhaps some more experimentation will reveal its purpose.

### Building the Module

Building the module is easy if you have the headers for your current kernel installed. Just run:

    make