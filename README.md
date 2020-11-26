# LDD-200250330034

*Basic steps
	*sudo -s //for entering super user (root file)
	*create file
	*create Makefile
	*make //for compiling code
	*insmod filename.ko //for insert the module
	*lsmod //to check is module inserted or not
	*dmesg //to see the msg or output.(dmesg -C //used for clear dmesg buffer.
	*rmmod filename //for remove the module.
	
	
*HelloWord
	*init module into kernel
	*exit module from kernel

*ModuleParameters
	*How to deal with variables
	*How to pass Parameters in command line.
		ex: insmod filename.ko a=20 b=30
		
*ExportSymbol
	*ExportSym_simple
		*just inserting manually (but must follow the flow)
      		 ex:if 2 files average.c and add.c and average.c depends on add.c then first inser insmod average.ko then add.c 
	
  	*ExportSym_Order
  		*here we use modprobe command it insert automatically in order
  		   it stores in extra folder (/lib/modules/$(shell uname -r)/build/extra);

*CharDevice 
	*Among 3 types of device, Chareter device is one whitch is acess like stream of bytes.
		*in kernel side first intiatlize char driver with major and minor numbers
		*then add char device
		*by using file_operation structure it perform particular operation
		       open ==> int my_open(struct inode *,struct file *);
		       read ==> ssize_t my_read(struct file *,char __user *,size_t ,loff_t *);
		       write ==> ssize_t my_write(struct file *,char __user *,size_t ,loff_t *);
		       release ==> int my_close(struct inode *,struct fil *);
		       
	*we have to save the device name by mknod command
		*mknod /dev/devicename c ma mi --mode==666
	
*CircularBuff
	*for efficient use of memory we use circular buffer
	*struct circ_buf cbuf; //declaring circular buffer structure in global space
	*cbuf.buf=kmalloc(SIZE,GFP_KERNEL)//dynamic memory allocation for circular buffer
	*kfree(cbuf.buf);
	
*waitQueue
	*wait queues are used when you use read two times continously without writind it waits for second read until writes.
	*vice versa.
		*static wait_queue_head_t _wq //declaring in globel space
		*init_waitqueue_head (&_wq); //intalization of wait queue
		*wait_event_interruptible(_wq,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)<-->condition);
			* >0  it is for read
			* <(int)cnt  it is for write
			
			
*multiDevices
	*it is for intializing mutipul devices
		*ex: 42 0  dev1       42 1 dev2      42 2 dev3 ....etc
		
	
*ioctl
	*it is for configaration in user application.it is one of the file_operation variable
		*.unlocked_ioctl
		   ==> long my_ioctl(struct file *,unsigned int ,unsigned long);
		* in user application config like  ==> ioctl(fd,CMD_1);
		
		
*semaphore

	*it is used for process sheduling
	*if you use this it aloows only one process into CS.
		*struct semaphore _sem; //declaring semaphore struture in goble space
		*sema_init(device structure,semaphore vaule);
		*down_interruptible(semaphore structure); //it make samaphore = 0.
		*up(semaphore structure); //it make semaphore = 1.
		
		
