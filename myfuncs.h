#include <asm/io.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36)
#include <generated/utsrelease.h>
#else
#include <linux/utsrelease.h>
#endif

#define MAX_ALLOWABLE_LEN 8
#define PS_DATA 4095

struct PrivateData {
  char PSData[PS_DATA];
  char userGivenPID[MAX_ALLOWABLE_LEN+1];
  int fFlag;
  int numOpens;			/* number of device opens */
  struct semaphore sem;		/* mutex for read/write */
  spinlock_t numOpenLock;	/* spinlock for updation of numOpens */
};

extern struct PrivateData myData;

int MyProcFileWrite(struct file *file, const char *buf, unsigned long count, void *data);
int MyProcFileRead(char *buf, char **start, off_t offset, int len, int *eof, void *unused);

