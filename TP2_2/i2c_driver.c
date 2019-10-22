#include "i2c_driver.h"

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
  gpio_request_one(SDA, GPIOF_OUT_INIT_LOW, "SDA");
  gpio_request_one(SCA, GPIOF_IN, "SCA");

  cdev_add(opDev,device,1);

  device_create(classDevice, NULL, device, NULL, "gpio1");
  return 0;
}

static void ch_cleanup(void)
{
    printk(KERN_DEBUG "DRIVER OFFLINE\n");

    gpio_free(SDA);  // SDA
    gpio_free(SCA);  // SCA
    kfree(buffer);
    unregister_chrdev_region(device,1);
    device_destroy(classDevice, device);
    cdev_del(opDev);
    class_destroy (classDevice);
}

ssize_t ch_read (struct file *file, char *bb, size_t size, loff_t *offset)
{

    /*sizeToCopy = copy_to_user (bb, struser, nb_struser);
    if (sizeToCopy != 0)
        return -EMSGSIZE;

    return sizeToCopy;*/
    return 0;
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
