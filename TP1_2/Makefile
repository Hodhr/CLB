ifneq ($(KERNELRELEASE),)
	obj-m := blk_driver.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C ${KERNEL_DIR} M=$(PWD) modules
	gcc ioctl.c -o ioctl
clean:
	$(MAKE) -C ${KERNEL_DIR} M=$(PWD) clean
endif
