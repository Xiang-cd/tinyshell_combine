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

* **获取帮助 grep --help**
* **计算符合样式的行数 grep -c \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，不标示该行所属的文件名称 grep -h \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，标示该行所属的文件名称 grep -H \[模式串]\[文件]\* **
* **忽略字符大小写区别 grep -i \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，标示该行的编号 grep -n \[模式串]\[文件]\* **
* **除了显示符合范本样式的那一行之外，同时显示出该行之后的\[行数]行内容 grep -A\[行数] \[模式串]\[文件]\* **
* **除了显示符合范本样式的那一行之外，同时显示出该行之前的\[行数]行内容 grep -B\[行数] \[模式串]\[文件]\* **
* **相互排斥的指令处理：以后出现的为准**
* **输入非法处理**
  * **文件不存在**
  * **指令有误**
  * **参数有误**

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

<center> <font size=4 face = "黑体">简介与实现框架展示</font> </center>

本次大作业TinyShell的Grep指令实现，输出样式、报错样式、匹配模式等均按照标准的Linux样式。对于文件的处理，采用“面向对象程序设计”的思想，构建了File类。每个File对象负责对一个文件（包括从strin读入）进行处理，并记录每个文件的具体内容、其匹配的行数、匹配的区段等信息。最终执行指令的时候，由doGrep函数统一调用File类中存储的已经处理好的信息进行输出。

![image-20211229234550587](https://tva1.sinaimg.cn/large/008i3skNly1gxv7lsaeygj31me0i4mz8.jpg)



<center> <font size="4" face="黑体">代码与关键函数总览</font></center>

* **stringProcess相关函数**

|                    stringProcess相关函数                     |                         函数功能简介                         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|      **void splitByStar(string s,vector\<string\>& v)**      |          将s通过*号分割开，分成若干段不含*号的子串           |
|       **int findFirstOf(string P,string T, int idx)**        | 在文本串T中找到模式串P第一次出现的位置，(.可以匹配任意字符)  |
|            **int findLastOf(string P,string T)**             | 在文本串T中找到模式串P最后一次出现的位置（.）可以匹配任意元素 |
| **bool match(string P,string T,int &front, int& back, int i_flag)** | 检测模式串P与字符串T是否匹配，并以引用的形式给出匹配的区间段[front,back] |
|         **string colorLine(string s, int l, int r)**         | 在[l,r]区间内的字符标红处理,如遇到l=r=-1则不用标红直接返回即可 |
|        **string colorFile(string fName, bool match)**        | 输出带颜色的文件名，如果是与子串匹配的要输出filename: 如果不是匹配的输出filename- |
|       **string colorLineNum(int lineNum, bool match)**       | 输出带颜色的行号，如果是匹配的，则输出lineNum： 如果不是匹配的则输出lineNum- |

* **fileProcess相关函数**

|                  fileProcess相关函数                   |                         函数功能简介                         |
| :----------------------------------------------------: | :----------------------------------------------------------: |
|              **File::File(string Name)**               | 构造函数，记录文件名，<font color="red">若输入的文件名是- 则将文件名置为(standard input)，并且从stdin读取。</font> |
|              **bool File::getContents()**              | 获取当前文件的内容,若成功读取则返回true，若该文件不存在则返回false |
| **void File::checkMatch(string patterns, int i_flag)** |    检测模式串与该文件内每一行是否匹配，并记录下匹配的位置    |

* **doGrep相关函数**

|                        doGrep相关函数                        |                         函数功能介绍                         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|                       **void init()**                        | 在每次解析命令行之前，先将c_flag,h_flag,i_flag,A_num,B_num初始化 |
| **int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)** | 对命令行参数进行解析.若为非法输入则返回0;若为help指令,返回2；其余返回1 |
|           **void doGrep(int argc, char * argv[])**           | grep指令函数，将按照传入的参数进行指令处理，并将结果输出到strout |

<center> <font face = "黑体" size=4> 主要实现功能说明</font> </center>

*  **实现带由*和.的模式串与文本串的<font color="red">最长</font>匹配**

  **整体构思**

  对于带有\*号的模式串P，可通过\* 号将其分割为$${P=p_1+'*'+p_2+'*'+…+p_n}$$.设$${T[l_i,r_i]}$$为子串$${p_i}$$在文本串T中分布的位置。则模式串与文本串能够匹配的条件$${\forall i\leq n,p_i}$$为文本串P的子串，且${\forall i <j,r_i <l_j}$.另外，要使匹配能够<font color="red">最长</font>,则需要使得$${p_1}$$的匹配尽量靠前，而${p_n}$的匹配尽量靠后。则匹配最长匹配的区段为${T[l_1,r_ n]}$.

  **具体实现**

  * 通过\* 切割模式串P的函数**void splitByStar(string s,vector\<string\>& v)**

    逐一扫描字符串中的每一个字符，如果遇到\* 号就利用STL中的substr函数将上一个\*号与该\*号之间的部分截取出来，如果遇到多个连续分布的多个\*号，只考虑其中一个即可。并将分割出来的子串存入STL提供的vector中。

    <font color="red"> 注：对于\*号之前或之后没有字符的情况，视分割出来的子串为空串。例如：\*abc分割后为空串以及"abc". </font>

  * 找到子串$${p_i}$$在文本串中首次出现位置的函数**int findFirstOf(string P,string T, int idx)**以及最后一次出现位置的函数**int findLastOf(string P,string T)**

    注：findFirstOf函数与FindLastOf实现思路类似，本处只介绍findFirstOf函数的实现思路。

    findFirstOf函数采用蛮力算法。对于模式串P和文本串T，按自左向右的次序考察各子串。在初始状态下，T的前m个字符将与P的m个字符两两对齐。接下来，自左向右检查相互对其的这m对字符。若当前字符对相互匹配，则转向下一个字符；一旦失配，则说明在此位置文本串与模式串不可能完全匹配，于是可将P向右移动一位，然后从其首字母开始与T中对应的新子串重新对比。若经过检查，当前的m对字符均匹配，则意味着整体匹配成功，从而返回匹配字符串的位置。

    <font color="red">注：由于.可以匹配任意字符，所以对于某对字符，模式串中字符为.或者模式串与字符串中对应的字符相同都视为该对字符匹配.对于空字符串，规定其与任何字符串都匹配。</font>

  * 判断模式串P（可带有\* 和.）是否与文本串匹配的函数**bool match(string P,string T,int &front, int& back, int i_flag)**

    先将模式串P拆解为不带\*号的字符串。然后找出第一个字符串在文本串中位置以及最后一个字符串在文本串中出现的最后一个位置。接下来只要检测除这两个字符串以外的所有字符串是否为文本串中的子串以及是否满足${\forall i <j,r_i <l_j}$即可。若上述检测均成功，则返回true，否则返回false。

    <font color="red">注：i_flag表示的是是否忽略大小写，i_flag为1时忽略大小写，为0时区分大小写。</font>

* **实现指令解析**

  **整体构思**

  Linux中的grep指令一般遵循以下原则：

  * -c的优先级最高。输入grep指令的时候，只要含有-c就只会输出对应的行数，并不会输出文本串具体内容。

    例如。例如，输入grep -c -A3 a a.txt会输出以下结果：

    ![image-20211204192131050](https://tva1.sinaimg.cn/large/008i3skNly1gxv7lyuamuj318i0g4jsb.jpg)

  * 对于互相“排斥”的指令，以最后出现的为准

    例如,grep -A3 -A2 a a.txt则会以后出现的-A2为准:

    ![image-20211204194153347](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m1nb43j318g0im0tr.jpg)

    又如，grep -H -h a a.txt会以后出现的-h为准：

    ![image-20211204195315723](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m5ed5ej311m0isq40.jpg)

  基于以上思路，为了控制各个各个指令的开启关闭等状态，本次实现大作业时，在doGrep.h文件中定义了几个变量。

  ```c++
  static int c_flag; //1代表命令中含有-c,应输出计算符合样式的行数
  static int h_flag; //0和-1代表不显示所属的文件名称（-1为默认值），1代表输出所属的文件名称
  static int n_flag; //0代表不用输出对应的行号，1代表要输出对应的行号
  static int i_flag; //0代表区分大小写，1代表不区分大小写
  static int A_num; //代表输出符合范本样式后的行数
  static int B_num; //代表输出符合范本样式前的行数
  ```

  **int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)**函数会根据传入的参数将对应的变量设置为对应值，从而在输出的时候可以实现对应的操作。

* **当输入非法时实现报错**

  要处理的输入非法一般分为以下几种情况：

  * 文件不存在

    文件存在性的检测由FIle类完成。ifstream fin打开对应文件失败时，即文件不存在。在此情况下，返回false并输出**No such file or directory**的提示。

  * 指令错误

    若输入的指令不是${\{-h\}\{-H\}\{-n\}\{-c\}\{-A[行数]\}\{-B[行数]\}}$之一，则**commandLineParse**函数会给出invalid option的提示。

    例如，输入grep -g A a.txt：

    ![image-20211204201634045](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m8nc3kj30gk0403yt.jpg)

  * 参数输入错误

    对于${\{-A[行数]\}\{-B[行数]\}}$，若输入的行数非法，则**int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)**函数会给出invalid context length argument的提示。

    例如：

    ![image-20211204201942982](https://tva1.sinaimg.cn/large/008i3skNly1gxv7mbi0nbj30h80380t2.jpg)

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