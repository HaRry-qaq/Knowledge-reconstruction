## lab5 test

陈俊卉   学号：2020212256   班级：2020219111

#### Task 1

- 首先make

- 然后将文件引入blitz的模拟disk
- 最后运行

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204172650788.png" alt="image-20221204172650788" style="zoom: 80%;" />



#### Task2、3

- 首先make

- 然后将文件引入blitz的模拟disk
- 最后运行

![image-20221204173115352](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204173115352.png)

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204173142738.png" alt="image-20221204173142738" style="zoom:80%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204173225691.png" alt="image-20221204173225691" style="zoom:80%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204173259257.png" alt="image-20221204173259257" style="zoom:80%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221204173327893.png" alt="image-20221204173327893" style="zoom:80%;" />



**易看版：**

```c
harryovo@harryovo-virtual-machine:~/Desktop/lab5$ make
kpl Kernel -unsafe
asm Kernel.s
lddd Runtime.o Switch.o System.o List.o BitMap.o Kernel.o Main.o -o os
harryovo@harryovo-virtual-machine:~/Desktop/lab5$ diskUtil -i
harryovo@harryovo-virtual-machine:~/Desktop/lab5$ diskUtil -a TestProgram1 TestProgram1
harryovo@harryovo-virtual-machine:~/Desktop/lab5$ diskUtil -a TestProgram2 TestProgram2
harryovo@harryovo-virtual-machine:~/Desktop/lab5$ blitz -g os
Beginning execution...
====================  KPL PROGRAM STARTING  ====================
Initializing Thread Scheduler...
Initializing Thread Manager...
Initializing Frame Manager...
AllocateRandomFrames called.  NUMBER_OF_PHYSICAL_PAGE_FRAMES = 100
Initializing Disk Driver...
Initializing File Manager...
User-level program 'TestProgram1' is running...

***** Testing Syscall Parameter Passing *****

***** About to call Sys_Yield...
***** Should print:
*****     Handle_Sys_Yield invoked!

Handle_Sys_Yield invoked!

***** About to call Sys_Fork...
***** Should print:
*****     Handle_Sys_Fork invoked!

Handle_Sys_Fork invoked!

***** About to call Sys_Join...
***** Should print:
*****     Handle_Sys_Join invoked!
*****     processID = 1111

Handle_Sys_Join invoked!
processID = 1111

***** About to call Sys_Create...
***** Should print:
*****     Handle_Sys_Create invoked!
*****     virt addr of filename = 0x0000BFF8
*****     filename = MyFileName

Handle_Sys_Create invoked!
virt addr of filename  = 0x0000BFF8
filename = MyFileName

***** About to call Sys_Open...
***** Should print:
*****     Handle_Sys_Open invoked!
*****     virt addr of filename = 0x0000BFF8
*****     filename = MyFileName

Handle_Sys_Open invoked!
virt addr of filename  = 0x0000BFF8
filename = MyFileName

***** About to call Sys_Read...
***** Should print:
*****     Handle_Sys_Read invoked!
*****     fileDesc = 2222
*****     virt addr of buffer = 0x0000B0B0
*****     sizeInBytes = 3333

Handle_Sys_Read invoked!
fileDesc = 2222
virt addr of buffer  = 0x0000B0B0
sizeInBytes = 3333

***** About to call Sys_Write...
***** Should print:
*****     Handle_Sys_Write invoked!
*****     fileDesc = 4444
*****     virt addr of buffer = 0x0000B0B0
*****     sizeInBytes = 5555

Handle_Sys_Write invoked!
fileDesc = 4444
virt addr of buffer  = 0x0000B0B0
sizeInBytes = 5555

***** About to call Sys_Seek...
***** Should print:
*****     Handle_Sys_Seek invoked!
*****     fileDesc = 6666
*****     newCurrentPos = 7777

Handle_Sys_Seek invoked!
fileDesc = 6666
newCurrentPos =7777

***** About to call Sys_Close...
***** Should print:
*****     Handle_Sys_Close invoked!
*****     fileDesc = 8888

Handle_Sys_Close invoked!
FileDesc = 8888

***** About to call Sys_Exit...
***** Should print:
*****     Handle_Sys_Exit invoked!
*****     returnStatus = 9999

Handle_Sys_Exit invoked! 
returnStatus = 9999

***** Syscall Test Complete *****

***** Testing Exec Syscall *****

***** About to call Sys_Exec with a non-existant file...
***** Should print:
*****     Okay

Okay

***** About to call Sys_Exec with an overly long file name...
***** Should print:
*****     Okay

Okay

***** About to perform a successful Exec and jump to TestProgram2...
***** Should print:
*****     User-level program 'TestProgram2' is running!

User-level program 'TestProgram2' is running!

***** About to call Sys_Shutdown...
***** Should print:
*****     FATAL ERROR in UserProgram: "Syscall 'Shutdown' was invoked by a user thread" -- TERMINATING!

FATAL ERROR in UserProgram: "Syscall 'Shutdown' was invoked by a user thread" -- TERMINATING!

(To find out where execution was when the problem arose, type 'st' at the emulator prompt.)

====================  KPL PROGRAM TERMINATION  ====================

```

