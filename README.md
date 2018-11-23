# TDOA
Implementation Of TDOA Using Chan Algorithm And Taylor Algorithm in 3D space

Author:HaoyaWHL<br>
Finish Time:2016/5 when I was masterGrade 1<br>
Implement：Visual Studio 2013 in C++<br>


## Workflow
step1:using socket to get the data from four base stations<br>
step2:pre-treating the data,get the form as I want<br>
step3.1:using CHAN algorithm to get the coordinates of X and Y<br>
step3.2:using Taylor algorithm to make the coordinates of X and Y more accuracy<br>
step4.1:using CHAN algorithm to get the coordinate of Z<br>
step4.2:using Taylor algorithm to make the coordinates of Z more accuracy<br>
step5:output the coordinates of X , Y and Z


## 2018.11.23更新

这是2016年研一的时候独立承担的工程，应用场景中包含四个基站，在PC上通过8888号端口获取数据

由于解码问题，所以main.cpp中把原始数据从16进制->2进制->10进制，再进行处理

原理是采用了CHAN氏算法：4个基站ABCD，BCD分别与A构建三个双曲线方程，运用最小二乘两次求解（第一次最小二乘是为了估计噪声）

可以直接求解XYZ坐标，但是因为Z方向精度问题，先求解XY，再暴力求解Z

以现在来看，C++代码可读性较差，需要matlab仿真代码（100行不到，更为清晰）的同学可以直接发送邮件给我

----------------
ldlwh1@163.com


