# 程序设计大作业说明文档
 [toc]

## 成员介绍

### 成员列表

### 分工情况



## 功能实现

### 基本功能

#### terminal框架

#### 复合指令

#### diff指令

#### grep指令

#### tee指令

#### cat指令

#### cp指令

#### cd指令

#### pwd指令

#### echo指令

### 额外功能

#### ls指令

#### cls/clear指令

#### change指令(主题切换)

#### vim指令(系统调用)

#### exit指令(退出程序)

## 实现细节

### 组织框架和文件说明





接下来我们分模块介绍每个模块的实现细节, 对于算法的应用, 会适当解析算法的细节, 对于部分重要实现, 我们会详细解说相应的代码。

### part1

##### diff指令

### part2

##### grep指令

### part3

##### tee指令

##### cat指令

##### cp指令

##### cd指令

##### pwd指令

### part4

##### echo指令

##### 框架实现

##### 复合指令

### part5

##### ls指令

##### cls/clear指令

##### change指令(主题切换)

##### vim指令(系统调用)

##### exit指令(退出程序)



## 测试部分



### 视频测试内容

由于视频时间所限, 我们挑选了较为复杂的测试样例进行测试, 主要聚焦基本功能的实现和一部分的出错处理:

// todo每个部分想自己的测试样例, 尽量组合成复合指令

```shell
Machine Name:mac
Root Directory: #相应工作目录
Login:usr
ls
pwd
echo hello|grep hel -|cat -n -
echo hello|diff - a.txt|cat -s -
cat a.txt |grep di -|cat -n -s  -E -
cls

```





### 其他测试内容和测试结果