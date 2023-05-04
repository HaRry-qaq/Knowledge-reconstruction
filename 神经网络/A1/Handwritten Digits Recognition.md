## A1：Handwritten Digits Recognition

姓名：陈俊卉     班级：2020219111      学号：2020212256

[TOC]



### 一、实验要求


- • In this assignment you will practice putting together a simple image classification pipeline, based on the Softmax and the fully-connected classifier. The goals of this assignment are as follows:

  ​	– understand the basic **Image** **Classification pipeline** and the data-driven approach (train/predict stages)

  ​	– understand the train/val/test **splits** and the use of validation data for **hyper-parameter tuning**

  ​	– implement and apply a **Softmax** classifier

  ​	– implement and apply a **Fully-connected** **neural network** classifier

  ​	– understand the differences and tradeoffs between these classifiers implement various **update** **rules** used to optimize Neural Networks

  

  • Do something extra! 

  ​	– Maybe you can experiment with a different loss function and regularization? (+5 points)

  ​	– Or maybe you can experiment with different optimization algorithm (e.g., batch GD, online GD, mini-batch GD, SGD, or other optimization alg., e.g., Momentum, Adsgrad, Adam, Admax)  (+5 points)

  

  补充：[MINST](http://yann.lecun.com/exdb/mnist/)是一个手写数字数据集，包括了若干手写数字体及其对应的数字，共60000个训练样本，10000个测试样本。每个手写数字体被表示为一个28*28的向量。



### 二、模型简介

​		本次实验规定了使用“ Fully-connected neural network classifier”进行训练，且要求使用softmax分类器。但没有对**“是否使用激活函数”**，**“是否进行batch normalization”**进行要求。所以本次实验的模型设计了三种，分别为：

- 只有全连接层和softmax的模型（net_1）
- 有全连接层、激活层和softmax的模型（net_2）
- 有全连接层、激活层、batch normalization和softmax的模型（net_3）
- 激活层使用ReLU函数

**注：由于torch定义的函数nn.CrossEntropyLoss()中含有了softmax，所以当使用该loss时，不需要经过softmax层。**

具体定义如下所示：

- **in_dim = 28 * 28**
- **n_hidden_1 = 512**
- **n_hidden_2 = 128**
- **out_dim = 10**

```python
class net_1(nn.Module):
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super(net_1, self).__init__()
        self.layer1 = nn.Linear(in_dim, n_hidden_1)
        self.layer2 = nn.Linear(n_hidden_1, n_hidden_2)
        self.layer3 = nn.Linear(n_hidden_2, out_dim)
        #self.softmax = nn.Softmax()

    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        #x = self.softmax(x)
        return x
```



```python
class net_2(nn.Module):
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super(net_2, self).__init__()
        self.layer1 = nn.Sequential(nn.Linear(in_dim, n_hidden_1), nn.ReLU(True))
        self.layer2 = nn.Sequential(nn.Linear(n_hidden_1, n_hidden_2), nn.ReLU(True))
        self.layer3 = nn.Sequential(nn.Linear(n_hidden_2, out_dim))
        #self.softmax = nn.Softmax()

    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        #x = self.softmax(x)
        return x
```



```python
class net_3(nn.Module):
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super(net_3, self).__init__()
        self.layer1 = nn.Sequential(nn.Linear(in_dim, n_hidden_1), nn.BatchNorm1d(n_hidden_1), nn.ReLU(True))
        self.layer2 = nn.Sequential(nn.Linear(n_hidden_1, n_hidden_2), nn.BatchNorm1d(n_hidden_2), nn.ReLU(True))
        self.layer3 = nn.Sequential(nn.Linear(n_hidden_2, out_dim))
        #self.softmax = nn.Softmax()
        

    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        #x = self.softmax(x)
        return x
```



### 三、训练过程

#### 1、获取数据集并进行预处理

① 首先定义一个transforms.Compose，将处理过程集成：

```python
data_tf = transforms.Compose(
    [transforms.ToTensor(),
     transforms.Normalize([0.5], [0.5])])
```

先将图片转换为Tensor，再进行标准化：其中均值与标准差均为0.5.

② 随后获取MNIST数据集并使用Dataloader进行读取：

```python
train_dataset = datasets.MNIST(root='./data', train=True, transform=data_tf, download=True)
test_dataset = datasets.MNIST(root='./data', train=False, transform=data_tf)

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)
```

**注：由于本次任务的训练量较小，且不需要在训练途中进行调参，所以并没有建立验证集，直接多次训练测试完后填写表格。**



#### 2、训练测试过程

我们使用pytorch的SGD优化器（实际上是MBGD，即小批量梯度下降），并使用nn.CrossEntropyLoss()作为损失函数（两者在之后都会更换进行对比实验）。超参数的调整范围分别为：

- **batch_size：[32，64，128，256]**
- **初始learning_rate：[0.01，0.02，0.03]**
- **num_epoches：[3，5，7，10，15]**

##### （1）Net_1：只有全连接层和softmax的模型

###### ① batch_size = 32

横坐标：iter     纵坐标：loss

|  LOSS   |                          epoch = 3                           |                          epoch = 5                           |                          epoch = 7                           |                          epoch = 10                          |                          epoch = 15                          |
| :-----: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| lr=0.01 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126223115610.png" alt="image-20221126223115610" style="zoom: 33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126223348921.png" alt="image-20221126223348921" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126223607975.png" alt="image-20221126223607975" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126223848130.png" alt="image-20221126223848130" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126224226921.png" alt="image-20221126224226921" style="zoom:33%;" /> |
| lr=0.02 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126224409075.png" alt="image-20221126224409075" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126224547776.png" alt="image-20221126224547776" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126224748828.png" alt="image-20221126224748828" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126225124560.png" alt="image-20221126225124560" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126225515016.png" alt="image-20221126225515016" style="zoom:33%;" /> |
| lr=0.03 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126225632629.png" alt="image-20221126225632629" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126225810857.png" alt="image-20221126225810857" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126230050457.png" alt="image-20221126230050457" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126230420866.png" alt="image-20221126230420866" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126230837446.png" alt="image-20221126230837446" style="zoom:33%;" /> |



|   ACC   | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-----: | :-------: | :-------: | :-------: | :--------: | :--------: |
| lr=0.01 | 0.917800  | 0.916200  | 0.919200  |  0.918600  |  0.917600  |
| lr=0.02 | 0.917200  | 0.907400  | 0.913900  |  0.919600  |  0.915700  |
| lr=0.03 | 0.911000  | 0.917300  | 0.918500  |  0.913700  |  0.915300  |



| TEST LOSS | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-------: | :-------: | :-------: | :-------: | :--------: | :--------: |
|  lr=0.01  | 0.291336  | 0.289206  | 0.286459  |  0.291607  |  0.284422  |
|  lr=0.02  | 0.293134  | 0.324542  | 0.299019  |  0.283997  |  0.299071  |
|  lr=0.03  | 0.298780  | 0.289120  | 0.280897  |  0.303246  |  0.285114  |





###### ② batch_size = 64

|  LOSS   |                          epoch = 3                           |                          epoch = 5                           |                          epoch = 7                           |                          epoch = 10                          |                          epoch = 15                          |
| :-----: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| lr=0.01 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126202917102.png" alt="image-20221126202917102" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126203257714.png" alt="image-20221126203257714" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126203554117.png" alt="image-20221126203554117" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126213843980.png" alt="image-20221126213843980" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126214314517.png" alt="image-20221126214314517" style="zoom:33%;" /> |
| lr=0.02 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126214705861.png" alt="image-20221126214705861" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126215219074.png" alt="image-20221126215219074" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126215905485.png" alt="image-20221126215905485" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126220505363.png" alt="image-20221126220505363" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126220843077.png" alt="image-20221126220843077" style="zoom:33%;" /> |
| lr=0.03 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126221149980.png" alt="image-20221126221149980" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126221324196.png" alt="image-20221126221324196" style="zoom: 33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126221824361.png" alt="image-20221126221824361" style="zoom: 33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126222042566.png" alt="image-20221126222042566" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221126222642181.png" alt="image-20221126222642181" style="zoom:33%;" /> |



|   ACC   | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-----: | :-------: | :-------: | :-------: | :--------: | :--------: |
| lr=0.01 | 0.911600  | 0.919300  | 0.918700  |  0.916000  |  0.919300  |
| lr=0.02 | 0.912100  | 0.917900  | 0.918200  |  0.921100  |  0.920200  |
| lr=0.03 | 0.907800  | 0.917200  | 0.914700  |  0.917500  |  0.921600  |



| TEST LOSS | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-------: | :-------: | :-------: | :-------: | :--------: | :--------: |
|  lr=0.01  | 0.308932  | 0.287011  | 0.291265  |  0.288875  |  0.282603  |
|  lr=0.02  | 0.311689  | 0.292793  | 0.288230  |  0.281129  |  0.276192  |
|  lr=0.03  | 0.335004  | 0.294877  | 0.297949  |  0.287533  |  0.281300  |





###### ③ batch_size = 128

|  LOSS   |                          epoch = 3                           |                          epoch = 5                           |                          epoch = 7                           |                          epoch = 10                          |                          epoch = 15                          |
| :-----: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| lr=0.01 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127164151153.png" alt="image-20221127164151153" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127164315962.png" alt="image-20221127164315962" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127164503140.png" alt="image-20221127164503140" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127164810253.png" alt="image-20221127164810253" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127165127343.png" alt="image-20221127165127343" style="zoom:33%;" /> |
| lr=0.02 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127165408062.png" alt="image-20221127165408062" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127165632391.png" alt="image-20221127165632391" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127165837723.png" alt="image-20221127165837723" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127170046909.png" alt="image-20221127170046909" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127170350119.png" alt="image-20221127170350119" style="zoom:33%;" /> |
| lr=0.03 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127170537459.png" alt="image-20221127170537459" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127170758100.png" alt="image-20221127170758100" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127170930400.png" alt="image-20221127170930400" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127171148403.png" alt="image-20221127171148403" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127171508959.png" alt="image-20221127171508959" style="zoom:33%;" /> |



|   ACC   | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-----: | :-------: | :-------: | :-------: | :--------: | :--------: |
| lr=0.01 | 0.901100  | 0.912300  | 0.914600  |  0.917400  |  0.922200  |
| lr=0.02 | 0.912500  | 0.914400  | 0.920400  |  0.920400  |  0.921400  |
| lr=0.03 | 0.901200  | 0.917300  | 0.919200  |  0.916400  |  0.922500  |



| TEST LOSS | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-------: | :-------: | :-------: | :-------: | :--------: | :--------: |
|  lr=0.01  | 0.338656  | 0.308099  | 0.296644  |  0.292506  |  0.280458  |
|  lr=0.02  | 0.303856  | 0.299330  | 0.283538  |  0.290468  |  0.280527  |
|  lr=0.03  | 0.332167  | 0.284488  | 0.283668  |  0.284560  |  0.271662  |



###### ③ batch_size = 256

|  LOSS   |                          epoch = 3                           |                          epoch = 5                           |                          epoch = 7                           |                          epoch = 10                          |                          epoch = 15                          |
| :-----: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| lr=0.01 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127172944022.png" alt="image-20221127172944022" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127175432617.png" alt="image-20221127175432617" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127174014254.png" alt="image-20221127174014254" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127174226871.png" alt="image-20221127174226871" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127175738558.png" alt="image-20221127175738558" style="zoom:33%;" /> |
| lr=0.02 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127174734731.png" alt="image-20221127174734731" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127175103351.png" alt="image-20221127175103351" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127175243896.png" alt="image-20221127175243896" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127175943213.png" alt="image-20221127175943213" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127180237391.png" alt="image-20221127180237391" style="zoom:33%;" /> |
| lr=0.03 | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127180402683.png" alt="image-20221127180402683" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127180521327.png" alt="image-20221127180521327" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127180713948.png" alt="image-20221127180713948" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127180930697.png" alt="image-20221127180930697" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127181219157.png" alt="image-20221127181219157" style="zoom:33%;" /> |



|   ACC   | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-----: | :-------: | :-------: | :-------: | :--------: | :--------: |
| lr=0.01 | 0.887100  | 0.899000  | 0.906900  |  0.913700  |  0.916400  |
| lr=0.02 | 0.902500  | 0.909500  | 0.914600  |  0.918400  |  0.919300  |
| lr=0.03 | 0.906000  | 0.914800  | 0.912900  |  0.908700  |  0.916800  |



| TEST LOSS | epoch = 3 | epoch = 5 | epoch = 7 | epoch = 10 | epoch = 15 |
| :-------: | :-------: | :-------: | :-------: | :--------: | :--------: |
|  lr=0.01  | 0.411369  | 0.348102  | 0.323320  |  0.304524  |  0.291010  |
|  lr=0.02  | 0.341368  | 0.311227  | 0.298958  |  0.286631  |  0.284640  |
|  lr=0.03  | 0.323450  | 0.300734  | 0.300894  |  0.305032  |  0.299708  |



##### （2）分析：

- batch_size：
  - batch_size = 32，训练loss波动较大，测试效果随着epoch的增加，ACC也没有明显的增长趋势。这说明对于训练样本为60000的训练集来说，batch_size = 32过于小。
  - batch_size = 64，训练稳定性有所增加，训练loss的波动有所减小；在lr = 0.02是，增大epoch对ACC开始有较为明显的增长趋势，test loss也在逐渐变小；但可能是lr = 0.03对于batch_size = 64来说较大，所以在epoch增大时有异常下降的情况，但ACC总体的趋势还是增长的。
  - batch_size = 128，对于三个lr，在epoch增加的情况下ACC基本均为增长趋势。这说明该batch_size是适合该模型的。但由于epoch较小，可能还没有到达理论上的最优解。
  - batch_size = 256，由于batch_size的逐渐增加，参数更新次数随之减少，可能导致训练轮数并不足够。可以看到，lr相同时，增大epoch，虽然仍然保持上升趋势，但是ACC相比batch_size较小时有较大的下滑。这可以一定程度上通过增大学习率来弥补。但增大学习率本身是不稳定的，他需要重新寻找一个局部最优解，所以lr = 0.03, epoch = 10时的反常是能够解释的。
- lr：
  - 对于batch_size较小时，若epoch较小，小的lr能更快的找到局部最优解，但增大epoch提升不大。而较大的lr虽然在epoch小的时候性能可能不如使用小lr的模型，但epoch增大后往往能找到更好的局部最优解。
- epoch：
  - 当batch_size较大，lr较大，需要相应增大epoch，以保证收敛到局部最优解。
  - 对于batch_size = 256来说，15个epoch显然是不够的。但在本报告中不多赘述。
- **通过观察，可以得知batch_size = 128，lr = 0.03，epoch = 15时最优。**



##### （3）选取最优参数进行模型的横向比较

​		通过对比，我们选取**batch_size = 128，lr = 0.03，epoch = 15**的参数进行不同模型的比较。

| 数值\模型 |                            Net_1                             |                            Net_2                             |                            Net_3                             |
| :-------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|   LOSS    | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127171508959.png" alt="image-20221127171508959" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127182932333.png" alt="image-20221127182932333" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127183237094.png" alt="image-20221127183237094" style="zoom:33%;" /> |
|    ACC    |                           0.922500                           |                           0.971400                           |                           0.981800                           |
| TEST LOSS |                           0.271662                           |                           0.093033                           |                           0.058293                           |

- 可以看见，经过了激活函数后，LOSS趋于稳定；而增加了batch normalization后，稳定性进一步增加，LOSS收敛得更加彻底，并且模型的泛化性也很好，没有出现过拟合的情况。



##### （4）在Net_3的基础上，更换不同的损失函数进行比较

|           | CrossEntropyLoss                                             | amsoftmax                                                    | armsoftmax                                                   |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| LOSS      | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127183237094.png" alt="image-20221127183237094" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127193447183.png" alt="image-20221127193447183" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127202844350.png" alt="image-20221127202844350" style="zoom:33%;" /> |
| ACC       | 0.981800                                                     | 0.762422                                                     | 0.759297                                                     |
| TEST LOSS | 0.058293                                                     | 0.002325                                                     | 0.002240                                                     |

###### ① 简要介绍

​		由于笔者最近在THU-cslt实验室实习进行的是说话人识别的相关研究，用到了amsoftmax与armsoftmax函数，它们与softmax函数不同，是**缩小类内距，增大类间距的策略**。Softmax能做到的只能是划分类别间的界线——绿色虚线，而AMSoftmax可以缩小类内距增大类间距，将类的区间缩小到Target region范围，同时又会产生margin大小的类间距：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127203411484.png" alt="image-20221127203411484" style="zoom: 50%;" />

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127203726679.png" alt="image-20221127203726679" style="zoom: 33%;" />		

其公式为：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127203523597.png" alt="image-20221127203523597" style="zoom:67%;" />



**笔者有预期，效果并不会太好：**因为手写数字识别只强调类间的区别，也就是说，只要区别出来就可以。单纯的softmax就可以很好的做到这点。**但AMsoftmax由于兼顾二者，可能会削弱实际分类识别的效果。**结果也符合我的预料。

本次采用的参数为：**m = 0.2，scale = 30**.



##### （5）在Net_3，CrossEntropyLoss且为最优参数的基础上尝试不同的优化算法

|           | MBGD                                                         | Adam                                                         | Adagrad                                                      | Adamax                                                       | ASGD                                                         | **Rprop**                                                    |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| LOSS      | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127183237094.png" alt="image-20221127183237094" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127210133661.png" alt="image-20221127210133661" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127205757031.png" alt="image-20221127205757031" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127210645965.png" alt="image-20221127210645965" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127211509843.png" alt="image-20221127211509843" style="zoom:33%;" /> | <img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221127212357627.png" alt="image-20221127212357627" style="zoom:33%;" /> |
| ACC       | 0.981800                                                     | 0.980400                                                     | 0.984900                                                     | 0.980500                                                     | 0.982100                                                     | 0.930000                                                     |
| TEST LOSS | 0.058293                                                     | 0.089257                                                     | 0.056550                                                     | 0.092219                                                     | 0.058844                                                     | 4.178316                                                     |

- 可以看到，虽然最后除了Rprop外ACC差别并不是很大（Rprop不适合小batch，基本没收敛），但是TEST LOSS相差还是较大的。而在训练时的LOSS稳定性比较，Adam和Adamax的表现较差，出现了比较不稳定的状况，尤其是Adam及其TEST LOSS.



### 四、总结

本次实验，我了解、尝试了：

- 基本图像分类流程和数据驱动方法（训练/预测阶段）
- 数据集分割以及验证数据在超参数调整中的使用
- 实施并应用Softmax分类器
- 实施并应用完全连接的神经网络分类器
- 了解这些分类器之间的差异和权衡，实施用于优化神经网络的各种更新规则
- 尝试不同的损失函数和正则化
- 尝试不同的优化算法

对网络训练过程加深了理解，受益匪浅。

