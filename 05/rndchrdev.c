#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("LICENSE");
MODULE_AUTHOR("Wiley Debs <wdebs91@gmail.com>");
MODULE_DESCRIPTION("Creates misc char device with dynamic device number");

/*
 * 1. create random char device and assign dynamic number.
 * 2. when device is read from, return message to caller, when written to
 *    device checks if message is valid and if it is returns valid write
 *    return, if not it returns err.
 * 3. register device when module is loaded and unregister when it
 *    is unloaded.
 */

//static int	g_major;
static int	g_device_open = 0;
static char	msg[80];
static char	*msg_ptr = NULL;

static struct miscdevice	my_dev;

static int	device_open(struct inode *indode, struct file *file)
{
	//static int counter = 0;
	if (g_device_open) {
		return -EBUSY;
	}
	g_device_open++;
//	sprintf(msg, "device opened %i times.", counter++);
	sprintf(msg, "wdebs\n");
	msg_ptr = msg;
	return (0);
}

static int	device_release(struct inode *inode, struct file *file)
{
	g_device_open--;
	return (0);
}

static ssize_t	device_read(struct file *f,
		char *buf,
		size_t length,
		loff_t *offset)
{
	int	bytes_read;

	bytes_read = 0;
	if (*msg_ptr == 0)
		return (0);
	while (length && *msg_ptr) {
		put_user(*(msg_ptr++), buf++);
		length--;
		bytes_read++;
	}
	return (bytes_read);
}

static ssize_t	device_write(struct file *f,
		const char *buf,
		size_t len,
		loff_t *off)
{
	int	is_same;
	int	ret_bytes;

	is_same = 0;
	ret_bytes = 0;
	while (len && *buf) {
		if (*buf++ == msg_ptr[ret_bytes]) {
			len--;
			ret_bytes++;
		}
		else {
			return (EINVAL);
		}
	}
	return (ret_bytes);
}

int		rnd_chr_init(void)
{
	int	retval;
	struct file_operations	my_fops = {
		.read = device_read,
		.write = device_write,
		.open = device_open,
		.release = device_release
	};

	my_dev.minor = MISC_DYNAMIC_MINOR;
	my_dev.name = "fortytwo";
	my_dev.fops = &my_fops;
	retval = misc_register(&my_dev);
	if (retval)
		return (retval);
	printk("fortytwo: got minor %i\n", my_dev.minor);
	return (0);
}

void	module_cleanup(void)
{
	printk(KERN_INFO "closing misc char number %i.\n", my_dev.minor);
	misc_deregister(&my_dev);
}

module_init(rnd_chr_init);
module_exit(module_cleanup);
