code Main

  -- OS Class: Project 3
  --
  -- Chen Junhui
  --

-----------------------------  Main  ---------------------------------

  function main ()
      -- FatalError ("Need to implement")
      InitializeScheduler ()
      --SleepingBarberProblem()
      gamingParlorProblem()
      ThreadFinish()
    endFunction



----------------------  SleepingBarberProblem  ----------------------
  const CHAIRS = 5
  
  var
    customerThreadArray: array [50] of Thread = new array of Thread {50 of new Thread }
    barberThread: Thread = new Thread
    monitor: barberCustomerMonitor
    
  function SleepingBarberProblem()
  var
    i, j: int
    monitor = new barberCustomerMonitor
    monitor.Init()
    
    barberThread.Init("barber")
    barberThread.Fork(barber, 1)
    
    for i = 0 to 49
      --printInt(i)
      customerThreadArray[i].Init("customer")
      customerThreadArray[i].Fork(customer, i+1)
      for j = 0 to 10
        --currentThread.Yield()
      endFor
    endFor
  endFunction
    
  function barber(id: int)
    monitor.barber_behave(id)
  endFunction
  
  function customer(id: int)
    monitor.customer_behave(id)
  endFunction

  
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
   
   behavior barberCustomerMonitor
   
     method Init()
       customers = new Semaphore
       barbers = new Semaphore
       mutex = new Semaphore
       
       customers.Init(0)
       barbers.Init(0)
       mutex.Init(1)
       
       waiting = 0
       
       -- to save the id of customer, so that can print in barber()
       q = new array of int {CHAIRS of 0}
       qf = 0
       qr = 0
     endMethod
     
     
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
     
     
     method cut_hair(id: int)
	print("The barber is cutting hair for customer ")
	printInt(q[qf])
	print("\n")
	
	qf = (qf+1) % CHAIRS

     endMethod
     
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
     
     
   endBehavior
   
   
   
   
   
----------------------  The Gaming Parlor Problem  ----------------------

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
     
     
   behavior gamingParlorMonitor
     method Init()
       dice_avail = 8
       mutex = new Mutex
       mutex.Init()
       
       dice_not_enough = new Condition
       dice_not_enough.Init()
       
       wait_count = 0
       wait_ = new Condition
       wait_.Init()
       
     endMethod
     
     
     
     
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
       
       
       
       
     
     
     
     
     method Print (str: String, count: int) 
        -- 
        -- This method prints the current thread's name and the arguments. 
        -- It also prints the current number of dice available. 
        -- 
          print (currentThread.name) 
          print (" ") 
          print (str) 
          print (" ") 
          printInt (count) 
          nl () 
          print ("------------------------------Number of dice now avail = ") 
          printInt (dice_avail) 
          nl () 
    endMethod 
     
  endBehavior
   
endCode






