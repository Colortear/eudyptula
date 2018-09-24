#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wiley Debs <wdebs91@gmail.com>");
MODULE_DESCRIPTION("Creates three debugfs files in directory");

struct dentry		*dfs_dir;
struct dentry		*dfs_file[3];
int					filevalue[3];
static const char	*names[3] = {"id", "jiffies", "foo"};
static const int	perms[3] = {0660, 0440, 0200};
static int			foo_write_lock = 0;

static char			*msg_ptr = "wdebs";
static char			foo_buf[PAGE_SIZE+1];

static ssize_t	id_reader(struct file *f, char *buf, size_t len, loff_t *off)
{
	int	bytes_read;

	bytes_read = 0;
	if (*msg_ptr == 0)
		return (0);
	while (len && *msg_ptr) {
		put_user(*(msg_ptr++), buf++);
		len--;
		bytes_read++;
	}
	return (bytes_read);
}

static ssize_t	id_writer(struct file *f,
		const char *buf,
		size_t len,
		loff_t *off)
{
	int	ret_bytes;

	ret_bytes = 0;
	while (len && *buf) {
		if (*buf++ == msg_ptr[ret_bytes]) {
			len--;
			ret_bytes++;
		}
		else {
			return (-EINVAL);
		}
	}
	return (ret_bytes);
}

static ssize_t	jiffies_reader(struct file *f,
		char *buf,
		size_t len,
		loff_t *off)
{
	return (jiffies);
}

static ssize_t	jiffies_writer(struct file *f,
		const char *buf,
		size_t len,
		loff_t *off)
{
	return (0);
}

static ssize_t	foo_reader(struct file *f,
		char __user *buf,
		size_t len,
		loff_t *off)
{
	return (simple_read_from_buffer(buf, len, off, foo_buf, PAGE_SIZE));
}

static ssize_t	foo_writer(struct file *f,
		const char *buf,
		size_t len,
		loff_t *off)
{
	ssize_t	ret;

	ret = -EINVAL;
	if (foo_write_lock || len > PAGE_SIZE)
		return (ret);
	foo_write_lock = 1;
	ret = simple_write_to_buffer(foo_buf, len, off, buf, PAGE_SIZE);
	foo_write_lock = 0;
	return (ret);
}

int		__init init_debug(void)
{
	int	i;
	struct file_operations	fops_dbg[3] = {
		{ .read = id_reader, .write = id_writer },
		{ .read = jiffies_reader, .write = jiffies_writer },
		{ .read = foo_reader, .write = foo_writer }
	};

	printk(KERN_INFO "Hello, world 1.\n");
	for (i = 0; i < PAGE_SIZE+1; i++)
		foo_buf[i] = 0;
	dfs_dir = debugfs_create_dir("fortytwo", NULL);
	for (i = 0; i < 3; i++)
		dfs_file[i] = debugfs_create_file(names[i], perms[i], dfs_dir, &filevalue[3], &fops_dbg[i]);
	return (0);
}

void	__exit debug_cleanup(void)
{
	printk(KERN_INFO "Goodbye, world 1.\n");
	debugfs_remove_recursive(dfs_dir);
}

module_init(init_debug);
module_exit(debug_cleanup);
