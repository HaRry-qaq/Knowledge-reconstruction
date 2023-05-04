#导入库
import matplotlib.pyplot as plt
import numpy as np
#设定画布。dpi越大图越清晰，绘图时间越久
fig=plt.figure(figsize=(4, 4), dpi=300)
#导入数据
x=[1,5,10,100,1000,3000,5000,7000,9000]
y=[0.8782,0.92764,0.9512,0.98436,0.9968,0.99859,1,1,1]
#绘图命令
plt.plot(x, y, lw=1, ls='-', c='b', alpha=0.5)
plt.plot()
#show出图形
plt.show()
#保存图片
fig.savefig("pic")
