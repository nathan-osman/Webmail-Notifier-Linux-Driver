## Webmail Notifier Linux Driver

This project intends to create a working Linux device driver for the [Dream Cheeky USB Webmail Notifier](http://www.dreamcheeky.com/webmail-notifier).

### The Device

The device itself consists of a small plastic shell in the shape of an envelope. According to the website, its physical dimensions are approximately 4.7cm x 7.5cm x 3 cm. The picture below demonstrates its appearance when lit:

![](http://i.stack.imgur.com/EaltB.png)

On a more technical note, the USB vendor and product ID are as follows:

**Vender:** `1d34`  
**Product:** `0004`

### The Protocol

This device employs an extremely simple protocol. Using a USB logging tool, I discovered that the device consists of three LEDs - one red, one green, and one blue. They can each be set to one of 0x40 different intensities.

A sample block of data sent to the device to make it appear blue would look like this:

    00 00 40 00 00 00 1f 05

I have no idea what the `1f 05` is for, but it seems to exist in every command dispatched, so I include it when sending commands to the device. Perhaps some more experimentation will reveal its purpose.

### Building the Module

Building the module is easy if you have the headers for your current kernel installed. Just run:

    make

### Using the Module

You can load the module into the kernel with:

    insmod webmail_notifier.ko

Assuming you either have the device plugged in or plug it in after loading the module, you should have a file `/dev/wn0` that you can use to control the device. Color values are specified using CSS hex notation:

    echo "#ff0000" >/dev/wn0

Using the above command will set the device color to red.
