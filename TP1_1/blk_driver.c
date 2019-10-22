#include "blk_driver.h"

int rb_transfert (struct request* rq)
{
	sector_t sector;
	int nb_sectors;
	struct bio_vec bio;
	struct req_iterator rq_it;
	unsigned long offset;
	char * buffer;
    int mode;
    unsigned int nb_sector_used;

	mode = rq_data_dir(rq);  //  if mode = 1 WRITE MODE

	sector = blk_rq_pos(rq);
	nb_sectors = blk_rq_sectors(rq);

	offset = 0;

	rq_for_each_segment(bio, rq, rq_it) {

		// taking the current segment to work on the data
    	buffer = page_address(bio.bv_page) + bio.bv_offset;
    	if ( bio.bv_len % LENGHT_SECTOR != 0)
    		return -EIO;

		// Stock the lenght of the data use
        nb_sector_used = bio.bv_len / LENGHT_SECTOR;

    	if (mode) 	// WRITE MODE
      		memcpy(rb_dev.data + ((sector + offset) * LENGHT_SECTOR), buffer, bio.bv_len);
    	else		// READ MODE
      		memcpy(buffer, rb_dev.data + ((sector + offset) * LENGHT_SECTOR), bio.bv_len);

		// Make sure to deplace the pointer to write in the next blocks
        offset += nb_sector_used;
	 }

     if ( offset != nb_sectors )
        return -EIO;
     else
        return 0;
}

void rb_request (struct request_queue *q)
{
  	struct request * rq;
  	int ret;

  	rq = blk_fetch_request(q);
	// Loop till you dont have request to work on
  	while ( rq != NULL ) {
    		ret = rb_transfert (rq);
    		__blk_end_request_all(rq, ret);
    		rq = blk_fetch_request(q);
  	}
}

int init_module(void)
{
  	rb_dev.size = NB_SECTOR;
  	major = register_blkdev(0, "blk_driver");
  	if ( major < 0 ) {
    		printk(KERN_ALERT "ERROR ASSOCIATION DEVICE MAJOR\n");
    		return 1;
    }

  	spin_lock_init (&rb_dev.lock);

  	rb_dev.rb_queue = blk_init_queue(rb_request,&rb_dev.lock);
  	if ( rb_dev.rb_queue == 0 ) {
    		printk(KERN_ALERT "ERROR REQUEST_QUEUE\n");
    		return 2;
    }

    rb_dev.data = vmalloc (rb_dev.size*LENGHT_SECTOR);
    if (rb_dev.data == NULL) {
    		printk(KERN_ALERT "ERROR VMALLOC\n");
    		return 3;
  	}

  	rb_dev.rb_disk = alloc_disk(NB_DISK);
  	rb_dev.rb_disk->major = major;
  	rb_dev.rb_disk->first_minor = 0;
  	rb_dev.rb_disk->fops = &rb_fops;
  	rb_dev.rb_disk->queue = rb_dev.rb_queue;
    memcpy (rb_dev.rb_disk->disk_name, "blkd", (strlen("blkd")+1)*sizeof(char));

  	set_capacity(rb_dev.rb_disk, rb_dev.size);
  	add_disk(rb_dev.rb_disk);

  	printk(KERN_ALERT "DRIVER ONLINE\n");
  	return 0;
}

void cleanup_module(void)
{
  	vfree (rb_dev.data);
  	del_gendisk(rb_dev.rb_disk);
  	put_disk(rb_dev.rb_disk);
  	blk_cleanup_queue(rb_dev.rb_queue);
  	unregister_blkdev(major, "blk_driver");
  	printk(KERN_ALERT "DRIVER OFFLINE\n");
}

int rb_open (struct block_device * bd, fmode_t fm)
{
	 return 0;
}

void rb_close (struct gendisk * bd, fmode_t fm)
{

}

int rb_getgeo (struct block_device * bd, struct hd_geometry * hdd)
{
    return 0;
}

MODULE_AUTHOR(AUTEUR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(LICENCE);
MODULE_SUPPORTED_DEVICE(DEVICE);
