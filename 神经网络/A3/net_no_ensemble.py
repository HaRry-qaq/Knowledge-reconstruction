import torch
from torchvision import transforms
from torchvision import datasets
from torch.utils.data import DataLoader
import torch.nn.functional as F
import torch.optim as optim
import matplotlib.pyplot as plt
import numpy as np
from sklearn.model_selection import KFold
 


criterion = torch.nn.CrossEntropyLoss()
epoch_num = 20

# 准备数据集
batch_size = 32
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])
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

 
class Net(torch.nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = torch.nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = torch.nn.Conv2d(10, 20, kernel_size=5)
        self.pooling = torch.nn.MaxPool2d(2)
        self.fc = torch.nn.Linear(320, 10)
        self.dropout = torch.nn.Dropout(p=0.1)
        self.batch_norm1 = torch.nn.BatchNorm2d(num_features = 10, eps = 1e-05)
        self.batch_norm2 = torch.nn.BatchNorm2d(num_features = 20, eps = 1e-05)
 
    def forward(self, x, is_train):
        batch_size = x.size(0)
        x = F.relu(self.batch_norm1(self.pooling(self.conv1(x))))
        x = F.relu(self.batch_norm2(self.pooling(self.conv2(x))))
        x = x.view(batch_size, -1)
        if is_train == True:
            x = self.dropout(x)
        x = self.fc(x)
        return x
 
class LeNet(torch.nn.Module):
   def __init__(self):
       super(LeNet, self).__init__()

       self.conv1 = torch.nn.Sequential(torch.nn.Conv2d(1, 6, 3, 1, 2), torch.nn.ReLU(),
                                  torch.nn.MaxPool2d(2, 2))

       self.conv2 = torch.nn.Sequential(torch.nn.Conv2d(6, 16, 5), torch.nn.ReLU(),
                                  torch.nn.MaxPool2d(2, 2))

       self.fc1 = torch.nn.Sequential(torch.nn.Linear(16 * 5 * 5, 120),
                                torch.nn.BatchNorm1d(120), torch.nn.ReLU())

       self.fc2 = torch.nn.Sequential(
           torch.nn.Linear(120, 84),
           torch.nn.BatchNorm1d(84),
           torch.nn.ReLU(),
           torch.nn.Linear(84, 10))

   def forward(self, x):
       x = self.conv1(x)
       x = self.conv2(x)
       x = x.view(x.size()[0], -1)
      #x.view(x.size(0), -1)这句话是说将第二次卷积的输出拉伸为一行，接下来就是全连接层
       x = self.fc1(x)
       x = self.fc2(x)
       return x 





def train(epoch, train_loader):
    global total_batch
    running_loss = 0.0
    for batch_idx, data in enumerate(train_loader, 0):
        inputs, target = data
        inputs,target=inputs.to(device),target.to(device)
        # print(device)
        optimizer.zero_grad()
 
        outputs = model(inputs, True)
        loss = criterion(outputs, target)
        loss.backward()
        optimizer.step()
        total_batch += 1
        
 
        # running_loss += loss.item()
        if batch_idx % 50 == 49:
            x.append(total_batch)
            y.append(loss.item())
            print('[epoch:%d, batch_idx: %d] loss:%f' % (epoch + 1, batch_idx + 1, loss.item()))
            # running_loss = 0.0
        

avg_acc = 0     
 
def test(test_loader, epoch):
    global avg_acc
    correct=0
    total=0
    loss=0
    count_num=0
    with torch.no_grad():
        for data in test_loader:
            count_num += 1
            inputs,target=data
            inputs,target=inputs.to(device),target.to(device)
            outputs=model(inputs, False)
            _,predicted=torch.max(outputs.data,dim=1)
            total+=target.size(0)
            correct+=(predicted==target).sum().item()
            loss += criterion(outputs, target).item()
            # print(loss)
    print('Accuracy on test set:%f%% [%d %d]' %(100*correct/total,correct,total))
    print('test loss: %f' %(loss / count_num))
    if (epoch + 1) % epoch_num == 0:
        file = open('data.txt','a')
        file.write('Accuracy: %f ' %(100*correct/total))
        file.write('test loss: %f' %(loss / count_num))
        file.write('\n')
        file.close()
        avg_acc += 100*correct/total
 

def paint(count, x, y):
    plt.clf()
    iter_list = np.array(x)
    loss_list = np.array(y)
    plt.plot(iter_list, loss_list, label = 'k%d'%(count),  ls = 'solid')
    plt.xlabel('batch')
    plt.ylabel('loss')
    plt.title('loss')
    plt.legend()
    plt.savefig('k_5_loss_%d.jpg' %(count))
 


def weight_init(m):
    if isinstance(m, torch.nn.Linear):
        torch.nn.init.xavier_normal_(m.weight)
        torch.nn.init.constant_(m.bias, 0)

    elif isinstance(m, torch.nn.Conv2d):
        torch.nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')

    elif isinstance(m, torch.nn.BatchNorm2d):
        torch.nn.init.constant_(m.weight, 1)
        torch.nn.init.constant_(m.bias, 0)




if __name__ =='__main__':
    kf = KFold(n_splits=5 ,shuffle=True, random_state=0)  # init KFold
    count = 1
    for train_index , test_index in kf.split(dataFold):  # split 
        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        print(device)
        total_batch = 0 
        x = [] #batch
        y = [] #loss
        
        model = Net()
        model.apply(weight_init)

        model.to(device)
        optimizer = optim.SGD(model.parameters(), lr=0.03, momentum=0.5 ,weight_decay=1e-4)
        
        

        
        print("n_splits_5 count = %d" %(count))
        # get train, val 根据索引划分
        train_fold = torch.utils.data.dataset.Subset(dataFold, train_index)
        test_fold = torch.utils.data.dataset.Subset(dataFold, test_index) 
 
        # package type of DataLoader
        train_loader = torch.utils.data.DataLoader(dataset=train_fold, batch_size=batch_size, shuffle=True)
        test_loader = torch.utils.data.DataLoader(dataset=test_fold, batch_size=batch_size, shuffle=True)

        for epoch in range(epoch_num):
            train(epoch, train_loader)
            test(test_loader, epoch)

        print(len(x))
        paint(count, x, y)
        count += 1

    avg_acc = avg_acc / 5
    file = open('data.txt','a')
    file.write('avg acc: %f' %(avg_acc))
    file.write('\n')
    

