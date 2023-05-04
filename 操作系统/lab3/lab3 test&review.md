## lab3 test&review

陈俊卉   学号：2020212256   班级：2020219111

### Task 1  Sleeping Barber Problem

##### 1、代码说明

###### ① 类的定义

```c
  class barberCustomerMonitor
   superclass Object
   fields
     customers: Semaphore
     barbers: Semaphore
     mutex: Semaphore
     waiting: int
     
     --need a queue to sort the customer who is waiting
     q: array [CHAIRS] of int
     --front of the queue
     qf: int
     -- rear of the queue
     qr: int
     
   methods
     Init()
     barber_behave(id: int)
     customer_behave(id: int)
     cut_hair(id: int)
     get_haircut(id: int)
   endClass
```

- customers、barbers、mutex：信号量
- waiting：表示正坐在椅子上等待理发的顾客
- q：在barber_behave()函数中，barber理发时需要**按顺序**将正在理发的顾客的id输出。所以需要一个队列。
- qf：队列的头；qr：队列的尾
- 函数作用在下文讲述。



###### ② 各个函数的作用

- Init() 略

- barber_behave(id: int)

  - ```c
         method barber_behave(id: int)
           while true
             customers.Down()
             mutex.Down()
             waiting = waiting - 1
             barbers.Up()
             mutex.Up()
             
             self.cut_hair(id)
             
           endWhile
         endMethod
    ```

  - 首先进行P关系的变化customers.Down()【此时若没有顾客，barber将会sleep，直到有第一个顾客来到并执行customers.Up()后才会唤醒barber】，然后上锁，对waiting进行-1操作表示当前顾客已经理发完毕；随后barbers.Up()，解锁。此时上一个顾客已经完毕，即将被下一个顾客的线程进行barbers.Down()以开始理发，所以此时我们应该在cut_hair()输出即将理发的顾客的信息。

- cut_hair(id: int)

  - ```c
       method cut_hair(id: int)
    	print("The barber is cutting hair for customer ")
    	printInt(q[qf])
    	print("\n")
    	
    	qf = (qf+1) % CHAIRS
    
      endMethod
    ```

  - cut_hair()函数主要进行输出下一个顾客的理发信息，并更新队列。

- customer_behave(id: int)

  - ```c
         method customer_behave(id: int)
         
           mutex.Down()
           
           if waiting < CHAIRS
             print("customer ")
             printInt (id)
             print (" is waiting, ")
             
             waiting = waiting + 1
             q[qr] = id
             qr = (qr+1) % CHAIRS
             
             
             printInt(5 - waiting)
             print(" chair(s) left \n")
             
             customers.Up()
             mutex.Up()
             barbers.Down()
             
             -- cutting time, not necessary to use
             --self.get_haircut(id)
           
           else
             print("The customer ")
             printInt(id)
             print(" is leaving since no chair to sit\n")
             mutex.Up()
           
           endIf
         endMethod
    ```

  - 首先需要上锁。随后判断是否有空闲的位置，如果有则输出入座等待的信息并更新waiting和队列。随后输出还有多少椅子空闲。因为**我们需要在确定顾客是否入座，之后才能执行barbers.Down()**，所以需要在customers.Up()和mutex.Up()后才执行barbers.Down()。而如果此时barber如果忙碌，该线程将会sleep，直到信号量barbers内的队列中轮到该线程了，才会被唤醒并开始理发。开始理发后执行get_haircut()函数，所以该函数需要体现理发时间。

- get_haircut(id:int)

  - ```
         method get_haircut(id:int)
         var
           i:int
           -- during cutting
           mutex.Down()
             for i = 0 to 5
               currentThread.Yield()
             endFor
           mutex.Up()
           
           
         endMethod
    ```

  - 为了保证在理发过程中其他线程不会被唤醒，所以我们需要上锁。由于理发时，当前处于ready态的顾客线程只有这一个，其他顾客都在睡眠，处于barbers.Down()的等待队列中，所以currentThread.Yield()的作用仅仅只是消耗时间。



##### 2、输出

**没有设置cutting_time，即没有执行get_haircut(id)的情况：**

![image-20221114220027992](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114220027992.png)

![image-20221114220216918](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114220216918.png)





**有理发时间，即执行get_haircut(id)的情况：**

![image-20221114220430253](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114220430253.png)

![image-20221114220504932](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114220504932.png)



显然，有理发时间的情况比没有理发时间的情况，成功理发的顾客要多。符合预期。





### Task 2 The Gaming Parlor Problem

##### 1、代码说明

###### ① 类的定义

```c
   class gamingParlorMonitor
     superclass Object
     fields
     
       -- dices left
       dice_avail: int
       mutex: Mutex
       
       -- to ensure that have only 1 thread in while_
       wait_: Condition
       wait_count: int
       
       -- whether the dices are enough
       dice_not_enough: Condition 
       
       

     methods
       Init()
       Request(number_of_dice: int)
       Return(number_of_dice: int)
       Print(str: String, count: int)
     endClass
```

- dice_avail：剩余的骰子
- mutex：锁
- wait_：用来**保证按照顺序给出骰子，不会进入饥饿状态的条件变量**：保证每一次只会有一个线程进入询问骰子的while内。下文会详细讲述。
- wait_count：用来记录当前是否有多于1个线程在等待取骰子。
- dice_not_enough：条件变量，用来标志当前骰子不足。



###### ② 各个函数的作用

- Init() 略

- Request(number_of_dice: int)

  - ```c
         method Request(number_of_dice: int)
           mutex.Lock()
           self.Print ("requests", number_of_dice)
           
           
           
           wait_count = wait_count + 1
           if wait_count > 1
             wait_.Wait(&mutex)
           endIf
           
           
           while dice_avail < number_of_dice
             -- dice is not enough
             dice_not_enough.Wait(&mutex)
           endWhile
           
           -- dice is enough now
           dice_avail = dice_avail - number_of_dice
           
           wait_count = wait_count - 1
           wait_.Signal(&mutex)
           
           self.Print("proceeded with", number_of_dice)
           mutex.Unlock()
           
         endMethod
         
         
         method Return(number_of_dice: int)
           mutex.Lock()
           dice_avail = dice_avail + number_of_dice
           self.Print("returned", number_of_dice)
           
           -- ensure signal when enough and the queue order unchanged
           dice_not_enough.Signal(&mutex)    
           mutex.Unlock()
           
         endMethod
    ```

  - 先上锁，输出必要语句。更新wait_count的值。如果目前已经有大于1的线程在索要骰子，我们将当前线程放入条件变量wait_的wait队列中。【**它将在前一个索取线程成功索取后被signal，以确保永远只有一个索取线程在索要骰子，并确保它们都是按照队列顺序一个一个索取的，以避免发生下面的状况：有两个等待索取的线程，前面的需要5个，后面的需要3个，而现在有2个骰子。更前的一个线程归还了2个骰子后，唤醒需要5个骰子的线程，骰子仍然不足，这时候就会再次sleep，但它被加入到了队列的最后面，从而可能导致它会长时间无法获得骰子，这不是我们想见到的**】当该索取线程被signal后，它进入while判断当前有的骰子是否足够。若不够，则进入dice_not_enough的等待队列。而在这一个索取线程成功索取之前，不会再有另外一个在后面的索取线程进入到条件变量dice_not_enough的等待队列中，而是都在wait\_的等待队列中。当足够后，进行索取，wait_count -1，为下一个索取线程signal。最后解锁。

- Return(number_of_dice: int)

  - ```c
         method Return(number_of_dice: int)
           mutex.Lock()
           dice_avail = dice_avail + number_of_dice
           self.Print("returned", number_of_dice)
           
           -- ensure signal when enough and the queue order unchanged
           dice_not_enough.Signal(&mutex)    
           mutex.Unlock()
           
         endMethod
    ```

  - 先上锁。还骰子，对条件变量dice_not_enough进行signal，最后解锁。

- 测试函数

  - ```c
    var
      gaming_monitor: gamingParlorMonitor = new gamingParlorMonitor
      playerArray: array[8] of Thread = new array of Thread {8 of new Thread}
    
       function gamingParlorProblem()
         gaming_monitor.Init()
         
         playerArray[0].Init("A")
         playerArray[0].Fork(game, 4)
         playerArray[1].Init("B")
         playerArray[1].Fork(game, 4)
         playerArray[2].Init("C")
         playerArray[2].Fork(game, 5)
         playerArray[3].Init("D")
         playerArray[3].Fork(game, 5)
         playerArray[4].Init("E")
         playerArray[4].Fork(game, 2)
         playerArray[5].Init("F")
         playerArray[5].Fork(game, 2)
         playerArray[6].Init("G")
         playerArray[6].Fork(game, 1)
         playerArray[7].Init("H")
         playerArray[7].Fork(game, 1)
         
       endFunction
       
       function game(dice_need: int)
       var
         i: int
         j: int
         for i = 1 to 5
           gaming_monitor.Request(dice_need)
           
           -- play
           for j = 1 to 50
             currentThread.Yield()
           endFor
           
           gaming_monitor.Return(dice_need)
           
           -- to avoid a group play continuously
           currentThread.Yield()
         endFor
         
       endFunction
    ```

  - 要点在game()：每个group进行5次索取。在每次索取后，使用数次（这里50次）currentThread.Yield()表示**play持续时间**。最后归还后，为了避免一个组连续地归还后直接索取，在归还后增加了一个currentThread.Yield()。



##### 2、测试结果

###### ① 没有**play持续时间**

![image-20221114224234989](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224234989.png)

![image-20221114224302757](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224302757.png)

![image-20221114224328925](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224328925.png)

![image-20221114224351077](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224351077.png)

![image-20221114224411397](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224411397.png)

![image-20221114224440542](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221114224440542.png)





###### ① 有**play持续时间**

为了您看得方便，还是直接复制结果吧...

```
harryovo@harryovo-virtual-machine:~/Desktop/lab3/osai22/labs/lab3$ blitz -g os
Beginning execution...
====================  KPL PROGRAM STARTING  ====================
Initializing Thread Scheduler...
A requests 4
------------------------------Number of dice now avail = 8
A proceeded with 4
------------------------------Number of dice now avail = 4
B requests 4
------------------------------Number of dice now avail = 4
B proceeded with 4
------------------------------Number of dice now avail = 0
C requests 5
------------------------------Number of dice now avail = 0
D requests 5
------------------------------Number of dice now avail = 0
E requests 2
------------------------------Number of dice now avail = 0
F requests 2
------------------------------Number of dice now avail = 0
G requests 1
------------------------------Number of dice now avail = 0
H requests 1
------------------------------Number of dice now avail = 0
B returned 4
------------------------------Number of dice now avail = 4
B requests 4
------------------------------Number of dice now avail = 4
A returned 4
------------------------------Number of dice now avail = 8
C proceeded with 5
------------------------------Number of dice now avail = 3
A requests 4
------------------------------Number of dice now avail = 3
C returned 5
------------------------------Number of dice now avail = 8
D proceeded with 5
------------------------------Number of dice now avail = 3
E proceeded with 2
------------------------------Number of dice now avail = 1
C requests 5
------------------------------Number of dice now avail = 1
D returned 5
------------------------------Number of dice now avail = 6
F proceeded with 2
------------------------------Number of dice now avail = 4
D requests 5
------------------------------Number of dice now avail = 4
G proceeded with 1
------------------------------Number of dice now avail = 3
H proceeded with 1
------------------------------Number of dice now avail = 2
E returned 2
------------------------------Number of dice now avail = 4
B proceeded with 4
------------------------------Number of dice now avail = 0
E requests 2
------------------------------Number of dice now avail = 0
G returned 1
------------------------------Number of dice now avail = 1
G requests 1
------------------------------Number of dice now avail = 1
F returned 2
------------------------------Number of dice now avail = 3
H returned 1
------------------------------Number of dice now avail = 4
A proceeded with 4
------------------------------Number of dice now avail = 0
H requests 1
------------------------------Number of dice now avail = 0
F requests 2
------------------------------Number of dice now avail = 0
B returned 4
------------------------------Number of dice now avail = 4
B requests 4
------------------------------Number of dice now avail = 4
A returned 4
------------------------------Number of dice now avail = 8
C proceeded with 5
------------------------------Number of dice now avail = 3
A requests 4
------------------------------Number of dice now avail = 3
C returned 5
------------------------------Number of dice now avail = 8
D proceeded with 5
------------------------------Number of dice now avail = 3
E proceeded with 2
------------------------------Number of dice now avail = 1
C requests 5
------------------------------Number of dice now avail = 1
G proceeded with 1
------------------------------Number of dice now avail = 0
D returned 5
------------------------------Number of dice now avail = 5
H proceeded with 1
------------------------------Number of dice now avail = 4
D requests 5
------------------------------Number of dice now avail = 4
F proceeded with 2
------------------------------Number of dice now avail = 2
G returned 1
------------------------------Number of dice now avail = 3
G requests 1
------------------------------Number of dice now avail = 3
E returned 2
------------------------------Number of dice now avail = 5
B proceeded with 4
------------------------------Number of dice now avail = 1
E requests 2
------------------------------Number of dice now avail = 1
F returned 2
------------------------------Number of dice now avail = 3
H returned 1
------------------------------Number of dice now avail = 4
F requests 2
------------------------------Number of dice now avail = 4
A proceeded with 4
------------------------------Number of dice now avail = 0
H requests 1
------------------------------Number of dice now avail = 0
B returned 4
------------------------------Number of dice now avail = 4
B requests 4
------------------------------Number of dice now avail = 4
A returned 4
------------------------------Number of dice now avail = 8
C proceeded with 5
------------------------------Number of dice now avail = 3
A requests 4
------------------------------Number of dice now avail = 3
C returned 5
------------------------------Number of dice now avail = 8
D proceeded with 5
------------------------------Number of dice now avail = 3
G proceeded with 1
------------------------------Number of dice now avail = 2
C requests 5
------------------------------Number of dice now avail = 2
E proceeded with 2
------------------------------Number of dice now avail = 0
D returned 5
------------------------------Number of dice now avail = 5
E returned 2
------------------------------Number of dice now avail = 7
F proceeded with 2
------------------------------Number of dice now avail = 5
D requests 5
------------------------------Number of dice now avail = 5
E requests 2
------------------------------Number of dice now avail = 5
H proceeded with 1
------------------------------Number of dice now avail = 4
B proceeded with 4
------------------------------Number of dice now avail = 0
G returned 1
------------------------------Number of dice now avail = 1
G requests 1
------------------------------Number of dice now avail = 1
F returned 2
------------------------------Number of dice now avail = 3
F requests 2
------------------------------Number of dice now avail = 3
H returned 1
------------------------------Number of dice now avail = 4
A proceeded with 4
------------------------------Number of dice now avail = 0
H requests 1
------------------------------Number of dice now avail = 0
B returned 4
------------------------------Number of dice now avail = 4
B requests 4
------------------------------Number of dice now avail = 4
A returned 4
------------------------------Number of dice now avail = 8
C proceeded with 5
------------------------------Number of dice now avail = 3
A requests 4
------------------------------Number of dice now avail = 3
C returned 5
------------------------------Number of dice now avail = 8
D proceeded with 5
------------------------------Number of dice now avail = 3
C requests 5
------------------------------Number of dice now avail = 3
E proceeded with 2
------------------------------Number of dice now avail = 1
G proceeded with 1
------------------------------Number of dice now avail = 0
E returned 2
------------------------------Number of dice now avail = 2
F proceeded with 2
------------------------------Number of dice now avail = 0
E requests 2
------------------------------Number of dice now avail = 0
D returned 5
------------------------------Number of dice now avail = 5
H proceeded with 1
------------------------------Number of dice now avail = 4
B proceeded with 4
------------------------------Number of dice now avail = 0
D requests 5
------------------------------Number of dice now avail = 0
G returned 1
------------------------------Number of dice now avail = 1
G requests 1
------------------------------Number of dice now avail = 1
F returned 2
------------------------------Number of dice now avail = 3
F requests 2
------------------------------Number of dice now avail = 3
H returned 1
------------------------------Number of dice now avail = 4
A proceeded with 4
------------------------------Number of dice now avail = 0
H requests 1
------------------------------Number of dice now avail = 0
B returned 4
------------------------------Number of dice now avail = 4
A returned 4
------------------------------Number of dice now avail = 8
C proceeded with 5
------------------------------Number of dice now avail = 3
E proceeded with 2
------------------------------Number of dice now avail = 1
C returned 5
------------------------------Number of dice now avail = 6
D proceeded with 5
------------------------------Number of dice now avail = 1
G proceeded with 1
------------------------------Number of dice now avail = 0
E returned 2
------------------------------Number of dice now avail = 2
F proceeded with 2
------------------------------Number of dice now avail = 0
D returned 5
------------------------------Number of dice now avail = 5
H proceeded with 1
------------------------------Number of dice now avail = 4
G returned 1
------------------------------Number of dice now avail = 5
F returned 2
------------------------------Number of dice now avail = 7
H returned 1
------------------------------Number of dice now avail = 8

*****  A 'wait' instruction was executed and no more interrupts are scheduled... halting emulation!  *****

Done!  The next instruction to execute will be:
000EC8: 09000000       ret     
Number of Disk Reads    = 0
Number of Disk Writes   = 0
Instructions Executed   = 3144216
Time Spent Sleeping     = 0
    Total Elapsed Time  = 3144216
harryovo@harryovo-virtual-machine:~/Desktop/lab3/osai22/labs/lab3$ 
```

