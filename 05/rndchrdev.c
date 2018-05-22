#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
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


int		rnd_char_init(void)
{
	printk(KERN_INFO "Hello, world 1.\n");
	return (0);
}

void	cleanup_module(void)
{
	printk(KERN_INFO "Goodbye, world 1.\n");
}

module_init(rnd_chr_init);
module_exit(cleanup_module);
