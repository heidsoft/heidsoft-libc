#如果已经定义KERNELRELEASE,则说明从内核构建系统调用
#因此可利用其内建语句。
ifneq ($(KERNELRELEASE),)
	obj-m :=hello.o

#否则，是直接从命令行调用的。
#这时要调用内核构造系统

else
	obj-m :=hello.o
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)	

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)	modules
#LINUX_KERNEL:=$(shell uname -r)
#LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)
#all:
#make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
#clean:
#make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean

endif
