#include "myfuncs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vamshidhar Dantu");

struct proc_dir_entry * procEntry;

//extern struct PrivateData myData;
#define init_MUTEX(a) sema_init(a, 1); /* init MUTEX not available!!!?? */

static int assignment5_init(void)
{
  procEntry=create_proc_entry("myPS", 0666, NULL); /* creating a proc entry myPS */
  procEntry->read_proc = MyProcFileRead;
  procEntry->write_proc = MyProcFileWrite;

#if LINUX_VERSION_CODE <= KERNEL_VERSION (2,6,29) /* INTERESTING */
  procEntry->owner = THIS_MODULE;
#endif 
  spin_lock_init(&myData.numOpenLock);
  init_MUTEX(&myData.sem);
  printk(KERN_INFO"UCSC-LDD: A4: myPS created\n");

  return 0;
}

static void assignment5_exit(void)
{
  if(procEntry)
    remove_proc_entry("myPS", NULL);
  printk(KERN_INFO "UCSC-LDD: A4: myPS removed\n");
}

module_init(assignment5_init);
module_exit(assignment5_exit);
