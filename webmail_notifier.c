/* Copyright (c) 2013 Nathan Osman

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE. */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>

/* The vendor and product ID of our device. */
#define WN_VENDOR  0x1D34
#define WN_PRODUCT 0x0004

/* Define the table listing the vendor and device IDs of devices
   that this module was written for. */
static struct usb_device_id wn_table[] = {
    {
        USB_DEVICE(WN_VENDOR, WN_PRODUCT)
    },
    {}
};

/* Called when our device is opened. */
static int wn_open(struct inode * inode,
                   struct file * file)
{
    //...
}

/* Called when our device is closed. */
static int wn_release(struct inode * inode,
                      struct file * file)
{
    //...
}

/* Called when data is written to the device (to change the color
   for example). */
static ssize_t wn_write(struct file * file,
                        const char __user * user_buf,
                        size_t count,
                        loff_t * ppos)
{
    //...
}

/* Create the table containing the list of file operation functions. */
static struct file_operations wn_fops = {
    .owner =    THIS_MODULE,
    .open =     wn_open,
    .release =  wn_release,
    .write =    wn_write
};

/* Called when one of the decives is connected. */
static int wn_probe(struct usb_interface * interface,
                    const struct usb_device_id * id)
{
    //...
}

/* Called when one of the devices is disconnected. */
static void wn_disconnect(struct usb_interface *interface)
{
    //...
}

static struct usb_driver ml_driver = {
    .name =       "webmail_notifier",
    .id_table =   wn_table,
    .probe =      wn_probe,
    .disconnect = wn_disconnect
};

/* Called when our module is loaded into the kernel. */
static int __init usb_wn_init(void)
{
    //...
}

/* Called when our device is unloaded from the kernel. */
static void __exit usb_wn_exit(void)
{
    //...
}

module_init(usb_wn_init);
module_exit(usb_wn_exit);