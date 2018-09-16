#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <string.h>
#include <unistd.h>

MODULE_LICENSE("LICENSE");
MODULE_AUTHOR("Wiley Debs <wdebs91@gmail.com>");
MODULE_DESCRIPTION("Creates three debugfs files in directory");

struct dentry	*dfs_dir;
struct dentry	*dfs_file[3];
int				filevalue;
static const struct file_operations	fops_dbg[3];
static const char	*names[3] = {"id", "jiffies", "foo"};
static const int	perms[3] = {0660, 0440, 0200};

static struct 

int		init_module(void)
{
	printk(KERN_INFO "Hello, world 1.\n");
	dfs_dir = debugfs_create_dir("fortytwo", NULL);
	chown("/sys/kernel/debug/fortytwo", -1, -1);
	fops_dbg[] = {
		[0].read = id_reader, [0].write = id_writer,
		[1].read = jiffies_reader, [1].write = jiffies_writer,
		[2].read = foo_reader, [2].write = foo_writer
	}
	for (int i = 0; i < 3; i++)
		dfs_file[i] = debugfs_create_file(names[i], perms[i],
				dfs_dir, &fops_dbg[i]);
	return (0);
}

void	module_cleanup(void)
{
	printk(KERN_INFO "Goodbye, world 1.\n");
}

module_init(init_module);
module_exit(module_cleanup);
