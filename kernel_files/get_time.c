//333

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage long sys_get_time(void)
{
	static const long big = 1000000000;
	struct timespec t;
	getnstimeofday(&t);
	return (t.tv_sec * big + t.tv_nsec);
}
