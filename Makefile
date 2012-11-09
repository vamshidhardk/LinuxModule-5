obj-m += fifthmodule.o
fifthmodule-objs := myfuncs.o assignment5.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm *~ *.ko