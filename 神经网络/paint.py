import matplotlib.pyplot as plt
from matplotlib.pyplot import MultipleLocator
k_list = []
score_list = []
time_list = []

file = open('time.txt','r')
for i in range(600):
    s = file.readline()
    data = s.split('   ')
    k = data[0].split(':')[1]
    score = data[1].split(':')[1]
    score = float(score)
    time = data[2].split(':')[1]
    time = float(time)
    k_list.append(k)
    score_list.append(score)
    time_list.append(time)
# print(score_list)
plt.scatter(k_list, time_list,  ls = 'solid', s=6)
plt.xlabel('dimension')
plt.ylabel('time')
plt.xticks([0,99,199,299,399,499,599])
plt.yticks([0,5,10,15,20,25,30,35])
plt.legend()
plt.savefig('dimension_time.jpg')

    
