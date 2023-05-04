## A3: CNN-Handwritten Digits Recognition

姓名：陈俊卉     班级：2020219111      学号：2020212256

[TOC]



### 一、实验要求

• In this assignment you will practice training Neural Networks and Convolutional Networks. The goals of this assignment are as follows:

​	– understand Neural Networks and how they are arranged in layered architectures

​	– implement dropout to regularize networks and explore its effects on model generalization

​	– effectively cross-validate and find the best hyperparameters for Neural Network architecture

​	– understand the architecture of Convolutional Neural Networks and train gain experience with training 		these models on data


• Do something extra!

​	– In the process of training your network, you should feel free to implement anything that you want to get better performance. You can modify the solver, implement additional layers, use different types of regularization, use an ensemble of models, or anything else that comes to mind.  (+5 points) 



### 二、模型简介

#### 1、网络

```python
class Net(torch.nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = torch.nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = torch.nn.Conv2d(10, 20, kernel_size=5)
        self.pooling = torch.nn.MaxPool2d(2)
        self.fc = torch.nn.Linear(320, 10)
        self.dropout = torch.nn.Dropout(p=0.1)
 
    def forward(self, x, is_train):
        batch_size = x.size(0)
        x = F.relu(self.pooling(self.conv1(x)))
        x = F.relu(self.pooling(self.conv2(x)))
        x = x.view(batch_size, -1)
        if is_train == True:
            x = self.dropout(x)
        x = self.fc(x)
        return x
```

我们知道，MNIST数据集图片大小为$28 \times 28$.

- 首先我们使用$5 \times 5$的卷积核，经过卷积后大小为$24 \times 24$，通道数为10；
- 经过$2 \times 2$的最大池化后变为$12 \times 12$，通道数不变；在经过$5 \times 5$的卷积核变为$8 \times 8$，通道数为20；
- 通过第二个$2 \times 2$最大池化后变为$4 \times 4$，通道数不变，为20；
- 将张量转换为$1 \times 320$的向量，输入全连接层，输出维度为10
- 最后在训练时增加dropout层，防止过拟合。
- dropout的作用：
  - dropout掉不同的隐藏神经元就类似在训练不同的网络，随机删掉一半隐藏神经元导致网络结构已经不同，整个dropout过程就相当于对很多个不同的神经网络取平均。而不同的网络产生不同的过拟合，一些互为“反向”的拟合相互抵消就可以达到整体上减少过拟合。
  - dropout导致两个神经元不一定每次都在一个dropout网络中出现。这样权值的更新不再依赖于有固定关系的隐含节点的共同作用，阻止了某些特征仅仅在其它特定特征下才有效果的情况 。迫使网络去学习更加鲁棒的特征 ，这些特征在其它的神经元的随机子集中也存在。其消除减弱了神经元节点间的联合适应性，增强了**泛化能力**。
  - Dropout的出现很好的可以解决这个问题，每次做完dropout，相当于从原始的网络中找到一个更瘦的网络。因而，对于一个有N个节点的神经网络，有了dropout后，就可以看做是2^n个模型的集合了，但此时要训练的参数数目却是不变的，这就解决了费时的问题。



#### 2、数据集

采用k折交叉验证方法（k = 5）：

- 先将pytorch划分好的训练集和验证集合并：

  - ```python
    train_dataset = datasets.MNIST(root='../dataset/mnist/',
                                   train=True,
                                   download=True,
                                   transform=transform)
    
    test_dataset = datasets.MNIST(root='../dataset/mnist',
                                  train=False,
                                  download=True,
                                  transform=transform)
    
    # the dataset for k fold cross validation   
    dataFold = torch.utils.data.ConcatDataset([train_dataset, test_dataset])
    ```

- 再通过 **sklearn.model_selection 的 KFold** 划分数据集

  - k折每次都要重置网络




#### 3、损失函数与优化器

```python
criterion = torch.nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.5)
```

- optimizer的lr、momentum在下文会调整。



### 三、训练测试

**k折交叉验证：k=5**

**固定网络结构，探究超参数batch_size、lr与epoch的最优值。**

- **batch_size [32,64,128]**
- **lr [0.01,0.02,0.03]**
- **epoch [10,20]** **因为epoch=10左右网络差不多已经收敛；先据此决定超参，后续选取优秀超参数进行更多epoch的训练调优。**

train loss的纵坐标每个图是不一样的，所以只用来观察是否稳定收敛，不做横向对比。

#### batch_size = 32  lr = 0.01  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129005050490](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129005050490.png) | ![image-20221129005059064](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129005059064.png) | ![image-20221129005106446](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129005106446.png) | ![image-20221129005114421](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129005114421.png) | ![image-20221129005120245](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129005120245.png) |
|   ACC(%)   |                          98.635714                           |                          98.821429                           |                          98.642857                           |                          98.800000                           |                          98.892857                           |
| TEST LOSS  |                           0.044129                           |                           0.043093                           |                           0.041334                           |                           0.039321                           |                           0.034918                           |

**Avg ACC：98.7585714%**



#### batch_size = 32  lr = 0.02  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129011718633](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129011718633.png) | ![image-20221129011725032](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129011725032.png) | ![image-20221129011731351](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129011731351.png) | ![image-20221129011736506](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129011736506.png) | ![image-20221129011743087](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129011743087.png) |
|   ACC(%)   |                          98.871429                           |                          98.900000                           |                          99.000000                           |                          98.857143                           |                          98.850000                           |
| TEST LOSS  |                           0.038033                           |                           0.034293                           |                           0.030721                           |                           0.037891                           |                           0.035316                           |

**Avg ACC：98.8957144%**



#### batch_size = 32  lr = 0.03  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129012752031](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129012752031.png) | ![image-20221129012833631](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129012833631.png) | ![image-20221129013024961](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129013024961.png) | ![image-20221129013727774](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129013727774.png) | ![image-20221129013734605](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129013734605.png) |
|   ACC(%)   |                          98.814286                           |                          99.042857                           |                          98.992857                           |                          98.850000                           |                          99.157143                           |
| TEST LOSS  |                           0.042122                           |                           0.032760                           |                           0.035750                           |                           0.039034                           |                           0.028440                           |

**Avg ACC：98.9714286%**

**观察可知，lr = 0.03相对较大，导致train loss不太稳定（虽然大多也只是在0到0.3之间波动）**



#### batch_size = 64  lr = 0.01  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129020606284](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129020606284.png) | ![image-20221129020611925](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129020611925.png) | ![image-20221129020619149](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129020619149.png) | ![image-20221129020628582](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129020628582.png) | ![image-20221129020841317](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129020841317.png) |
|   ACC(%)   |                          98.550000                           |                          98.492857                           |                          98.500000                           |                          98.678571                           |                          98.592857                           |
| TEST LOSS  |                           0.047099                           |                           0.049402                           |                           0.045863                           |                           0.042746                           |                           0.047693                           |

**Avg ACC：98.562857%**



#### batch_size = 64  lr = 0.02  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129022440201](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129022440201.png) | ![image-20221129022446188](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129022446188.png) | ![image-20221129022452808](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129022452808.png) | ![image-20221129022459581](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129022459581.png) | ![image-20221129022510910](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129022510910.png) |
|   ACC(%)   |                          98.671429                           |                          98.771429                           |                          98.921429                           |                          98.950000                           |                          98.792857                           |
| TEST LOSS  |                           0.044117                           |                           0.040766                           |                           0.035834                           |                           0.035158                           |                           0.036793                           |

**Avg ACC：98.821429%**



#### batch_size = 64  lr = 0.03  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129024244144](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129024244144.png) | ![image-20221129024250986](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129024250986.png) | ![image-20221129024257880](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129024257880.png) | ![image-20221129024305131](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129024305131.png) | ![image-20221129024318480](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129024318480.png) |
|   ACC(%)   |                          98.771429                           |                          98.721429                           |                          98.942857                           |                          98.900000                           |                          99.057143                           |
| TEST LOSS  |                           0.038273                           |                           0.039294                           |                           0.033551                           |                           0.034737                           |                           0.031825                           |

**Avg ACC：98.878571%**



#### batch_size = 128  lr = 0.01  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129025931747](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129025931747.png) | ![image-20221129025937725](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129025937725.png) | ![image-20221129025944632](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129025944632.png) | ![image-20221129025950746](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129025950746.png) | ![image-20221129025958455](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129025958455.png) |
|   ACC(%)   |                          98.171429                           |                          98.085714                           |                          98.364286                           |                          98.128571                           |                          98.185714                           |
| TEST LOSS  |                           0.064314                           |                           0.061963                           |                           0.052101                           |                           0.057939                           |                           0.059497                           |

**Avg ACC：98.187143%**



#### batch_size = 128  lr = 0.02  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129031604067](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129031604067.png) | ![image-20221129031610211](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129031610211.png) | ![image-20221129031615915](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129031615915.png) | ![image-20221129031622880](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129031622880.png) | ![image-20221129031629637](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129031629637.png) |
|   ACC(%)   |                          98.428571                           |                          98.585714                           |                          98.750000                           |                          98.678571                           |                          98.842857                           |
| TEST LOSS  |                           0.048384                           |                           0.044895                           |                           0.040372                           |                           0.042612                           |                           0.041946                           |

**Avg ACC：98.657143%**



#### batch_size = 128  lr = 0.03  epoch = 10

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129034656169](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129034656169.png) | ![image-20221129034702078](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129034702078.png) | ![image-20221129034708649](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129034708649.png) | ![image-20221129034716029](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129034716029.png) | ![image-20221129034723272](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129034723272.png) |
|   ACC(%)   |                          98.607143                           |                          98.778571                           |                          98.564286                           |                          98.707143                           |                          98.971429                           |
| TEST LOSS  |                           0.045789                           |                           0.039690                           |                           0.041787                           |                           0.042477                           |                           0.035774                           |

**Avg ACC：98.725714%**



**对于batch_size=128，epoch=10还是有点太小，ACC有小幅下滑。**



#### batch_size = 32  lr = 0.01  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129042610165](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129042610165.png) | ![image-20221129042616825](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129042616825.png) | ![image-20221129042624127](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129042624127.png) | ![image-20221129042630987](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129042630987.png) | ![image-20221129042637755](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129042637755.png) |
|   ACC(%)   |                          98.928571                           |                          98.842857                           |                          99.128571                           |                          98.957143                           |                          99.042857                           |
| TEST LOSS  |                           0.034086                           |                           0.040078                           |                           0.029353                           |                           0.035307                           |                           0.032967                           |

**Avg ACC：98.980000%**



#### batch_size = 32  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129083504241](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129083504241.png) | ![image-20221129083510774](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129083510774.png) | ![image-20221129083519677](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129083519677.png) | ![image-20221129083528559](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129083528559.png) | ![image-20221129083536692](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129083536692.png) |
|   ACC(%)   |                          98.928571                           |                          98.950000                           |                          99.164286                           |                          98.978571                           |                          99.164286                           |
| TEST LOSS  |                           0.042151                           |                           0.040902                           |                           0.033124                           |                           0.038845                           |                           0.030593                           |

**Avg ACC：99.037143%**



#### batch_size = 32  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129091348893](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129091348893.png) | ![image-20221129091355251](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129091355251.png) | ![image-20221129091401782](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129091401782.png) | ![image-20221129091408523](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129091408523.png) | ![image-20221129091420741](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129091420741.png) |
|   ACC(%)   |                          98.942857                           |                          98.857143                           |                          99.164286                           |                          99.014286                           |                          99.185714                           |
| TEST LOSS  |                           0.044837                           |                           0.041812                           |                           0.031553                           |                           0.038445                           |                           0.029851                           |

**Avg ACC：99.032857%**



#### batch_size = 64  lr = 0.01  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129094853305](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129094853305.png) | ![image-20221129094900689](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129094900689.png) | ![image-20221129094907526](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129094907526.png) | ![image-20221129094914528](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129094914528.png) | ![image-20221129094922761](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129094922761.png) |
|   ACC(%)   |                          98.878571                           |                          98.857143                           |                          98.871429                           |                          98.978571                           |                          98.835714                           |
| TEST LOSS  |                           0.039972                           |                           0.038280                           |                           0.037599                           |                           0.033899                           |                           0.035644                           |

**Avg ACC：98.884286%**



#### batch_size = 64  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129115949490](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129115949490.png) | ![image-20221129115958514](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129115958514.png) | ![image-20221129120007173](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129120007173.png) | ![image-20221129120015087](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129120015087.png) | ![image-20221129120024439](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129120024439.png) |
|   ACC(%)   |                          98.928571                           |                          98.964286                           |                          98.957143                           |                          99.071429                           |                          99.164286                           |
| TEST LOSS  |                           0.036814                           |                           0.033534                           |                           0.034467                           |                           0.033806                           |                           0.027379                           |

**Avg ACC：99.017143%**



#### batch_size = 64  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129123950576](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129123950576.png) | ![image-20221129124001851](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129124001851.png) | ![image-20221129124009426](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129124009426.png) | ![image-20221129124016029](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129124016029.png) | ![image-20221129124024391](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129124024391.png) |
|   ACC(%)   |                          98.964286                           |                          98.985714                           |                          99.028571                           |                          99.071429                           |                          98.964286                           |
| TEST LOSS  |                           0.039792                           |                           0.034142                           |                           0.039206                           |                           0.031763                           |                           0.035137                           |

**Avg ACC：99.002857%**



#### batch_size = 128  lr = 0.01  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129131235077](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129131235077.png) | ![image-20221129131241187](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129131241187.png) | ![image-20221129131247921](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129131247921.png) | ![image-20221129131254657](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129131254657.png) | ![image-20221129131303595](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129131303595.png) |
|   ACC(%)   |                          98.614286                           |                          98.607143                           |                          98.728571                           |                          98.757143                           |                          98.750000                           |
| TEST LOSS  |                           0.046682                           |                           0.047169                           |                           0.040424                           |                           0.039409                           |                           0.042777                           |

**Avg ACC：98.691429%**



#### batch_size = 128  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129141225093](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129141225093.png) | ![image-20221129141231505](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129141231505.png) | ![image-20221129141238338](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129141238338.png) | ![image-20221129141246305](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129141246305.png) | ![image-20221129141252563](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129141252563.png) |
|   ACC(%)   |                          98.878571                           |                          98.778571                           |                          98.942857                           |                          98.950000                           |                          98.928571                           |
| TEST LOSS  |                           0.036354                           |                           0.042747                           |                           0.035724                           |                           0.036140                           |                           0.034402                           |

**Avg ACC：98.895714%**



#### batch_size = 128  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129140843704](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129140843704.png) | ![image-20221129140852713](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129140852713.png) | ![image-20221129140859556](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129140859556.png) | ![image-20221129140906387](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129140906387.png) | ![image-20221129140916354](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129140916354.png) |
|   ACC(%)   |                          98.828571                           |                          98.957143                           |                          98.971429                           |                          99.071429                           |                          99.014286                           |
| TEST LOSS  |                           0.038084                           |                           0.035694                           |                           0.034270                           |                           0.031475                           |                           0.033374                           |

**Avg ACC：98.968571%**



### 四、继续设法提升ACC

选取ACC超过99%的模型：

- batch_size = 32  lr = 0.02  epoch = 20
- batch_size = 32  lr = 0.03  epoch = 20
- batch_size = 64  lr = 0.02  epoch = 20
- batch_size = 64  lr = 0.03  epoch = 20



#### 1、在激活层之前增加batch_normalization

#### batch_size = 32  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129150629723](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150629723.png) | ![image-20221129150636336](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150636336.png) | ![image-20221129150642543](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150642543.png) | ![image-20221129150648745](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150648745.png) | ![image-20221129150655364](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150655364.png) |
|   ACC(%)   |                          99.021429                           |                          98.957143                           |                          99.007143                           |                          98.985714                           |                          99.135714                           |
| TEST LOSS  |                           0.033903                           |                           0.033399                           |                           0.030595                           |                           0.032947                           |                           0.030287                           |

**Avg ACC：98.968571%**



#### batch_size = 32  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129150901338](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150901338.png) | ![image-20221129150908970](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150908970.png) | ![image-20221129150915570](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150915570.png) | ![image-20221129150921556](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150921556.png) | ![image-20221129150926440](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129150926440.png) |
|   ACC(%)   |                          99.014286                           |                          99.028571                           |                          99.157143                           |                          99.071429                           |                          99.021429                           |
| TEST LOSS  |                           0.034609                           |                           0.032913                           |                           0.031359                           |                           0.034433                           |                           0.032309                           |

**Avg ACC：99.058571%**



#### batch_size = 64  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129154421069](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129154421069.png) | ![image-20221129154428150](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129154428150.png) | ![image-20221129154436182](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129154436182.png) | ![image-20221129154442084](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129154442084.png) | ![image-20221129154449597](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129154449597.png) |
|   ACC(%)   |                          99.100000                           |                          98.964286                           |                          99.028571                           |                          98.928571                           |                          99.028571                           |
| TEST LOSS  |                           0.032574                           |                           0.035042                           |                           0.031297                           |                           0.033718                           |                           0.030124                           |

**Avg ACC：99.010000%**



#### batch_size = 64  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129153350150](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129153350150.png) | ![image-20221129153356564](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129153356564.png) | ![image-20221129153402313](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129153402313.png) | ![image-20221129153408799](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129153408799.png) | ![image-20221129153415916](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129153415916.png) |
|   ACC(%)   |                          98.985714                           |                          98.971429                           |                          99.007143                           |                          98.957143                           |                          99.200000                           |
| TEST LOSS  |                           0.033515                           |                           0.033941                           |                           0.030266                           |                           0.035768                           |                           0.029665                           |

**Avg ACC：99.024286%**



**去除不到99%的组。**



#### 2、增加参数初始化和weight_decay（多次调整得到最优参数）

```python
def weight_init(m):
    if isinstance(m, torch.nn.Linear):
        torch.nn.init.xavier_normal_(m.weight)
        torch.nn.init.constant_(m.bias, 0)

    elif isinstance(m, torch.nn.Conv2d):
        torch.nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')

    elif isinstance(m, torch.nn.BatchNorm2d):
        torch.nn.init.constant_(m.weight, 1)
        torch.nn.init.constant_(m.bias, 0)
```

```
optimizer = optim.SGD(model.parameters(), lr=0.03, momentum=0.5 ,weight_decay=1e-4)
```



#### batch_size = 32  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129162619865](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129162619865.png) | ![image-20221129162630551](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129162630551.png) | ![image-20221129162636619](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129162636619.png) | ![image-20221129162644644](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129162644644.png) | ![image-20221129162652669](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129162652669.png) |
|   ACC(%)   |                          99.078571                           |                          99.071429                           |                          99.171429                           |                          99.057143                           |                          99.171429                           |
| TEST LOSS  |                           0.032598                           |                           0.030385                           |                           0.030090                           |                           0.031636                           |                           0.027563                           |

**Avg ACC：99.110000%**



#### batch_size = 64  lr = 0.02  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129161129436](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129161129436.png) | ![image-20221129161134827](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129161134827.png) | ![image-20221129161140623](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129161140623.png) | ![image-20221129161146731](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129161146731.png) | ![image-20221129161152916](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129161152916.png) |
|   ACC(%)   |                          98.942857                           |                          99.050000                           |                          99.021429                           |                          99.007143                           |                          99.064286                           |
| TEST LOSS  |                           0.034962                           |                           0.031964                           |                           0.030429                           |                           0.030575                           |                           0.030251                           |

**Avg ACC：99.017143%**



#### batch_size = 64  lr = 0.03  epoch = 20

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129163502855](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129163502855.png) | ![image-20221129163509245](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129163509245.png) | ![image-20221129163515226](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129163515226.png) | ![image-20221129163522131](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129163522131.png) | ![image-20221129163528144](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129163528144.png) |
|   ACC(%)   |                          99.042857                           |                          99.035714                           |                          99.007143                           |                          98.928571                           |                          99.121429                           |
| TEST LOSS  |                           0.033805                           |                           0.030509                           |                           0.030533                           |                           0.034823                           |                           0.028515                           |

**Avg ACC：99.027143%**



**只保留batch_size = 32  lr = 0.03  epoch = 20，继续优化。**

#### 3、使用L1正则化（预期效果会差）

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129183457181](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129183457181.png) | ![image-20221129183502862](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129183502862.png) | ![image-20221129183508975](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129183508975.png) | ![image-20221129183514904](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129183514904.png) | ![image-20221129183521349](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129183521349.png) |
|   ACC(%)   |                          99.021429                           |                          99.000000                           |                          99.078571                           |                          99.028571                           |                          99.107143                           |
| TEST LOSS  |                           0.033516                           |                           0.032485                           |                           0.030003                           |                           0.030038                           |                           0.027805                           |

**Avg ACC：99.047143%**





#### 4、使用ensemble learning（bagging）

**使用多个类似而不同的CNN网络进行预测。增加了epoch，为了节约运算资源加快速度，test loss没有进行记录。**

**这些CNN网络的超参数都预先调出了Avg ACC在99.1%左右的性能。**

增大epoch以充分收敛，**选取epoch对应的ckpt进行测试**，最优结果为：

##### batch_size = 64  lr = 0.03  epoch = 50 

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129184824037](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129184824037.png) | ![image-20221129184834733](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129184834733.png) | ![image-20221129184843812](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129184843812.png) | ![image-20221129184851111](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129184851111.png) | ![image-20221129185508970](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129185508970.png) |
|   ACC(%)   |                          99.242857                           |                          99.235714                           |                          99.250000                           |                          99.292857                           |                          99.321429                           |

**Avg ACC：99.2685714%**



#### 5、在4的基础上使用seed(3407)并尝试SGD、Adam

并微调参数，性能相比之前提高幅度较大！但Adam的ACC不如SGD，故没有将结果放出。

增大epoch以充分收敛，**选取epoch对应的ckpt进行测试**，最优结果为：

##### batch_size = 64  lr = 0.03  epoch = 50  torch.cuda.manual_seed(3407)

|            |                             kf_1                             |                             kf_2                             |                             kf_3                             |                             kf_4                             |                             kf_5                             |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| TRAIN LOSS | ![image-20221129231810769](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129231810769.png) | ![image-20221129231816770](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129231816770.png) | ![image-20221129231823156](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129231823156.png) | ![image-20221129231829243](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129231829243.png) | ![image-20221129231837403](C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221129231837403.png) |
|   ACC(%)   |                          99.371429                           |                          99.307143                           |                          99.314286                           |                          99.328571                           |                          99.300000                           |

**Avg ACC：99.3242858%**



#### 6、调节momentum

没有优秀的效果，还是原来的参数最好，故不在此展示。



### 五、总结

#### 通过本次实验，我做到了：

- 了解神经网络及其分层架构的排列方式，并自己实现网络

- 实施了dropout以规范网络，并探讨了其对模型推广的影响

- 有效地使用了k折交叉验证，并通过十分充分的实验找到了神经网络架构的最佳超参数

- 更加深刻认识到神经网络的训练效果与很多因素都息息相关。甚至与随机种子也相关

- 使用了以下方法试图找到最佳网络：**batch_size = 32  lr = 0.03 seed(3407)  epoch=60 参数初始化的ensemble learning**

  - 遍历合适的学习率、batchsize和epoch

  - 在激活层前添加batch normalization

  - 调节参数初始化和weight_decay

  - 使用L1正则化

  - 使用ensemble learning

  - 设置随机种子

  - 调节momentum
  
    除了调节momentum、L1正则化，均起到了较好的效果。

#### 训练过程中学到的地方：

- 一个重要的习惯：在数个epoch结束之后验证一次，以实时关注网络的变化，判断过拟合或欠拟合，抑或是无法跳出当前局部最优解，以及时调节对应的参数。
- 可以先使用较小的数据集、小epoch找出比较合适的超参，再使用大数据集、大epoch训练。
- 使用不同的初始化种子可能会有出其不意的好效果，但需要综合调参判断。
- ensemble learning本身虽然已经能够一定程度上提高实验的准确性，但如果要追求高性能，可以考虑先分别将每一个模型的参数调到较为优秀的程度，再通过ensemble learning试图达到准确率的新高。
- dropout虽然能够一定程度上起到效果，但这是建立在模型已经充分收敛，存在过拟合的迹象的时候。此外还取决于dropout的大小，若设置的太大，模型难以收敛。
- 正则化理论上一般能够调节解空间的光滑性，所以一般加上为宜。







