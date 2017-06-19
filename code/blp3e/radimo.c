/*
 * Sample RAm DIsk MOdule, Radimo
 *
 */

#include <linux/module.h>

#if defined(CONFIG_SMP)
#define __SMP__
#endif

#if defined(CONFIG_MODVERSIONS)
#define MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

#include <asm/uaccess.h>

#include "radimo.h"

#define MAJOR_NR        RADIMO_MAJOR
#define DEVICE_NAME     "radimo"
#define DEVICE_REQUEST      radimo_request
#define DEVICE_NR(device)   (MINOR(device))
#define DEVICE_ON(device)
#define DEVICE_OFF(device)
#define DEVICE_NO_RANDOM

#include <linux/blk.h>

#define RADIMO_HARDS_BITS   9   /* 2**9 byte hardware sector */
#define RADIMO_BLOCK_SIZE   1024    /* block size */
#define RADIMO_TOTAL_SIZE   2048    /* size in blocks */

/* the storage pool */
static char* radimo_storage;

static int radimo_hard = 1 << RADIMO_HARDS_BITS;
static int radimo_soft = RADIMO_BLOCK_SIZE;
static int radimo_size = RADIMO_TOTAL_SIZE;

static int radimo_readahead = 4;

/* for media changes */
static int radimo_changed;
struct timer_list radimo_timer;

/* module parameters and descriptions */
MODULE_PARM(radimo_soft, "1i");
MODULE_PARM_DESC(radimo_soft, "Software block size");
MODULE_PARM(radimo_size, "1i");
MODULE_PARM_DESC(radimo_size, "Total size in KB");
MODULE_PARM(radimo_readahead, "1i");
MODULE_PARM_DESC(radimo_readahead, "Max number of sectors to read ahead");

MODULE_LICENSE("GPL");


/* forward declarations for _fops */
static int radimo_open(struct inode* inode, struct file* file);
static int radimo_release(struct inode* inode, struct file* file);
static int radimo_ioctl(struct inode* inode, struct file* file,
                        unsigned int cmd, unsigned long arg);
static int radimo_media_change(kdev_t dev);
static int radimo_revalidate(kdev_t dev);


/* Because the kernel interface to device drivers changes a bit
   from version to version, we have to test kernel versions here.
   If you need your module to compile on older kernels, you can usei
   this as an example for your device drivers.  It should
   work on kernels from 2.3 to 2.6.0
*/

#if LINUX_VERSION_CODE < 0x20326
static struct file_operations radimo_fops = {
read:
    block_read,
write:
    block_write,
ioctl:
    radimo_ioctl,
open:
    radimo_open,
release:
    radimo_release,
check_media_change:
    radimo_media_change,
revalidate:
    radimo_revalidate,
};
#else
static struct block_device_operations radimo_fops = {
open:
    radimo_open,
release:
    radimo_release,
ioctl:
    radimo_ioctl,
check_media_change:
    radimo_media_change,
revalidate:
    radimo_revalidate,
#if LINUX_VERSION_CODE >= 0x20414
owner:
    THIS_MODULE,
#endif
};
#endif


static void radimo_request(request_queue_t* q) {
    unsigned long offset, total;
radimo_begin:
    INIT_REQUEST;
    MSG(RADIMO_REQUEST, "%s sector %lu of %lu\n",
        CURRENT->cmd == READ ? "read" : "write",
        CURRENT->sector,
        CURRENT->current_nr_sectors);
    offset = CURRENT->sector * radimo_hard;
    total = CURRENT->current_nr_sectors * radimo_hard;

    /* access beyond end of the device */
    if (total + offset > radimo_size * (radimo_hard << 1)) {
        /* error in request  */
        end_request(0);
        goto radimo_begin;
    }

    MSG(RADIMO_REQUEST, "offset = %lu, total = %lu\n", offset, total);

    if (CURRENT->cmd == READ) {
        memcpy(CURRENT->buffer, radimo_storage + offset, total);
    } else if (CURRENT->cmd == WRITE) {
        memcpy(radimo_storage + offset, CURRENT->buffer, total);
    } else {
        /* can't happen */
        MSG(RADIMO_ERROR, "cmd == %d is invalid\n", CURRENT->cmd);
        end_request(0);
    }

    /* successful */
    end_request(1);
    /* let INIT_REQUEST return when we are done */
    goto radimo_begin;
}

static int radimo_media_change(kdev_t dev) {
    if (radimo_changed) {
        MSG(RADIMO_INFO, "media has changed\n");
    }

    /* 0 means medium has not changed, while 1 indicates a change */
    return radimo_changed;
}

static int radimo_revalidate(kdev_t dev) {
    MSG(RADIMO_INFO, "revalidate\n");
    /* just return 0, check_disk_change ignores it anyway */
    return 0;
}

void radimo_timer_fn(unsigned long data) {
    MSG(RADIMO_TIMER, "timer expired\n");

    /* only "change media" if device is unused */
    if (MOD_IN_USE) {
        radimo_changed = 0;
    } else {
        /* medium changed, clear storage and */
        MSG(RADIMO_TIMER, "simulating media change\n");
        /* By erasing the first four blocks! */
        memset(radimo_storage, 0, RADIMO_BLOCK_SIZE * 4);
        radimo_changed = 1;
        /* data contains i_rdev */
        fsync_dev(data);
        invalidate_buffers(data);
    }

    /* set it up again */
    radimo_timer.expires = RADIMO_TIMER_DELAY + jiffies;
    add_timer(&radimo_timer);
}

static int radimo_release(struct inode* inode, struct file* file) {
    MSG(RADIMO_OPEN, "closed\n");
    MOD_DEC_USE_COUNT;
    return 0;
}

static int radimo_open(struct inode* inode, struct file* file) {
    MSG(RADIMO_OPEN, "opened\n");
    MOD_INC_USE_COUNT;
    /* timer function needs device to invalidate buffers. pass it as
       data. */
    radimo_timer.data = inode->i_rdev;
    radimo_timer.expires = RADIMO_TIMER_DELAY + jiffies;
    radimo_timer.function = &radimo_timer_fn;

    if (!timer_pending(&radimo_timer)) {
        add_timer(&radimo_timer);
    }

    return 0;
}

static int radimo_ioctl(struct inode* inode, struct file* file,
                        unsigned int cmd, unsigned long arg) {
    unsigned int minor;

    if (!inode || !inode->i_rdev) {
        return -EINVAL;
    }

    minor = MINOR(inode->i_rdev);

    switch (cmd) {
    case BLKFLSBUF: {
            /* flush buffers */
            MSG(RADIMO_IOCTL, "ioctl: BLKFLSBUF\n");

            /* deny all but root */
            if (!capable(CAP_SYS_ADMIN)) {
                return -EACCES;
            }

            fsync_dev(inode->i_rdev);
            invalidate_buffers(inode->i_rdev);
            break;
        }

    case BLKGETSIZE: {
            /* return device size */
            MSG(RADIMO_IOCTL, "ioctl: BLKGETSIZE\n");

            if (!arg) {
                return -EINVAL;
            }

            return put_user(radimo_size * 2, (long*) arg);
        }

    case BLKRASET: {
            /* set read ahead value */
            int tmp;
            MSG(RADIMO_IOCTL, "ioctl: BLKRASET\n");

            if (get_user(tmp, (long*)arg)) {
                return -EINVAL;
            }

            if (tmp > 0xff) {
                return -EINVAL;
            }

            read_ahead[RADIMO_MAJOR] = tmp;
            return 0;
        }

    case BLKRAGET: {
            /* return read ahead value */
            MSG(RADIMO_IOCTL, "ioctl: BLKRAGET\n");

            if (!arg) {
                return -EINVAL;
            }

            return put_user(read_ahead[RADIMO_MAJOR], (long*)arg);
        }

    case BLKSSZGET: {
            /* return block size */
            MSG(RADIMO_IOCTL, "ioctl: BLKSSZGET\n");

            if (!arg) {
                return -EINVAL;
            }

            return put_user(radimo_soft, (long*)arg);
        }

    default: {
            MSG(RADIMO_ERROR, "ioctl wanted %u\n", cmd);
            return -ENOTTY;
        }
    }

    return 0;
}

static int __init radimo_init(void) {
    int res;

    /* block size must be a multiple of sector size */
    if (radimo_soft & ((1 << RADIMO_HARDS_BITS) - 1)) {
        MSG(RADIMO_ERROR, "Block size not a multiple of sector size\n");
        return -EINVAL;
    }

    /* allocate room for data */
    radimo_storage = (char*) vmalloc(1024 * radimo_size);

    if (radimo_storage == NULL) {
        MSG(RADIMO_ERROR, "Not enough memory. Try a smaller size.\n");
        return -ENOMEM;
    }

    memset(radimo_storage, 0, 1024 * radimo_size);
    /* register block device */
    res = register_blkdev(RADIMO_MAJOR, "radimo", &radimo_fops);

    if (res) {
        MSG(RADIMO_ERROR, "couldn't register block device\n");
        return res;
    }

    /* for media change */
    radimo_changed = 0;
    init_timer(&radimo_timer);
    /* set hard- and soft blocksize */
    hardsect_size[RADIMO_MAJOR] = &radimo_hard;
    blksize_size[RADIMO_MAJOR] = &radimo_soft;
    blk_size[RADIMO_MAJOR] = &radimo_size;
    /* define our request function */
    /* Here's another instace where kernel versions really matter.
       The request queue interface changed in the 2.4 series kernels
    */
#if LINUX_VERSION_CODE < 0x20320
    blk_dev[RADIMO_MAJOR].request_fn = &radimo_request;
#else
    blk_init_queue(BLK_DEFAULT_QUEUE(RADIMO_MAJOR), radimo_request);
#endif
    read_ahead[RADIMO_MAJOR] = radimo_readahead;
    MSG(RADIMO_INFO, "loaded\n");
    MSG(RADIMO_INFO, "sector size of %d, block size of %d, total size = %dKb\n",
        radimo_hard, radimo_soft, radimo_size);
    return 0;
}

static void __exit radimo_cleanup(void) {
    unregister_blkdev(RADIMO_MAJOR, "radimo");
    del_timer(&radimo_timer);
    invalidate_buffers(MKDEV(RADIMO_MAJOR, 0));
    /* remove our request function */
#if LINUX_VERSION_CODE < 0x20320
    blk_dev[RADIMO_MAJOR].request_fn = 0;
#else
    blk_cleanup_queue(BLK_DEFAULT_QUEUE(RADIMO_MAJOR));
#endif
    vfree(radimo_storage);
    MSG(RADIMO_INFO, "unloaded\n");
}



module_init(radimo_init);
module_exit(radimo_cleanup);

