/* Webmail Notifier Linux Driver
   Copyright (C) 2013 Nathan Osman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

/* Utility macro for logging information / error messages. */
#define WN_DEBUG(msg, args...) printk(KERN_DEBUG msg "\n", ##args)

/* The vendor and product ID of our device. */
#define WN_VENDOR_ID  0x1D34
#define WN_PRODUCT_ID 0x0004

/* We store some device-specific information in this struct. */
struct usb_wn {
    struct usb_device * udev;
};

/* Define the table listing the vendor and device IDs of devices
   that this module was written for. */
static struct usb_device_id wn_table[] = {
    {
        USB_DEVICE(WN_VENDOR_ID, WN_PRODUCT_ID)
    },
    {}
};
MODULE_DEVICE_TABLE(usb, wn_table);

/* Forward declare the wn_driver instance. */
static struct usb_driver wn_driver;

/* Set the device to the specified color. */
static int wn_set_color(struct usb_device * udev,
                        u8 red, u8 green, u8 blue)
{
    /* Create the data buffer to be sent to the device. */
    u8 buf[8] = {
        red, green, blue, 0, 0, 0, 0x1F, 0x05
    };
    
    /* Send the data to the device. */
    return usb_control_msg(udev,
                           usb_sndctrlpipe(udev, 0),
                           0x09,
                           0x21,
                           0x0200,
                           0,
                           buf,
                           8,
                           0);
}

/* Called when our device is opened. */
static int wn_open(struct inode * inode,
                   struct file * file)
{
    int subminor = iminor(inode);
    struct usb_interface * interface = usb_find_interface(&wn_driver, subminor);
    
    /* Ensure that the interface is valid. */
    if(!interface)
    {
        WN_DEBUG("unable to find interface for device file");
        return -ENODEV;
    }
    
    /* Store a pointer to the usb_wn instance with the file. */
    file->private_data = usb_get_intfdata(interface);
    
    return 0;
}

/* Called when our device is closed. */
static int wn_release(struct inode * inode,
                      struct file * file)
{
    return 0;
}

/* Called when data is written to the device (to change the color
   for example). */
static ssize_t wn_write(struct file * file,
                        const char __user * user_buf,
                        size_t count,
                        loff_t * ppos)
{
    struct usb_wn * dev = file->private_data;

    /* Verify that the device is still valid. */
    if(!dev->udev)
    {
        WN_DEBUG("usb_device is NULL");
        return -ENODEV;
    }
    
    return wn_set_color(dev->udev, 0x40, 0, 0);
}

/* Table containing the list of file operation functions. */
static struct file_operations wn_fops = {
    .owner =    THIS_MODULE,
    .open =     wn_open,
    .release =  wn_release,
    .write =    wn_write
};

/* ... */
static struct usb_class_driver wn_class = {
    .name       = "wn%d",
    .fops       = &wn_fops
};

/* Called when one of the decives is connected. */
static int wn_probe(struct usb_interface * interface,
                    const struct usb_device_id * id)
{
    struct usb_device * udev = interface_to_usbdev(interface);
    struct usb_wn * dev = NULL;
    int retval = -ENODEV;
    
    /* Ensure that the interface exists. */
    if(!udev)
    {
        WN_DEBUG("interface_to_usbdev returned NULL");
        goto error;
    }
    
    /* Allocate storage space for the usb_wn struct and store
       it with the interface. */
    dev = kzalloc(sizeof(struct usb_wn), GFP_KERNEL);
    if(!dev)
    {
        WN_DEBUG("unable to allocate memory for usb_wn struct");
        retval = -ENOMEM;
        goto error;
    }
    
    /* Associate the usb_wn with the interface. */
    dev->udev = udev;
    usb_set_intfdata(interface, dev);
    
    /* Now attempt to actually create the device. */
    retval = usb_register_dev(interface, &wn_class);
    
    WN_DEBUG("device connected and initialized");
    return retval;

error:
    
    /* Free the memory used by the structure if an error occurred. */
    kfree(dev);
    return retval;
}

/* Called when one of the devices is disconnected. */
static void wn_disconnect(struct usb_interface * interface)
{
    /* Retrieve the data for the device and free it. */
    struct usb_wn * dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);
    kfree(dev);
    
    /* Deregister the device we created. */
    usb_deregister_dev(interface, &wn_class);
    
    WN_DEBUG("device has been disconnected");
}

/* Driver information table. */
static struct usb_driver wn_driver = {
    .name =       "webmail_notifier",
    .probe =      wn_probe,
    .disconnect = wn_disconnect,
    .id_table =   wn_table
};

/* Called when our module is loaded into the kernel. */
static int __init usb_wn_init(void)
{
    /* Attempt to register the USB driver. */
    int retval = usb_register(&wn_driver);
    
    /* Either way, log the status. */
    if(retval) WN_DEBUG("failed to register the webmail_notifier driver");
    else       WN_DEBUG("webmail_nofitifier driver registered");
    
    return retval;
}

/* Called when our device is unloaded from the kernel. */
static void __exit usb_wn_exit(void)
{
    /* Remove the USB driver. */
    usb_deregister(&wn_driver);
    
    WN_DEBUG("webmail_nofitifier driver registered");
}

module_init(usb_wn_init);
module_exit(usb_wn_exit);

/* Define the license and provide a brief description of our module. */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nathan Osman <admin@quickmediasolutions.com>");
MODULE_DESCRIPTION("Dream Cheeky Webmail Notifier driver");
MODULE_VERSION("1.0");
