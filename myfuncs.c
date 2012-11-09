#include "myfuncs.h"
 
#define minVal(a,b)  ((a<b) ? (a) : (b))

struct PrivateData myData;


int MyProcFileWrite(struct file *file, const char *buf, unsigned long count, void *data) {
  unsigned long length=count;
  struct PrivateData *localData = &myData;
  spin_lock(&localData->numOpenLock);
  (localData->numOpens)++;
  spin_unlock(&localData->numOpenLock);

  length = minVal((unsigned long) MAX_ALLOWABLE_LEN, count);
  if(down_interruptible(&localData->sem))
    return -ERESTARTSYS;
  if(copy_from_user(localData->userGivenPID, buf, length))
    return -EFAULT;
  up(&localData->sem);
  localData->userGivenPID[length-1]='\0';
  if(length >=1)
    localData->fFlag =1;
  printk(KERN_INFO"UCSC-LDD: A4: Requested PID: %s\n",localData->userGivenPID);
  spin_lock(&localData->numOpenLock);
  (localData->numOpens)--;
  spin_unlock(&localData->numOpenLock);
  return length;
}

int MyProcFileRead(char *buf, char **start, off_t offset, int len, int *eof, void *unused) {
  struct PrivateData *localData = &myData;
  int PID = 0, i=0;
  struct task_struct *requestedTask;
  spin_lock(&localData->numOpenLock);
  (localData->numOpens)++;
  spin_unlock(&localData->numOpenLock);
  *eof =1;
  memset(&localData->PSData, 0, PS_DATA);
  if(localData->fFlag !=0) {
    sscanf(localData->userGivenPID, "%d", (int *) &(PID));
  }
  if(PID==0) {
    sprintf(localData->PSData,"Please provide a non-zero PID\n");
  }
  else {
    requestedTask=pid_task(find_vpid(PID), PIDTYPE_PID);
    if(!requestedTask) {
      sprintf(localData->PSData, "No Data for PID %d\n", PID);	
      return -ENODATA;
    }
    else {
      i=sprintf(localData->PSData, "PPID: %d\tPID: %d\tCommand: %s\nNumber of times deviced opened is %d\n", 
		requestedTask->parent->pid, requestedTask->pid, requestedTask->comm, localData->numOpens);	
      localData->PSData[i]='\0';
    }
  }

  printk(KERN_INFO"UCSC-LDD: A4: buf :- %s\n", buf);
  printk(KERN_INFO"UCSC-LDD: A4: PSData :- %s\n", localData->PSData);
  printk(KERN_INFO"UCSC-LDD: Number of time this device opened is %d", localData->numOpens);
  memset(buf, 0, strlen(buf));

  spin_lock(&localData->numOpenLock);
  (localData->numOpens)--;
  spin_unlock(&localData->numOpenLock);
return (sprintf(buf, "%s", localData->PSData));  
}

