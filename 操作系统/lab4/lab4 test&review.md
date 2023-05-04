## lab4 test&review

陈俊卉   学号：2020212256   班级：2020219111

[TOC]



### Task 1:  Threads and the ThreadManager

#### 1、代码解释

##### ① Init

```c
      ----------  ThreadManager . Init  ----------

      method Init ()
        --
        -- This method is called once at kernel startup time to initialize
        -- the one and only "ThreadManager" object.
        -- 
        var
          i: int
          
        
        
          print ("Initializing Thread Manager...\n")
          
          -- initialize the array of threads
          self.threadTable = new array of Thread {10 of new Thread}
          
          -- initialize each thread
          self.threadTable[0].Init("0")
          self.threadTable[1].Init("1")
          self.threadTable[2].Init("2")
          self.threadTable[3].Init("3")
          self.threadTable[4].Init("4")
          self.threadTable[5].Init("5")
          self.threadTable[6].Init("6")
          self.threadTable[7].Init("7")
          self.threadTable[8].Init("8")
          self.threadTable[9].Init("9")
          

          -- initialize freelist
          self.freeList = new List [Thread]
          
          for i = 0 to 9
            self.threadTable[i].status = UNUSED
            self.freeList.AddToEnd (&threadTable[i])
          endFor
          
          
          -- initialize mutex and condition
          self.threadManagerLock = new Mutex
          self.aThreadBecameFree = new Condition
          
          self.threadManagerLock.Init()
          self.aThreadBecameFree.Init()
          
        endMethod
```

**解释：简单的初始化。**



##### ② GetANewThread

```c
      ----------  ThreadManager . GetANewThread  ----------

      method GetANewThread () returns ptr to Thread
        -- 
        -- This method returns a new Thread; it will wait
        -- until one is available.
        -- 
        var th: ptr to Thread
          
        
           -- lock
           self.threadManagerLock.Lock()
           
           while freeList.IsEmpty() == true
             aThreadBecameFree.Wait(&self.threadManagerLock)
           endWhile
           
           -- remove and return_ a thread from freelist
           th = freeList.Remove()
           
           
           th.status = JUST_CREATED
           
           
           -- unlock
           self.threadManagerLock.Unlock()
           
           return th
        endMethod
```

**解释：**

- 因为GetANewThread是入口方法，所以需要在一开始获得锁，在最后释放锁。
- GetANewThread需要在freeList中删除并返回一个线程，所以如果freeList为空，我们需要等待线程条件满足再继续执行。而由于condition是mesa-style的，条件变量signal(broadcast)后并不是立即执行，而需要再次进行条件判断，所以我们需要使用while，而不是if.
- 将线程从freeList取出后，我们需要将其状态设为JUST_CREATED.
- 最后释放锁。



##### ③ FreeThread

```
      method FreeThread (th: ptr to Thread)
        -- 
        -- This method is passed a ptr to a Thread;  It moves it
        -- to the FREE list.
        -- 
           
           -- lock
           self.threadManagerLock.Lock()
           
           
           
           th.status = UNUSED
           
           freeList.AddToEnd (th)
           aThreadBecameFree.Broadcast(&self.threadManagerLock)
           
           
           -- unlock
           self.threadManagerLock.Unlock()

           
        endMethod

    endBehavior
```

**解释：**

- 因为FreeThread是入口方法，所以需要在一开始获得锁，在最后释放锁。
- 需要将该线程的状态置为UNUSED.
- 然后将其放入freeList中。
- 依据pdf，需要向所有等待该条件的人发出信号，故这里使用broadcast.
- 最后释放锁。



#### 2、输出

![image-20221123213608608](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221123213608608.png)







### Task 2:  Processes and the ProcessManager

#### 1、代码解释

##### ① Init

```c
      ----------  ProcessManager . Init  ----------

      method Init ()
        --
        -- This method is called once at kernel startup time to initialize
        -- the one and only "processManager" object.  
        --
        var
          i: int
        
           self.processTable = new array of ProcessControlBlock {10 of new ProcessControlBlock}
           
           
           for i = 0 to 9
             self.processTable[i].Init()
           endFor
           
           
           self.processManagerLock = new Mutex
           self.processManagerLock.Init()
           
           self.aProcessBecameFree = new Condition
           self.aProcessBecameFree.Init()
           
           self.aProcessDied = new Condition
           self.aProcessDied.Init()
           
           self.freeList = new List [ProcessControlBlock]

           
           for i = 0 to 9
             self.freeList.AddToEnd (&processTable[i])
           endFor
           
           self.nextPid = 0
           
           
        -- NOT IMPLEMENTED
        endMethod
```

**解释：初始化，pid将会在GetANewProcess初始化。**



##### ② GetANewProcess

```c
      ----------  ProcessManager . GetANewProcess  ----------

      method GetANewProcess () returns ptr to ProcessControlBlock
        --
        -- This method returns a new ProcessControlBlock; it will wait
        -- until one is available.
        --
          -- NOT IMPLEMENTED
          var pr :ptr to ProcessControlBlock
          
          -- lock
          self.processManagerLock.Lock()
           
          while freeList.IsEmpty() == true
            aProcessBecameFree.Wait(&self.processManagerLock) 
          endWhile
          
          -- remove and return_ a process from freelist
          pr = freeList.Remove()
           
          self.nextPid = self.nextPid + 1
          pr.pid = self.nextPid
           
          pr.status = ACTIVE
           
          -- unlock
          self.processManagerLock.Unlock()
                    
          
          return pr
        endMethod
```

**解释：**

- GetANewProcess需要对pid进行分配。而分配的数字是ProcessManager所保存的nextPid.
- 同样地，GetANewProcess是入口方法，所以需要在一开始获得锁，在最后释放锁。
- GetANewProcess的结构与GetANewThread基本一致：freeList为空时需要等待、满足条件后从freeList拿出相应process，更新nextPid后为该process设置pid，并将其的status设为ACTIVE.
- 最后释放锁。



##### ③ FreeProcess

```c
     ----------  ProcessManager . FreeProcess  ----------

      method FreeProcess (p: ptr to ProcessControlBlock)
        --
        -- This method is passed a ptr to a Process;  It moves it
        -- to the FREE list.
        --
          -- NOT IMPLEMENTED
          
          
          -- lock
           self.processManagerLock.Lock()
           
           
           p.status = FREE
           p.pid = -1
           
           freeList.AddToEnd (p)
           aProcessBecameFree.Broadcast(&self.processManagerLock)
           
           
           
           -- unlock
           self.processManagerLock.Unlock()

          
        endMethod
```

**解释：**

- FreeProcess是入口方法，所以需要在一开始获得锁，在最后释放锁。
- 我们需要将该process的状态设为FREE，并将pid清空，加入freeList.
- 依据pdf，需要向所有等待该条件的人发出信号，故这里使用broadcast.
- 最后释放锁。



#### 2、输出：

![image-20221123220054423](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221123220054423.png)





### Task 3: The Frame Manager

#### 1、代码解释

##### ① GetNewFrames

```c
      ----------  FrameManager . GetNewFrames  ----------

      method GetNewFrames (aPageTable: ptr to AddrSpace, numFramesNeeded: int)
          -- NOT IMPLEMENTED
          var 
            free_frame_index: int
            free_frame_addr: int
            i: int
          
          frameManagerLock.Lock()
          
          wait_count = wait_count + 1
          if wait_count > 1
            wait_.Wait(&frameManagerLock)
          endIf
          
          while numberFreeFrames < numFramesNeeded
            newFramesAvailable.Wait(&frameManagerLock)
          endWhile
          
          -- now available
          
          for i = 0 to numFramesNeeded - 1
            free_frame_index = framesInUse.FindZeroAndSet()
            free_frame_addr = PHYSICAL_ADDRESS_OF_FIRST_PAGE_FRAME + (free_frame_index * PAGE_SIZE)
            aPageTable.SetFrameAddr (i, free_frame_addr)
          endFor
          
          numberFreeFrames = numberFreeFrames - numFramesNeeded
          
          aPageTable.numberOfPages = numFramesNeeded
          
          
          -- add
          wait_count = wait_count - 1
          wait_.Signal(&frameManagerLock)
          
          
          frameManagerLock.Unlock()
          
          
        endMethod
```

**解释：**

- 首先上锁。

- 根据pdf所说：
  - You’ll need to do a Broadcast, because a Signal will only wake up one thread.  The thread that gets 
    awakened may not have enough free frames to complete, but other waiting threads may be able to 
    proceed.  A broadcast should be adequate, but perhaps after carefully studying the Game Parlor problem, 
    **you will find a more elegant approach which wakes up only a single thread.** 
  - 为了防止饥饿，笔者迁移了在Game Parlor problem中解决饥饿的**“优雅的方法”**：使用额外的一个条件变量wait_，**以保证每次进入while判断的进程只有一个，不会发生争抢的情况**。wait_count用来累计当前等待的进程数。只要这个数大于1，后来的进程都要进入条件变量wait\_的等待队列中，直到前面已经进入while判断的进程满足条件，被成功分配帧后，条件变量wait\_才会被signal。
- 当可用帧大于所需帧，即成功分配后，需要在framesInUse这个BitMap中下标由小到大找到可用帧并计算出相对应的地址，存储已分配帧的地址。
- 随后numberFreeFrames减去刚刚分配的帧数，更新aPageTable.numberOfPages的值。
- 该进程的分配已经结束，等待分配的数量减1，同时对wait\_进行signal，下一个进程进入分配帧的条件判断。
- 最后释放锁。



##### ②  ReturnAllFrames

```c
      ----------  FrameManager . ReturnAllFrames  ----------

      method ReturnAllFrames (aPageTable: ptr to AddrSpace)
          -- NOT IMPLEMENTED
          var 
            numFramesReturned: int
            frameAddr: int
            bitNumber: int
            i: int
            
          
          frameManagerLock.Lock()
          
          numFramesReturned = aPageTable.numberOfPages
          
          for i = 0 to numFramesReturned - 1
            frameAddr = aPageTable.ExtractFrameAddr(i)
            bitNumber = (frameAddr -  PHYSICAL_ADDRESS_OF_FIRST_PAGE_FRAME) /  PAGE_SIZE
            framesInUse.ClearBit(bitNumber)
          endFor
          
          -- ??? whether needing to set 0?  
          -- A:need to but not neccessary:it may be covered by next need_num.
          --aPageTable.numberOfPages = 0
          
          numberFreeFrames = numberFreeFrames + numFramesReturned
          
          newFramesAvailable.Signal(&frameManagerLock)
          

          frameManagerLock.Unlock()
          
        endMethod
```

**解释：**

- 首先上锁。
- 首先获取将要释放的帧数。
- 随后一个接一个地获取要释放的帧的地址，并转化为对应的bitmap上的bitNumber，最后clear.
- 理论上，此时aPageTable.numberOfPages需要置零。但考虑到之后重新申请帧时会直接覆盖值，所以不必要。
- 更新numberFreeFrames，并对条件变量newFramesAvailable进行signal.（但不代表立刻执行，还需要进行再次的条件判断——即可用帧是否大于所需帧）
- 最后释放锁。



#### 2、输出：

![image-20221123223336777](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221123223336777.png)

![image-20221123223355628](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221123223355628.png)



![image-20221123223425005](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221123223425005.png)









### Task 4:  Change Condition Variables to Hoare Semantics

#### 1、分析

- MESA semantics 和 Hoare Semantics 的区别：
  - Hoare Semantics：有一个**入口等待队列**以便管程外面的进程等待。在管程内有条件变量，若进程等待并释放互斥权，则在该条件变量上等待；在管程内有**紧急等待队列**，等待的进程进入该队列中，优先级高于入口等待队列。**使用if进行条件的判断，当紧急等待队列被signal，则立即执行。**由于使用if，所以Hoare不可能有broadcast方法，否则将产生混乱，并且，在signal前，还需要将锁交给被signal的进程,而不是解锁，以确保signal之后不会发生竞争，只有唯一一个进程会响应并接受条件。
  - MESA semantics：只有一个队列，当队列signal时，通知队头进程，但此时还不一定满足条件，可能仍然需要进入等待队列。由于被signal时不一定满足条件，所以需要使用while进行条件判断（有可能发生竞争从而使得条件仍然不满足），确保满足条件才进行下一步操作。

**事实上Hoare Semantics跟我们在task3采用的“优雅的方法”有点相似。**

#### 2、个别代码的修改

##### ① mutex需要增加Give方法，在signal之前将锁交给刚刚唤醒的进程，确保signal之后不会发生竞争，只有唯一一个进程会响应并接受条件。

```c
      -----------  Mutex . Give  -----------

	method Give (t: ptr to Thread)
          var
             oldIntStat: int          
          oldIntStat = SetInterruptsTo (DISABLED)
          if heldBy != currentThread
             FatalError ("Attempt to give away the mutex by a thread not holding it")
          endIf
          heldBy = t
          oldIntStat = SetInterruptsTo (oldIntStat)
        endMethod
```

- 首先需要关中断，因为这是原语操作！
- 将当前的锁的归属交给t.·
- 开中断。



##### ② condition需要在将进程置为ready态后，当前进程将锁给予该进程。

```c
     ----------  Condition . Signal  ----------

      method Signal (mutex: ptr to HoareMutex)
          var
            oldIntStat: int
	             t: ptr to Thread
          oldIntStat = SetInterruptsTo (DISABLED)
	  if !mutex. IsHeldByCurrentThread ()
	     FatalError ("Attempt to wait on condition when mutex is not held")
          endIf
          t = waitingThreads.Remove ()
          if t	
             t.status = READY
              
             -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	         mutex.Give (t)
              
             readyList.AddToEnd (t)
	  endIf
      	oldIntStat = SetInterruptsTo (oldIntStat)
      endMethod

  endBehavior
```



##### ③ Condition 的 Wait

condition的Wait中，在唤醒并不需要重新上锁，因为signal后锁的heldBy被Give到刚刚被signal的进程，相当于保证了signal后下一个进程一定是这个刚刚唤醒的进程，而不是排在Lock队列中较前的进程（如果不give，且跟MESA一样只在唤醒后Lock，则当前进程只能排在Lock队列的后面，仍然可能被其他进程抢先占用资源）。前面的进程如果试图Lock，将会在Lock队列中排在刚刚唤醒的这个进程的后面。

```c
      ----------  Condition . Wait  ----------

      method Wait (mutex: ptr to Mutex)
          var
            oldIntStat: int
          if ! mutex.IsHeldByCurrentThread ()
            FatalError ("Attempt to wait on condition when mutex is not held")
          endIf
          oldIntStat = SetInterruptsTo (DISABLED)
          mutex.Unlock ()
          waitingThreads.AddToEnd (currentThread)
          currentThread.Sleep ()
              
          -- no lock
              
          oldIntStat = SetInterruptsTo (oldIntStat)
        endMethod
```



#### 3、测试

为了测试，笔者将lab4中与ThreadManager类与测试有关的所有代码写成ThreadManager2：

- 将其中的mutex换为HoareMutex，condition换为HoareCondition，并将判断条件的while改成if.

- **还需要在以下两处分别添加判断，防止重复上锁或在最后时没有解锁：**

  - <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124015506236.png" alt="image-20221124015506236" style="zoom: 50%;" />

  

  - <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124015605464.png" alt="image-20221124015605464" style="zoom:50%;" />

  

**具体代码改动如下所示（所有代码都是新增，不会影响task1~3的执行。除了初始化会导致进程执行时间不同导致输出结果不同）：**



![image-20221124012318349](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124012318349.png)



![image-20221124012340005](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124012340005.png)



![image-20221124010750453](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124010750453.png)





```c
-----------------------------  RunThreadManager2Tests  ---------------------------------
--
-- This function tests the ThreadManager.  It creates a bunch of threads
-- (NUM_THREADS) and starts each thread running.  Each thread will execute
-- the "TestThreadManager" function.  The main thread will then wait until all
-- the threads complete.  To control this, there is a single Semaphore "allDone".
-- Each TestThreadManager thread signals it and the main thread will wait
-- for NUM-THREAD times, i.e., until all threads have finished.
--
-- Each TestThreadManager does basically this:
--        loop NUMBER_ITERATIONS times
--           call GetANewThread
--           wait
--           call FreeThread
--           wait
--        endLoop
--
  function RunThreadManager2Tests ()
      var i: int
          th: ptr to Thread

      allDone.Init (0)
      freeze.Init (0)
      uniqueNumberLock.Init ()
      nextUnique = 1

      print ("\n\n*****  THREAD-MANAGER TEST  *****\n\n")

      for i = 1 to NUM_THREADS
        th = alloc Thread
        th.Init ("TestThreadManager2")
        th.Fork (TestThreadManager2, i)
      endFor

      -- Wait for all the testing threads to complete.
      -- (Make sure you see the completion message!)
      for i = 1 to NUM_THREADS
        allDone.Down ()
      endFor

      if GetUniqueNumber (1) != NUM_THREADS * NUMBER_ITERATIONS + 1
        FatalError ("Concurrency control failure (1)")
      endIf
      print ("\n\n***** THREAD-MANAGER TEST COMPLETED SUCCESSFULLY *****\n\n")

    endFunction
```



```c
-----------------------------  TestThreadManager2  ---------------------------------
--
-- This function is the main function for a thread which will test the
-- ThreadManager.  It will request and return Thread objects.  First, it
-- grabs a unique number and stuffs it in the Thread.  Later, it makes sure that
-- the number is unchanged.  It could only have changed if some other tester
-- was allowed to access this Thread object before this tester returned it.
--
  function TestThreadManager2 (myID: int)
      var i, j, e: int
          th: ptr to Thread
      -- printIntVar ("Thread started", myID)
      for i = 1 to NUMBER_ITERATIONS
        printInt (myID)
        e = GetUniqueNumber (1)
        th = threadManager2.GetANewThread ()
        th.regs[0] = e
        for j = 1 to WAIT_TIME+i
          currentThread.Yield ()
        endFor
        if e != th.regs[0]
          FatalError ("Concurrency control failure (2)")
        endIf
        printChar ('.')
        threadManager2.FreeThread (th)
        for j = 1 to WAIT_TIME-i
          currentThread.Yield ()
        endFor
      endFor
      allDone.Up ()
      freeze.Down ()
    endFunction
```



```c
    -----------------------------  ThreadManager2  ---------------------------------
  --
  --  There is only one instance of this class, created at startup time.
  --
  class ThreadManager2
    superclass Object
    fields
      threadTable: array [MAX_NUMBER_OF_PROCESSES] of Thread
      freeList: List [Thread]
      
      -- add 
      threadManager2Lock: HoareMutex
      aThreadBecameFree: HoareCondition
      
    methods
      Init ()
      Print ()
      GetANewThread () returns ptr to Thread
      FreeThread (th: ptr to Thread)
  endClass
```



```c
-----------------------------  ThreadManager2  ---------------------------------

  behavior ThreadManager2

      ----------  ThreadManager2 . Init  ----------

      method Init ()
        --
        -- This method is called once at kernel startup time to initialize
        -- the one and only "ThreadManager2" object.
        -- 
        var
          i: int
          
        
        
          print ("Initializing Thread Manager 2...\n")
          
          -- initialize the array of threads
          self.threadTable = new array of Thread {10 of new Thread}
          
          -- initialize each thread
          self.threadTable[0].Init("0")
          self.threadTable[1].Init("1")
          self.threadTable[2].Init("2")
          self.threadTable[3].Init("3")
          self.threadTable[4].Init("4")
          self.threadTable[5].Init("5")
          self.threadTable[6].Init("6")
          self.threadTable[7].Init("7")
          self.threadTable[8].Init("8")
          self.threadTable[9].Init("9")
          

          -- initialize freelist
          self.freeList = new List [Thread]
          
          for i = 0 to 9
            self.threadTable[i].status = UNUSED
            self.freeList.AddToEnd (&threadTable[i])
          endFor
          
          
          -- initialize mutex and condition
          self.threadManager2Lock = new HoareMutex
          self.aThreadBecameFree = new HoareCondition
          
          self.threadManager2Lock.Init()
          self.aThreadBecameFree.Init()
          
        endMethod

      ----------  ThreadManager2 . Print  ----------

      method Print ()
        -- 
        -- Print each thread.  Since we look at the freeList, this
        -- routine disables interrupts so the printout will be a
        -- consistent snapshot of things.
        -- 
        var i, oldStatus: int
          oldStatus = SetInterruptsTo (DISABLED)
          print ("Here is the thread table...\n")
          for i = 0 to MAX_NUMBER_OF_PROCESSES-1
            print ("  ")
            printInt (i)
            print (":")
            ThreadPrintShort (&threadTable[i])
          endFor
          print ("Here is the FREE list of Threads:\n   ")
          freeList.ApplyToEach (PrintObjectAddr)
          nl ()
          oldStatus = SetInterruptsTo (oldStatus)
        endMethod

      ----------  ThreadManager2 . GetANewThread  ----------

      method GetANewThread () returns ptr to Thread
        -- 
        -- This method returns a new Thread; it will wait
        -- until one is available.
        -- 
        var th: ptr to Thread
          
        
           -- lock
           if threadManager2Lock.IsHeldByCurrentThread() == false
             self.threadManager2Lock.Lock()
           endIf
           
           if freeList.IsEmpty() == true
             aThreadBecameFree.Wait(&self.threadManager2Lock)
           endIf
           
           -- remove and return_ a thread from freelist
           th = freeList.Remove()
           
           
           th.status = JUST_CREATED
           
           
           -- unlock
           self.threadManager2Lock.Unlock()
           
           return th
        endMethod

      ----------  ThreadManager2 . FreeThread  ----------

      method FreeThread (th: ptr to Thread)
        -- 
        -- This method is passed a ptr to a Thread;  It moves it
        -- to the FREE list.
        -- 
           
           -- lock
           self.threadManager2Lock.Lock()
           
           
           
           th.status = UNUSED
           
           freeList.AddToEnd (th)
           aThreadBecameFree.Signal(&self.threadManager2Lock)
           
           
           -- no need to unlock because the lock is given
           if threadManager2Lock.IsHeldByCurrentThread() == true
             self.threadManager2Lock.Unlock()
           endIf

           
        endMethod

    endBehavior
```





**结果如下，输出正确：**

![image-20221124015704500](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124015704500.png)







**注1：代码修改后由于运行代码的增加，运行时间不一样，输出也会相应变化。**



**注2：执行task4之前，需要先修改Main.c的下列两处地方；**

**而在运行task1~3时，需要注释掉threadmanager2的初始化，以得到上面截图中的结果（初始化的增加会影响进程相应的执行时间）：**

![image-20221124100520733](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124100520733.png)



![image-20221124100600609](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221124100600609.png)

