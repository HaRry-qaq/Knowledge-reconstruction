# Lab 1

### Step 7:Assemble, Link, and Execute the “Hello” Program

Type :

```python
asm Hello.s #Assemble
```

Then we will create a new file named "Hello.o", which is named "Assemble".

 Then Type:

```python
lddd Hello.o -o Hello #Link
```

As the pdf said, although the program **Hello.s** is stand-alone, which means it does not need any library functions and does not rely on any operating system, it has to be linked to produce an executable.

"-o Hello" rename the executable from "a.out" to "Hello".

Last Type:

```python
blitz –g Hello #Execute
```

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918192907873.png" alt="image-20220918192907873" style="zoom:80%;" />









### Step 8:Run the “Echo” Program

Type in the following commands:

```
asm Echo.s
lddd Echo.o –o Echo
blitz Echo
```

then we need to type in "g" to run.

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918193709557.png" alt="image-20220918193709557" style="zoom:80%;" />





### Step 9:Compile and Execute a KPL Program called “HelloWorld”

Type the following commands:

```python
kpl -unsafe System
asm System.s
kpl HelloWorld
asm HelloWorld.s
asm Runtime.s
lddd Runtime.o System.o HelloWorld.o -o HelloWorld
```

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918194955535.png" alt="image-20220918194955535" style="zoom:80%;" />



### Step 10:Modify the HelloWorld Program

Modify the HelloWorld.c program by un-commenting the line "--foo (10)"

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918195704368.png" alt="image-20220918195704368" style="zoom:80%;" />

**there is an extra line "The value of b is 12".**



### Step 11: Try Some of the Emulator Commands

#### 1. t  (program will execute after "debug" sign)

which means executing a single high-level KPL language statement at a time. Typing “t”
several times to walk through the execution of the HelloWorld program.

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918200516933.png" alt="image-20220918200516933" style="zoom:80%;" />



#### 2. i (display details of the cpu)

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203030604.png" alt="image-20220918203030604" style="zoom:80%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203143945.png" alt="image-20220918203143945" style="zoom:80%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203259432.png" alt="image-20220918203259432" style="zoom:80%;" />



#### 3. st (information of stack)

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203429843.png" alt="image-20220918203429843" style="zoom:80%;" />



#### 4. fr (information of frame)

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203526010.png" alt="image-20220918203526010" style="zoom:80%;" />



#### 5. step (walk through the execution of an assembly language program, line-by-line.)

output:

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20220918203822243.png" alt="image-20220918203822243" style="zoom:80%;" />