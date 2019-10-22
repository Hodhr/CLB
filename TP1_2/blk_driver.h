#ifndef __BLK_DRIVER_HEADER__
#define __BLK_DRIVER_HEADER__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/blk_types.h>
#include <linux/errno.h>
#include <linux/ioctl.h>

#define LICENCE "GPL"
#define AUTEUR "Clement CHERBEIX clement.cherbeix@univ-tlse3.fr"
#define DESCRIPTION "base block driver"
#define DEVICE "blk_driver"
#define LENGHT_SECTOR 512
#define NB_DISK 16

#define BDC_IOC_MAGIC  'b'
#define IO_CRYPT _IOW (BDC_IOC_MAGIC, 1, int)

int major;
static char psswd [25] = "";
int NB_SECTOR = 512;
module_param (NB_SECTOR, int, 0);

int rb_transfert (struct request* rq);
void rb_request (struct request_queue *q);
void fun_io_crypt (void);
int rb_ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
int init_module(void);
void cleanup_module(void);
int rb_open (struct block_device * bd, fmode_t fm);
void rb_close (struct gendisk * bd, fmode_t fm);
int rb_getgeo (struct block_device * bd, struct hd_geometry * hdd);


static struct rb_device
{
    unsigned int size;              /* Size of the device (in sectors) */
    spinlock_t lock;                /* For exclusive access to our request queue */
    struct request_queue *rb_queue; /* Our request queue */
    struct gendisk *rb_disk;        /* kernel's internal representation */
    char * data;					/* pimp my data */

} rb_dev;

static struct block_device_operations rb_fops =
{
  	.owner = THIS_MODULE,
  	.open = rb_open,
  	.release = rb_close,
  	.getgeo = rb_getgeo,
    .ioctl = rb_ioctl
};


#endif /* end of include guard:  */
