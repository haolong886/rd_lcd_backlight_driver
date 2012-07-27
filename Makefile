KERNELDIR := $(LINUX)
all::
	$(MAKE) -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- M=$(PWD) modules
clean:
	rm *.o *.ko *.mod.c modules.order Module.symvers
