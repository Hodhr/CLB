#include "gpio_driver.h"

static int ch_init(void)
{
  printk(KERN_DEBUG "DRIVER ONLINE\n");
  printk(KERN_INFO "/dev/gpio1\n");

  if (alloc_chrdev_region(&device,0,1,"gpioch") == -1) {
    printk(KERN_ERR "ERROR : alloc_chrdev_region\n");
    return -EINVAL;
  }

  classDevice = class_create(THIS_MODULE, "gpioch");

  opDev = cdev_alloc();
  opDev->ops = &fops;
  opDev->owner = THIS_MODULE;
  gpio_request_one(TRIGGER, GPIOF_OUT_INIT_LOW, "TRIGGER");
  gpio_request_one(ECHO, GPIOF_IN, "ECHO");

  cdev_add(opDev,device,1);

  device_create(classDevice, NULL, device, NULL, "gpio1");
  return 0;
}

static void ch_cleanup(void)
{
    printk(KERN_DEBUG "DRIVER OFFLINE\n");

    gpio_free(TRIGGER);  // TRIGGER
    gpio_free(ECHO);     // ECHO
    kfree(buffer);
    unregister_chrdev_region(device,1);
    device_destroy(classDevice, device);
    cdev_del(opDev);
    class_destroy (classDevice);
}

ssize_t ch_read (struct file *file, char *bb, size_t size, loff_t *offset)
{
    int value;
    char struser[10];
    int nb_struser;
    size_t sizeToCopy;
    unsigned long timeSig;
    ktime_t bigTimer;
    ktime_t beginTimer;
    ktime_t endTimer;

    gpio_set_value (TRIGGER, HI);
    udelay (10);
    gpio_set_value (TRIGGER, LE);

    bigTimer = ktime_get();
    beginTimer = ktime_get();
    while (gpio_get_value (ECHO) == 0){
        beginTimer = ktime_get();
        if (ktime_ms_delta (ktime_get(), bigTimer) > 1000){
            return -ETIME;
        }
    }

    endTimer = ktime_get();
    while (gpio_get_value (ECHO) != 0){
        endTimer = ktime_get();
        if (ktime_ms_delta (ktime_get(), bigTimer) > 1000)
            return -ETIME;
    }

    timeSig = ktime_us_delta (endTimer, beginTimer) ;
    //convert the data in cm/s   
    value = (timeSig * 343) / 10000;
    // Distance in cm
    value = value / 2;
    nb_struser = sprintf (struser, "%d", value);

    printk(KERN_DEBUG "Distance in cm : %d\n", value);

    sizeToCopy = copy_to_user (bb, struser, nb_struser);
    if (sizeToCopy != 0)
        return -EMSGSIZE;

    return sizeToCopy;
}

ssize_t ch_write (struct file *file, const char *bb, size_t size, loff_t *offset)
{
    return 0;
}


int ch_open (struct inode * inode, struct file * file)
{
    printk(KERN_DEBUG "DRIVER OPEN \n");
    return 0;
}

int ch_release (struct inode * inode, struct file * file)
{
  printk(KERN_DEBUG "DRIVER RELEASE \n");
  return 0;
}

module_init(ch_init);
module_exit(ch_cleanup);

MODULE_LICENSE(LICENCE);
MODULE_AUTHOR(AUTEUR);
MODULE_DESCRIPTION(DESCRIPTION);
