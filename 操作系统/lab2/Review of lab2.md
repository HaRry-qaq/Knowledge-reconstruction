# Review of lab2

### step 1

略。





### step 2

##### 代码主要组成如下所述：

1. 在内核中启动时分配对象，此后不能再分配。
2. Runtime.s 包含用汇编语言编写的例程，包含程序入口与中断向量
3. 例程执行时调用main.c的main函数
4. List实现了操作系统的链表
5. List主要的键类名为Thread.它是线程的核心，包含线程调度（状态转换）和时间切片相关函数
         Thread中的FatalError函数就是进程终止函数，打印可能的错误。随后进入blitz的命令行模式，可能需要输入st查看函数和方法处于活动状态。
         Thread中的SetInterruptsTo用于改变CPU的中断标志（开中断或关中断），返回一个变量提示现在是什么状态。
6. 线程五种状态：JUST_CREATED、READY、RUNNING、BLOCKED和UNUSED
7. 每个线程都有堆栈（系统堆栈），堆栈放置在systemStack字段中的thread对象中。该堆栈只用于内核例程，而之后的实验中用户程序在各自的虚拟空间中有属于自己的堆栈。
8. Thread对象还存储CPU的状态，在线程切换时所有数据保存在thread对象，这些字段（regs和stackTop）由名为Switch的代码例程使用。（PCB？）
9. Thread对象还存储指向函数的指针(initialFunction)和函数的实参(initialArgument字段)，该指针指向现成的main函数，不同的线程执行不同的main函数。
10. initialArgument字段表示线程的编号。
11. 使用Thread对象中的Fork方法启动一个初始化的新线程，这会使他进入ready状态（加入readyList，这是一个全局变量）。
12. currentThread：全局变量，表示状态为RUNNING的唯一一个线程。
13. Thread对象中的Yield方法：只能在当前运行的线程上调用，作用为切换到其他线程。
         ① 禁用中断：防止干扰。
         ② 在readylist找下一个线程（若没有其他线程，则yield实际上是nop）
         ③ 使当前线程变为READY
         ④ 将当前线程添加到readyList尾部
         ⑤ 运行下一个线程(Run方法)
14. Thread对象中的Run方法：先检查线程堆栈溢出，再调用Switch方法执行线程切换。
15. Thread对象中的Switch方法：返回发生在另一个进程的另一个函数。
16. 其他类与方法：下面会提及。





### step 3

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024160831776.png" alt="image-20221024160831776" style="zoom:67%;" />

#### 解释：

当进程中的线程所使用的时间超过了时间片所定义的宽度，则会发生时间中断，时间中断的处理发生在TimerInterruptHandler方法中。取消注释，输出"_"是为了更加清晰地看出线程超时的发生。

仔细观察代码，执行输出"Simple Thread Example..."时进程已经开启，已知blitz操作系统中进程开启后会有一个基本线程："main-thread"（他并未在main.c定义却出现了），时间片已经开始计时，所以线程还没有运行就出现了"_"。 随后观察代码：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024163730596.png" alt="image-20221024163730596" style="zoom:67%;" />

若没有时间中断，将会交替出现"main-thread" 和 "second-thread".但存在时间中断，所以在中断后根据TimerInterruptHandler，会切换到下一个线程开始执行：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024163940535.png" alt="image-20221024163940535" style="zoom:67%;" />

这也是为什么会出现下列情况的原因：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024164104122.png" alt="image-20221024164104122" style="zoom:67%;" />







### step 4

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024164206782.png" alt="image-20221024164206782" style="zoom:67%;" />

#### 解释：

与step3类似，但出现了没有缺数字的情况。原因是在print执行完毕后才发生时间中断。

在for循环稳定后，就出现了上图中每一个循环超时的线程往前挪一个的情况。这也间接说明了时间片每一次计时是等长的。



### step 5

结果：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024164800691.png" alt="image-20221024164800691" style="zoom:67%;" />

但我们并不关心main.c的测试函数。我们重点考虑Synch.c中的Mutex类的实现：



<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024165337769.png" alt="image-20221024165337769" style="zoom: 67%;" />

heldBy：当前正在使用资源的线程。

waitingTheards：等待使用资源的线程的队列。



<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024165404885.png" alt="image-20221024165404885" style="zoom:67%;" />

如果heldBy为空则说明没有线程正在占用资源，则一定也没有线程在等待。所以直接将当前进程变为正在使用资源的运行态；

若heldBy为当前线程，抛出错误；

若当前已经有线程占用资源，则当前线程需要进入等待队列，并进入睡眠。

由于如果中断会出现很大的问题，所以不允许中断(原语)，事先关中断(110行)，执行完后再开中断(121行)。



<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024170035435.png" alt="image-20221024170035435" style="zoom:67%;" />

若当前没有锁，抛出错误；

从等待队列中移出最先进入的线程(FIFO)，若非空则将其放入READY状态。若队列中已经没有等待的线程，则将heldBy赋值为null。







<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024235834452.png" alt="image-20221024235834452" style="zoom:67%;" />



也可以不加锁：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221025000318651.png" alt="image-20221025000318651" style="zoom:67%;" />

解释略。





### step 6

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024171517823.png" alt="image-20221024171517823" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024171551668.png" alt="image-20221024171551668" style="zoom:67%;" />

注：顺序无关紧要，只需要每个producer都输出5次即正确。

**知识点：信号量与前后（同步）关系、互斥关系**

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024171805942.png" alt="image-20221024171805942" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024171903413.png" alt="image-20221024171903413" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024171924654.png" alt="image-20221024171924654" style="zoom:67%;" />

也就是所谓的PV关系。P：减少量  V：增加量。

**注意：**

① mutex的PV在同一个函数内是成对出现的(mutex.down()、mutex.up())

**② 必须先执行完P的同步关系再执行P的互斥关系，不然会造成死锁。**

③ 实际上这里的mutex也可以用step5实现的互斥锁mutex类实现，只是信号量类为二值时等价为mutex类而已。这也是为什么说mutex类可以仿照semaphore类写的原因。



### step 7

先介绍非管程的考虑方式与解决方案：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024173412382.png" alt="image-20221024173412382" style="zoom: 50%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024173430717.png" alt="image-20221024173430717" style="zoom:50%;" />



但是step7所采用的是管程解决方案。

**假设两根筷子都可用时才能拿起筷子：**

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024175534525.png" alt="image-20221024175534525" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221024175603612.png" alt="image-20221024175603612" style="zoom:67%;" />

结果：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221025000525380.png" alt="image-20221025000525380" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221025000534041.png" alt="image-20221025000534041" style="zoom:67%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221025000540636.png" alt="image-20221025000540636" style="zoom:67%;" />

