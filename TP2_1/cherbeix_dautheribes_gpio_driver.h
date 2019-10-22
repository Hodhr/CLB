#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/ktime.h>


#define AUTEUR  "CHERBEIX Clément clement.cherbeix@gmail.com"
#define LICENCE "GPL"
#define DESCRIPTION "Char device used to read on raspberrypi - GPIO"

#define MIN(a,b) a<b ? a : b
#define LENGTH 256 // Buffer size
#define HI 1 // hight state
#define LE 0 // low state
#define TRIGGER 17
#define ECHO 27

// operations list
ssize_t ch_read (struct file *, char *, size_t, loff_t *);
ssize_t ch_write (struct file *, const char *, size_t, loff_t *);
int ch_open (struct inode *, struct file *);
int ch_release (struct inode *, struct file *);

struct Buffer {
  char* data;
  size_t size;
};

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = ch_open,
  .release = ch_release,
  .read = ch_read,
  .write = ch_write
};

struct Buffer *buffer = NULL;
dev_t device; // loaded device
struct cdev *opDev = NULL; // device operations
struct class *classDevice = NULL; // Device class
