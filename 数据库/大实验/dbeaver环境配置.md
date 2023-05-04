## 环境配置

### 一、使用Dbeaver连接GaussDB数据库实例

#### 1、下载Dbeaver

下载链接：https://dbeaver.io/files/dbeaver-ce-latest-x86_64-setup.exe

#### 2、下载依赖包

下载链接：

https://opengauss.obs.cn-south-1.myhuaweicloud.com/3.1.0/x86_openEuler/openGauss-3.1.0-J

DBC.tar.gz

#### 3、添加JDBC驱动

- 驱动名称：openGauss
- 类名：org.postgresql.Driver
- URL 模版：jdbc:postgresql://{host}:{port}/{database}

#### 4、Dbeaver连接

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221205112926923.png" alt="image-20221205112926923" style="zoom:67%;" />

连接成功后如下所示：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221205115510751.png" alt="image-20221205115510751" style="zoom:50%;" />



#### 5、建表并导入

- 把 02-2 实验指导书上的建表 SQL 语句复制进去，点击运行

- 右键需要导入的表格，选择导入数据，找到要导入的 csv 文件，把标题位置改成“

  none”, 其他选项默认。等待一段时间即可。

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221205115803181.png" alt="image-20221205115803181" style="zoom:50%;" />

导入完成后nation表如下：

<img src="C:\Users\HaRry_\AppData\Roaming\Typora\typora-user-images\image-20221205115731299.png" alt="image-20221205115731299" style="zoom:50%;" />

