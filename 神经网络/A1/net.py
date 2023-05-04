from torch import nn
# import amsoftmax
import importlib

#-------------------------------------自定义模型--------------------------------------
#定义了三个不层次的神经网络模型：简单的FC，加激活函数的FC，加激活函数和批标准化的FC。
#1. 全连接网络层
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
# 2. 全连接网络层 +激活层
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

# 3. 全连接网络层 + 激活层 + BN 网络层
class net_3(nn.Module):
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super(net_3, self).__init__()
        self.layer1 = nn.Sequential(nn.Linear(in_dim, n_hidden_1), nn.BatchNorm1d(n_hidden_1), nn.ReLU(True))
        self.layer2 = nn.Sequential(nn.Linear(n_hidden_1, n_hidden_2), nn.BatchNorm1d(n_hidden_2), nn.ReLU(True))
        self.layer3 = nn.Sequential(nn.Linear(n_hidden_2, out_dim))
        # self.softmax = nn.Softmax()
        

    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        # x = self.softmax(x)
        return x

import torch
from torch import nn, optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms

#-------------------------------------超参数定义-------------------------------------
batch_size = 128 #一个batch的size
learning_rate = 0.03
num_epoches = 15 #总样本的迭代次数

#-------------------------------------数据预处理方法--------------------------------------
# transforms.ToTensor()将图片转换成PyTorch中处理的对象Tensor,并且进行标准化（数据在0~1之间）
# transforms.Normalize()做归一化。它进行了减均值，再除以标准差。两个参数分别是均值和标准差
# transforms.Compose()函数则是将各种预处理的操作组合到了一起
data_tf = transforms.Compose(
    [transforms.ToTensor(),
     transforms.Normalize([0.5], [0.5])])

#-------------------------------------数据集的下载器--------------------------------------
#训练和测试集预处理
train_dataset = datasets.MNIST(root='./data', train=True, transform=data_tf, download=True)
test_dataset = datasets.MNIST(root='./data', train=False, transform=data_tf)
#加载数据集
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

#-------------------------------------选择模型--------------------------------------
'''
1. 输入28*28(因为输入的图像像素为28*28)
2. 隐藏层分别为300和100
3. 输出层为10，因为识别的数字为0~9
'''
#下列3个模型可以任选其中之一
model = net_3(28 * 28, 512, 128, 10) 
# model = net.Activation_Net(28 * 28, 300, 100, 10)
# model = net.Batch_Net(28 * 28, 300, 100, 10)
if torch.cuda.is_available():
    model = model.cuda()

#-------------------------------------定义损失函数和优化器--------------------------------------

criterion = nn.CrossEntropyLoss() #softmax与交叉熵一起
# LossFunction = importlib.import_module('armsoftmax').__getattribute__('LossFunction')
# criterion = LossFunction()
optimizer = optim.Rprop(model.parameters(), lr=learning_rate)

#-------------------------------------开始训练-------------------------------------
iter_list = []
loss_list = []
print('Start Training!')
iter = 0 #迭代次数
for epoch in range(num_epoches):
    for data in train_loader:
        img, label = data
        img = img.view(img.size(0), -1)
        if torch.cuda.is_available():
            img = img.cuda()
            label = label.cuda()
        else:
            img = img
            label = label
        out = model(img)


        loss = criterion(out, label)
        
        # amsoftmax
        # out = out.reshape(-1, 1, 10).cuda()
        # loss = criterion(out, label)[0]

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        iter+=1

        

        #每迭代50次打印一次
        if iter%50 == 0:
            # 更新list
            iter_list.append(iter)
            loss_list.append(loss.data.item())
            print('epoch: {}, iter:{}, loss: {:.4}'.format(epoch, iter, loss.data.item()))


import numpy as np
import matplotlib.pyplot as plt

# 绘制loss曲线
iter_list = np.array(iter_list)
loss_list = np.array(loss_list)

plt.plot(iter_list, loss_list, label = 'loss', ls = 'solid')
plt.xlabel('iter')
plt.ylabel('loss')
plt.title("loss")
plt.legend()
plt.savefig('loss_net1.jpg')








#-------------------------------------模型评估-------------------------------------
print('Start eval!')
model.eval()
eval_loss = 0
eval_acc = 0
for data in test_loader:
    img, label = data
    img = img.view(img.size(0), -1)
    if torch.cuda.is_available():
        img = img.cuda()
        label = label.cuda()

    out = model(img)

    loss = criterion(out, label)
    eval_loss += loss.data.item()*label.size(0)
    _, pred = torch.max(out, 1) #onehout编码，dim=1选取最大值
    num_correct = (pred == label).sum()
    eval_acc += num_correct.item()

    # amsoftmax
    # out = out.reshape(-1, 1, 10).cuda()
    # print(criterion(out, label)[0])
    # print(criterion(out, label)[1])
    # eval_loss += criterion(out, label)[0].item()
    # eval_acc += criterion(out, label)[1].item()

    






print('Test Loss: {:.6f}, Acc: {:.6f}'.format(eval_loss / (len(test_dataset)), eval_acc / (len(test_dataset))))



