import numpy as np
import time
from sklearn.datasets import fetch_openml
import matplotlib.pyplot as plt
from scipy.io import loadmat
from sklearn.decomposition import PCA
from sklearn.neighbors import KNeighborsClassifier

mnist = loadmat('C://Users/HaRry_/Desktop/神经网络/A4/mnist-original.mat')


X = mnist['data'].T#这个一定要转置一下，因为这里面的行列是反的！！！！！
y = mnist['label'].T.flatten()#将数据展开
# y= y.astype(np.uint8)#将格式变为uint8


X_train = np.array(X[:60000], dtype=float)
y_train = np.array(y[:60000], dtype=float)
X_test = np.array(X[60000:], dtype=float)
y_test = np.array(y[60000:], dtype=float)


# 绘图
# pca = PCA(n_components=2)
# print(X.shape)
# X = pca.fit_transform(X)
# print(X.shape)
# print(sum(pca.explained_variance_ratio_))
# x1 = X[:, 0]
# x2 = X[:, 1]

# scatter = plt.scatter(x1, x2, c=y, alpha=1, s=0.1)
# plt.legend(*scatter.legend_elements())
# plt.savefig('result.jpg')


# #使用kNN


# print("start1")
# start = time.time() #使用time模块计时
# knn_clf1 = KNeighborsClassifier()
# knn_clf1.fit(X_train, y_train)
# print("dimension=784 score:", knn_clf1.score(X_test, y_test))
# end = time.time()
# print('Time: %.2f' % (end - start))

# # #使用PCA进行降维

# pca = PCA(n_components=2)
# X_pca = pca.fit(X_train)
# # print(pca.explained_variance_ratio_)
# X_train_reduction = pca.transform(X_train)
# X_test_reduction = pca.transform(X_test)

# start = time.time()
# knn_clf2 = KNeighborsClassifier()
# knn_clf2.fit(X_train_reduction, y_train)
# print("dimension=2 score:",knn_clf2.score(X_test_reduction, y_test))
# end = time.time()
# print('Time: %.3f' % (end - start))


# pca = PCA(n_components=X_train.shape[1])
# pca.fit(X_train)
# print(pca.explained_variance_ratio_)
# print(sum(pca.explained_variance_ratio_))
# plt.plot([i for i in range(X_train.shape[1])],
#     [np.sum(pca.explained_variance_ratio_[:i+1]) for i in range(X_train.shape[1])])
# plt.show()







x_list = []
y_list = []

for i in range(1,785):
    print(i)
    x_list.append(i)
    
    pca = PCA(n_components=i)
    pca.fit(X_train)
    X_train_reduction = pca.transform(X_train)
    X_test_reduction = pca.transform(X_test)
    start = time.time()
    knn_clf = KNeighborsClassifier()
    knn_clf.fit(X_train_reduction, y_train)
    score = knn_clf.score(X_test_reduction, y_test)
    y_list.append(score)
    end = time.time()
    file = open('time.txt','a')
    file.write('k:%d   score:%f   time:%f' %(i, score, end-start))
    file.write('\n')
    # print(knn_clf.score(X_test_reduction, y_test))

plt.plot(x_list, y_list)
plt.show()