实验30% 考试70%

20个选择（1分一个

10个填空（2分一个

简答10个（刷课后习题集

shell编程（10分，难度不会超过书上，不要出基本错误，不要乱写，15-20行

参考书：Linux命令行与shell脚本编程大全.第3版

 

# 第一章

linux 常识（会出选择填空）：内核是谁写的（记人名）、应用、组成、内核版本命名方式、主流发行版本、系统特点不用记不考

## Linux系统的应用领域

1.  Linux服务器：﻿可靠性，﻿支持多种硬件平台，相关软件开源或者免费
2. 嵌入式Linux系统：低成本、小内核、模块化
3. 软件开发平台
4. 桌面应用

## **Linux系统的组成**

1．内核：﻿内核的主要模块分为存储管理、 CPU和进程管理、文件系统、设备管理和驱动、网络通信、系统的初始化和系统调用等几个部分。

2．Shell: ﻿用户界面提供了用户与内核进行交互操作的一种接口

3．文件系统，﻿xfs、 ext4、 ext3 …

4．应用程序

## 内核版本命名方式

内核版本号是由点分隔的3段数字组成：r.x.y ，比如 3.10.0 - 327。

•       r：目前发布的内核主版本

•       x：偶数表示稳定版本;奇数表示开发版本

•       y：错误修补的次数

## 主流发行版本

1. **Red Hat**

2. **SUSE**

3. **Oracle**

4. **CentOS**

5. **Ubuntu**

6. **Debian**

# 第二章

## 交换分区有什么作用

​	虚拟内存 是为了满足物理内存的不足而提出的一种策略，它是**利用磁盘空间虚拟出的一块逻辑内存**，用作虚拟内存的磁盘空间被称为交换分区（swap分区）。内核会将暂时不用的内存块信息写到交换分区，这样一来， 物理内存得到了释放，这块内存就可以用于其它用途，当 需要用到原始的内容时，这些信息会被重新从交换分区读 入物理内存。

- Linux的内存管理采取的是**分页存取机制**，为了保证物理 内存能得到充分的利用，内核会在适当的时候将物理内存 中不经常使用的数据块自动交换到虚拟内存中，而将经常 使用的信息保留到物理内存。

- Linux系统会不时地进行页面交换操作，以保持尽可能多的空闲物理内存，**即使并没有什么操作需要使用内存， Linux也会交换出暂时不用的内存页面，这可以避免等待交换所需的时间**。

- Linux进行页面交换是有条件的，不是所有页面在不用时 都交换到虚拟内存，Linux内核根据“**最近最常使用**”算 法，仅仅将一些不经常使用的页面文件交换到虚拟内存。



## 分区有哪些，每个分区大小

![image-20201206100307852](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206100307852.png)



## linux下分区的命名方式

![image-20201206100433630](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206100433630.png)

 **磁盘分区一共有3种：主分区、扩展分区和逻辑驱动器。** 扩展分区只不过是逻辑驱动器的“容器”，实际上只有主 分区和逻辑驱动器才能进行数据存储。在一块磁盘上最多 只能有四个主分区，可以另外建立一个扩展分区来代替四个主分区的其中一个，然后在扩展分区下可以建立更多的逻辑驱动器。

# 第三章

## 字符界面切换

安装Linux系统之后，系统启动**默认进入的是图形化界面**， 可以通过使用以下命令修改为进入字符界面，所做改变在系统**重新引导之后即可生效**。

```shell
#查看计算机系统启动后要进入的默认目标，graphical.target表示图形化界面
[root@rhel ~]# systemctl get-default
graphical.target 
#/将multi-user.target目标设置为启动计算机系统后要进入的默认目标，multi-user.target表示字符界面
[root@rhel ~]# systemctl set-default multi-user.target
```

root 用户登录后时# ，普通用户时$

## linux下获取帮助的命令（**man**,help,info

## 关闭重启常用linux命令（reboot，halt，shutdown）

shutdown命令可以**安全地**关闭或重启Linux 系统。

```shell
shutdown [选项] [时间] [警告信息]
立即关闭计算机系统。
[root@rhel ～]# shutdown -h now

定时45分钟后关闭计算机系统。
[root@rhel ~]# shutdown -h +45

立即重新启动计算机系统，并发出警告信息。
shutdown -r now "system will be reboot now."

定时在1点38分重新启动计算机系统。
[root@rhel ~]# shutdown -r 01:38
```

**使用halt命令就是调用“shutdown –h”命 令执行关机任务。**

## 预定义目标和运行级别对应关系（此处有选择

![image-20201206103015091](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206103015091.png)

## 常用控制组合键（ctrl+z,ctrl+c等

![image-20201206103146803](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206103146803.png)



## 特殊字符对应功能需要会用

![image-20201206103245912](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206103245912.png)

![image-20201206103306987](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206103306987.png)

## 命令历史记录保存在什么地方

bash启动的时候会读取**~/.bash_history**文件，并将其载入 到内存中，**$HISTFILE变量就用于设置~/.bash_history**文 件，bash**退出时**也会把内存中的历史记录回写到 ~/.bash_history文件中。

## 搜索历史记录（ctrl+r快捷键



## 命令替换操作

Shell命令的参数可以由另外一 个命令的结果来替代，这种称之为命令替换。

2中替换方式

```shell
kill -9 $(pidof less)
kill -9 `pidof less`
```

## 管道

管道可以将 某个命令的输出信息当作某个命令的输入，由管 道符号“｜”来标识。

```shell
 rpm -qa|grep a|more
```



## 重定向（理解并会运用

重定向有四种方式：输出重定向、输入重定向、错误重定向以及同时实现输出和错误的重定向。

- 输出重定向，即将某一命令执行的输出保存到文 件中，如果已经存在相同的文件，那么 覆盖/追加 源文件中的内容。

```
ls /boot > /root/abc
echo Linux >> /root/ao
```

- 输入重定向，即将某一文件的内容作为命令的输入。另外一种特殊的输入重定向是输入追加重定向，这 种输入重定向告诉Shell，当前标准输入来自命令行 的一对分隔符之间的内容。

  ```shell
  cat < /root/mm
  #使用输入追加重定向创建/root/bc文件。
  cat > /root/bc <<EOF
  > Hello Linux
  > EOF
  ```

  ![image-20201206104427816](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206104427816.png)



- 错误重定向
  - 对于& 1 更准确的说应该是文件描述符 1,而1标识标准输出，stdout
  - 对于2 ，表示标准错误，stderr。
  - 2>&1 的意思就是将标准错误重定向到标准输出。这里标准输出已经重定向到了 /dev/null。那么标准错误也会输出到/dev/null
  - ls 2>1测试一下，不会报没有2文件的错误，但会输出一个空的文件1；
  - ls xxx >out.txt 2>&1, 实际上可换成 ls xxx 1>out.txt 2>&1；重定向符号>默认是1,错误和输出都传到out.txt了。

## vi编辑器：会保存，会切换模式，会删除，会复制粘贴（此处有选择

![image-20201206105212681](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206105212681.png)

 

dd  删除/剪贴光标所在行

ndd  从光标所在行开始删除n行，n代表数字

yy  复制光标所在行

p  粘贴

# 第四章

## 常见文件类型的符号（显示出来信息能看出是什么文件类型

```shell
-rw-------
第一个字符：
- : 普通文件
d : 目录文件
b : 设备文件 如dev/hda1
c : 字符设备文件 如，打印机和终端 /dev/null
p ： 管道文件
l : 链接文件
```

## 常见目录结构，给出一个目录要知道保存的是什么文件

![image-20201206105906620](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206105906620.png)

![image-20201206105927477](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206105927477.png)

## 链接文件（软链接和硬链接的创建、区别

- 在Linux系统中，内核为每一个新创建的文 件分配一个inode（索引节点）号，文件属 性保存在索引节点里，在访问文件时，索 引节点被复制到内存里，从而实现文件的 快速访问。
- 链接是一种在共享文件和访问它的用户的 若干目录项之间建立联系的方法。Linux系 统中包括**硬链接和软链接**（也被称为符号链接）两种。
- **硬链接**是一个指针，指向文件inode，系统 并不为它重新分配inode，两文件具有相同 的inode。硬链接节省空间，也是Linux系 统整合文件系统的传统方式。
  - 硬链接文件有两个限制： （1）不允许给目录创建硬链接； （2）只有在同一文件系统中的文件之间才 能创建链接。
- **软链接**也叫符号链接，这个文件包含了另 一个文件的路径名。可以是任意文件或目 录，可以链接不同文件系统的文件，和 Windows下的快捷方式相似**。链接文件甚 至可以链接不存在的文件，这就产生一般 称之为“断链”的问题**，链接文件甚至可 以循环链接自己。
- **区别：**
  - 硬链接记录的是目标的inode，软链接记录 的是目标的路径。软链接就像是快捷方式， 而硬链接就像是备份。
  - **软链接可以做跨分区的链接，而硬链接由于inode的缘故，只能在本分区中做链接**。所以软链接的使用 频率要高得多。

## 第五章

## 查找（grep;find -m要知道按文件名查找使用什么命令;

```shell
# 在/root/aa文件中找出以b开头的行内容。
[root@rhel ~]#grep ^b /root/aa

# 在/root/aa文件中找出不是以b开头的行内容。
[root@rhel ~]# grep -v ^b /root/aa 

# 在/root/kkk文件中找出以le结尾的行内容。
[root@rhel ~]# grep le$ /root/kkk


# 查找/boot目录下的启动菜单配置文件grub.cfg。
[root@rhel ~]# find /boot -name grub.cfg

#列出当前目录及其子目录下所有最近20天内更改过的文件。
[root@rhel ~]# find . -ctime -20
```

## 设定计算机名：hostnamectl 计算机名

## du -sh;df -sh两个命令分别是干什么的

 显示目录或文件的磁盘占用量。逐级进入指定目录的每一个子目录并显示该目录占 用文件系统数据块的情况。如果没有给出文件或目录名称，那么就对当前目录进行统计。

du [选项] [文件|目录]

•       -s：只显示总磁盘占用量。

•       -h：(human-readable) 以 MB 为单位显示。

df

display free disk space

•       -h：和 du 的一样 

# 第六章

```shell
#!/bin/bash
#filename:date
echo "Mr.$USER,Today is:"
echo `date`//此处为反引号
echo Whish you a lucky day !
```

 chmod u+x /root/date 增加执行权限

 常用预定义变量：

![image-20201206112738008](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206112738008.png)

变量置换

![image-20201206113205823](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206113205823.png)

变量表达式

![image-20201206113446767](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206113446767.png)

![image-20201206113455064](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206113455064.png)

![image-20201206113545454](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206113545454.png)

![image-20201206113627455](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206113627455.png)

```shell
test [表达式]
test $int1 -eq $int2
test $str1 = abcd
#/结果显示1表示文件empty存在且可读以及长度为0
test -r empty -a -s empty
```

![image-20201206191644609](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206191644609.png)

```shell
#!bin/bash
ans=0
for((i=1;i<=$1;i++))
do
 ans=`expr $i + $ans` # s=$[$s+$i]
done
echo $ans
```

![image-20201206191740876](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206191740876.png)

```shell
#!bin/bash
i=1
until [ $i -gt 10 ]
do
   echo `expr $i \* $i`
   i=`expr $i + 1`
done
```



```shell
#! /bin/bash
#filename:ak
echo -n "please input a score:"
read SCORE
echo "You input Score is $SCORE"
if [ $SCORE -ge 60 ];
then
echo -n "Congratulation!You Pass the examination。"
else
echo -n "Sorry !You Fail the examination!"
fi
echo -n "press any key to continue!"
read $GOOUT
```

```shell
#!/bin/bash
#filename:za
#Display a menu
echo _
echo "1 Restore"
echo "2 Backup"
echo "3 Unload"
echo
#Read and excute the user's selection
echo -n "Enter Choice:"
read CHOICE
case "$CHOICE" in
1) echo "Restore";;  #;;相当于break
2) echo "Backup";;
3) echo "Unload";;
*) echo "Sorry $CHOICE is not a valid choice  #*相当于default
exit 1
esac
```

```shell
#!/bin/bash
#filename:mm
for ab in 1 2 3 4
do
echo $ab
done

```

```shell
#!/bin/bash
#filename:qqq
sum=0
for INT in $*
do
sum=`expr $sum + $INT`
done
echo $sum


运行Shell程序qqq，输出内容如下所示。
[root@rhel ～]# bash /root/qqq 1 2 3 4 5

```

```shell
#!/bin/bash
#filename:zx
int=1
while [ $int -le 5 ]
do
sq=`expr $int \* $int`
echo $sq
int=`expr $int + 1`
done
echo "Job completed"
```

使用while语句创建一个根据输入的数值求累加和 （1+2+3+4+……+n）的Shell程序。 使用vi编辑器创建Shell程序，文件名为sum，文件内容如下所示。

```shell
#!/bin/bash
#filename:sum
echo -n "Please Input Number:"
read NUM
number=0
sum=0
while [ $number -le $NUM ]
do
echo number
echo "$number"
number='expr $number + 1'
echo sum
echo "$sum"
sum='expr $sum + $number'
done
echo
```

# 第七章

root用户UID为0。系统用户的UID为 1~999。普通用户的UID为1000~60000。

## 添加删除用户，添加删除组

```shell
#创建用户moon，并设置该用户UID为1510。
useradd -u 1010 moon
#创建用户newuser，并设置该用户主目录为/home/www。
useradd -d /home/www newuser
#创建用户pp，并指定该用户是属于组群root的成员。
useradd -g root pp
#创建用户abc，并设置该用户的Shell类型是/bin/ksh。
useradd -s /bin/ksh abc

#修改用户zhangsan的主目录为/home/kkk，并手动创建/home/kkk目录。
usermod -d /home/kkk zhangsan

#户moon的主目录随该用户一起删除了
userdel -r moon




```

groupadd [选项] [组群名]

•       -g 指定 GID

•       -r 创建系统组群

groupmod [选项][组群名]

•       -g 修改 GID

•       -n 修改组群名称

groupdel [组群名]

## 文件格式要有印象（/etc/shadow等四个文件

## 管理用户账户的配置文件有哪些?并描述这些文件各字段的含义。

/etc/passwd和/etc/shadow

/etc/passwd文件 一行有7个段位 `zhangsan:x:1000:1000:张三:/home/zhangsan:/bin/bash`

![image-20201206213142929](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206213142929.png)

**/etc/shadow**文件内容包括用户及被加密的密码以及其它/etc/passwd不能包括的信息，比如用户账户的有效期限等。﻿只有root用可以读取和操作。

## 管理组群账户的配置文件有哪些?并描述这些文件各字段的含义。

/etc/group和/etc/gshadow

﻿/etc/group文件是组群的配置文件，内容包括用户和组群，并且能显示出用户是归属哪个组群或哪几个组群。一个用户可以归属一个或多个不同的组群

﻿/etc/group文件的内容包括组群名、组群密码、 GID及该组群所包含的用户，每个组群一条记录，一行有4个段位，每个段位用“:”分隔。`zhangsan:x:1000:`

![image-20201206213320852](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206213320852.png)

GID为0的组群是root组群。 Linux系统会预留GID号1~999给系统虚拟组群使用，创建的新组群GID是从1000开始的



## password命令及其参数的使用

passwd [选项] [用户名]

普通用户只能修改自己的密码。如果 passwd 命令 后面不接任何选项或用户名，则表示修改当前用户的密码。

•       -l 锁住用户的密码

​        用户被锁住以后不能登录到系统，但是可以用 su 命令从其他用户切换到该用户

•       -u 解锁用户的密码

•       -S 查看用户密码的状态

•       -d 删除用户的密码

## su和sudo的区别

使用su命令可以切换到其它用户账户进行登录。如果su命令不加任何选项，默认为切换到root用户，并且不改变Shell环境

**su 使用的是root密码，但不会切换到root家目录**

sudo 使用的是当前用户的密码  使用sudo需要root对用户做授权**vi /etc/sudoers** 查看设置的权限，vi sudo 给普通用户设置root的权限，sudo可以对权限进行细化

# 第八章

了解将一块硬盘添加分区挂载使用的过程及命令

## 使用新磁盘存储数据一般要经过哪些操作步骤?

1. 进入fdisk界面，显示磁盘分区信息。 fdisk /dev/sda

2. 创建n和删除d主分区。

3. 创建扩展分区e和逻辑驱动器n。

4. 查看并转换分区类型。p

5. 保存分区设置信息，并退出fdisk。w

6. 在非交互式界面下显示当前硬盘的分区信息。﻿fdisk -l /dev/sda

7. 查看分区情况（使用partprobe命令更新分区表，使内核识别分区 ）﻿partprobe 后ls /dev/sda* 

8. 格式化分区，创建文件系统 **mkfs -t ext4 /dev/sda5**   ﻿mkfs命令可以在分区上创建各种文件系统

9. 挂载文件系统  ﻿**mount -o ro /dev/sda5 /mnt/kk**    以只读（ro）﻿挂载分区/dev/sda5到/mnt/kk目录中

10 卸载文件系统  ﻿**umount /dev/sda5** ﻿卸载分区/dev/sda5文件系统

11 ﻿df查看Linux系统上的磁盘分区挂载情况



## 磁盘分区

磁盘分区是指对硬盘物理介质的逻辑划分。将磁盘分成多 个分区，不仅仅利于对文件的管理，而且不同的分区可以 建立不同的文件系统，这样才能在不同的分区上安装不同 的操作系统。**磁盘分区一共有3种：主分区、扩展分区和逻辑驱动器。** 扩展分区只不过是逻辑驱动器的“容器”，**实际上只有主 分区和逻辑驱动器才能进行数据存储。在一块磁盘上最多 只能有四个主分区**，可以另外建立一个扩展分区来代替四 个主分区的其中一个，然后**在扩展分区下可以建立更多的 逻辑驱动器。**

用 fdisk 命令可以用来对磁盘进行分区， 还可以用来查看磁盘分区的详细信息，也能为每个分区指定分区的类型。

fdisk [选项] [设备]

-l 查看分区情况

**lsblk -f** 可以查看每个磁盘、磁盘的分区及分区的文件系统等

**fdisk** **交互式操作子命令：**

| 子命令 | 功能                               |
| ------ | ---------------------------------- |
| m      | 显示所有能在fdisk中使用的子命令    |
| p      | 显示磁盘分区信息                   |
| a      | 设置磁盘启动分区                   |
| n      | 创建新的分区                       |
| e      | 创建扩展分区                       |
| p      | 创建主分区                         |
| t      | 更改分区的系统ID(也就是分区类型ID) |
| d      | 删除磁盘分区                       |
| q      | 退出fdisk，不保存磁盘分区设置      |
| l      | 列出已知的分区类型                 |
| v      | 验证分区表                         |
| w      | 保存磁盘分区设置并退出fdisk        |

## 格式化

通过分区当然不能产生任何文件系统。分区只是对磁盘上 的磁盘空间进行了保留，还不能直接使用，在此之后分区 必须要进行格式化。在Linux系 统中大多使用mkfs命令来完成。 Linux系统支持不同的文件系统，目前应用最广泛的就是 xfs和ext4。

```
#格式化/dev/sda5分区，创建ext4文件系统。
mkfs -t ext4 /dev/sda5
```

## 挂载

如果要挂载一个分区，首先需要确认文件系统的类型，然后才能挂载使用，通过使用 mount 和 umount 命令可以实现文件系统的挂载和卸载。

```shell
#挂载分区/dev/sda5到/mnt/kk目录中。
mount /dev/sda5 /mnt/kk
#以只读方式挂载/dev/sda5分区到/mnt/kk目录中。
mount -o ro /dev/sda5 /mnt/kk

umount [选项] [设备|挂载目录]
#卸载分区/dev/sda5文件系统。
umount /dev/sda5
```

挂载选项

![image-20201206130309288](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206130309288.png)



## 开机自动挂载文件系统

### /etc/fstab文件简介

/etc/fstab 文件包含了所有磁盘分区以及存储设备的信息。其中包含了磁盘分区和存储设备如何挂载，以及挂载在什么目录上的信息。

/etc/fstab 文件是一个简单的文本文件，必须要以 root 用户登录才可以编辑该文件。

![image-20201206130352206](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206130352206.png)

1．**设备** 使用设备名称和UUID号表示都可以。如果要查看某 设备的UUID号，可以用下面命令： ls –l /dev/disk/by-uuid 或 blkid

2．**挂载目录** Linux系统为每个设备或分区设定了挂载目录。

3．**文件系统类型** Linux系统为每个设备或分区指定了文件系统类型。

 4．**挂载选项** （1）auto和noauto （2）user和nouser （3）exec和noexec （4）ro （5）rw （6）sync和async （7）defaults （8）owner

5．**转储选项** dump选项检查文件系统并用一个数字来决定该文 件系统是否需要备份。如果它是0，dump将会忽 略该文件系统，不做备份

6．**文件系统检查选项** fsck选项通过检验第6项中的数字来决定以何种 顺序检查文件系统，如果它是0，fsck将不检查 该文件系统。根文件系统（“/”）的默认值为1， 其他文件系统可以为2-9

# 第九章

## 用RPM软件包管理的用途是什么?

- 可以安装、删除、升级、刷新和管理RPM软件包；不需要进行编译即可安装软件包
- 通过RPM软件包管理能知道软件包包含哪些文件，也能知道系统中的某个文件属于哪个RPM软件包
- 可以查询系统中的RPM软件包是否安装并查询其安装的版本；
- 开发者可以把自己的程序打包为RPM软件包并发布
- 依赖性的检查，查看是否有RPM软件包由于不兼容而扰乱系统。

## 简述升级RPM软件包和刷新RPM软件包的区别

升级软件包实际上是**删除和安装**的组合。不管该软件包的早期版本是否已被安装，升级选项都会安装该软件包。

﻿刷新选项会升级到**更新的版本**。如果软件包先前没有安装， RPM的刷新选项将不会安装该软件包

## 简述在本地磁盘上创建本地软件仓库的步骤。

1．安装软件包。安装deltarpm、 python-deltarpm和createrepo软件包。

2．复制软件包。复制Linux系统安装光盘中的软件包。

3．创建软件仓库配置文件

4．创建软件仓库。使用createrepo命令创建软件仓库





## rpm命令的作用及常用选项

## rpm

•       -ivh：安装

•       -e：删除

•       -Uvh：升级

•       -Fvh：刷新。有则升级，无不安装

•       -q：查询
 -qa：查询系统所有

## yum命令

•       yum help 显示使用信息

•       yum list 列出软件包

•       yum search keyword 搜索关键

•       yum info packagename 列出软件包详细信息 
 yum info installed 列出所有已经安装的软件包信息

•       yum install packagename 安装软件包

​        yum -y packagename 无需确认，直接安装软件包

•       yum remove packagename 删除软件包

•       yum update packagename 升级软件包

•       yum deplist pacagename 列出软件包的依赖关系

•       yum repolist 显示软件仓库的配置

## tar命令 -zcvf -xcvf这两个是干什么的

```shell
#备份/root/abc目录及其子目录下的全部文件，备份文件名为abc.tar。
tar cvf abc.tar /root/abc
#-x或--extract或--get 从备份文件中还原文件。
tar xvf abc.tar
#只要里面的一个文件
tar xf /etc/tos/conf/tos.tar.gz manifest.json

#-c或--create 建立新的备份文件。
#-t或--list 列出备份文件的内容。
#-x或--extract或--get 从备份文件中还原文件。
#-v或--verbose 显示指令执行过程。
#-f<备份文件>或--file=<备份文件> 指定备份文件。



#将文件/root/abc/d添加到abc.tar包里面去。
#tar rvf abc.tar /root/abc/d
#-r或--append 新增文件到已存在的备份文件的结尾部分。
[root@rhel ~]# touch /root/abc/d

#更新原来tar包abc.tar中的文件/root/abc/d。
#-u或--update 仅置换较备份文件内的文件更新的文件。
[root@rhel ~]# tar uvf abc.tar /root/abc/d
```

#### tar调用gzip。

使用tar命令可以在归档或者是解包的同时 调用gzip压缩程序。以“.gz”结尾的文件 就是gzip压缩的结果。与gzip相对应的解压 缩程序是gunzip，tar命令中使用-z选项来 调用gzip。 

```shell
#把/root/abc目录包括其子目录全部做备份文件，并进行压缩，文件名为abc.tar.gz。
[root@rhel ~]# tar zcvf abc.tar.gz /root/abc
```

使用tar命令可以在归档或者是解包的同时 调用bzip2压缩程序。以“.bz2”结尾的文 件就是bzip2压缩的结果。与bzip2相对应 的解压缩程序是bunzip2。tar命令中使用-j 选项来调用bzip2。

```shell
#将目录/root/abc及该目录所有文件压缩成abc.tar.bz2文件。
[root@rhel ~]# tar jcvf abc.tar.bz2 /root/abc
```



# **第十章**

## 字符设定法和数值设定法

```shell
#使用文字设定法对/root/ab文件设置权限，所有者为读取、写入和执行权限，同组用户为读取和写入权限，而其他用户没有任何权限。
chmod u=rwx,g+rw,o-rwx ab

#使用数字设定法设置/root/ab文件的权限，所有者只拥有读取和写入权限。
chmod 600 ab

#将/root/ab文件的所有者更改为用户zhangsan。
chown zhangsan ab

#将/root/ab文件的组改为张三
chown :zhangsan ab

#将/root/ab文件的组和所有者改为张三
chown zhangsan:zhangsan ab
```

## 权限对于文件和目录有什么影响

•       **r**：读取文件；浏览目录

•       **w**：新增、修改文件内容；删除移动目录内文件

•       **x**：执行文件；进入目录

•       **-**：表示不具有该项权限

## 三个特殊权限

![image-20201206132624997](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206132624997.png)



![image-20201206132636927](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206132636927.png)



![image-20201206141144138](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206141144138.png)



 chmod 4000 a

# 第十一章

## 进程命令

### ps

•       ps -e：显示所有进程

•       ps -aux：显示所有不带控制台终端的进程，并显示用户名和进程的起始时间

•       ps -ax|grep less：查看 less 进程是否在运行

•       ps -u：显示用户名和进程的起始时间   ps -u root 显示用户 root 的进程

•       ps -t tty1：显示 tty1 终端下的进程

•       **ps -p 1659**：显示进程号为 1659 的进程



强制杀死进程号为 1659 的进程

```shell
kill -9 $(ps -ef | grep crond) 1 > /dev/null 2 > &1
```

## 任务计划cron会写

​		如果要在固定的时间上触发某个作业，就 需要创建任务计划，按时执行该作业，在 Linux系统中常用cron实现该功能。使用cron实现任务自动化可以通过**修改 /etc/crontab文件以及使用crontab**命令实 现，其结果是一样的。

### /etc/crontab文件详解

- root用户通过修改/etc/crontab文件可以实现任务 计划，而普通用户却无法修改该文件。crond守 护进程可以在无需人工干预的情况下，根据时间 和日期的组合来调度执行重复任务。

- /etc/crontab文件是cron的默认配置文件，前面3 行是用来配置cron任务运行环境的变量。Shell变 量的值告诉系统要使用哪个Shell环境（在这个例 子里是/bin/bash）。PATH变量定义用来执行命令的路径。cron任务的输出被邮寄给MAILTO变 量定义的用户名。如果MAILTO变量被定义为空白字符串，电子邮件就不会被寄出。

- /etc/crontab文件中的每一行都代表一项任 务，它的格式如下：

  minute hour day month dayofweek user-name commands

  - minute  分钟，0～59之间的任何整数
  - hour   小时，0～23之间的任何整数
  - day     日期，从 1～31之间的任何整数（如果指定了月份，必须 是该月份的有效日期）
  - month   月份，1～12之间的任何整数（或使用月份的英文简写 如jan、feb等）
  - day of week   星期，0～7之间的任何整数，这里的0或7代表星期日 （或使用星期的英文简写如sun、mon等）
  - user-name   执行命令的用户
  - command    要执行的命令或者是自己编写的脚本

##### 时间格式

| 时间格式 | 描述                                                         |
| -------- | ------------------------------------------------------------ |
| *        | 可以用来代表所有有效的值。如月份值中的星号意味着 在满足其它制约条件后**每月都执行该命令** |
| \-       | 指定一个整数范围。比如1-4意味着整数1、2、3、4                |
| ,        | 指定隔开的一系列值指定一个列表。比如3,4,6,8标明这 4个指定的整数 |
| /        | 可以用来指定间隔频率。在范围后加上/ 意味着 在范围内可以跳过integer。如“0-59/2”可以用来在分钟字 段上定义时间间隔为两分钟。间隔频率值还可以和星号一 起使用，**如“*/3”的值可以用在月份字段中表示每3个月 运行一次任务** |

/etc/crontab文件配置举例

```shell
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
30 21* * * root /root/backup.sh
//在每天晚上的21:30执行/root/backup.sh文件
45 4 1,10,22 * * root /root/backup.sh
//在每月1、10、22日的4:45执行/root/backup.sh文件
20 1 * * 6,0 root /bin/find / -name core -exec rm {} \;
//在每星期六、星期日的1:20执行一个find命令，查找相应的文件
0,30 18-23 * * * root /root/backup.sh
//在每天18:00～23:00之间每隔30分钟执行/root/backup.sh
0 23 * * 6 root /root/backup.sh
//在每星期六的23:00执行/root/backup.sh
```

### /etc/cron.d目录

除了通过修改/etc/crontab文件实现任务计 划之外，还可以在/etc/cron.d目录中创建文 件来实现。

该目录中的所有文件和/etc/crontab文件使 用一样的配置语法。

### crontab命令简介

root以外的用户可以使用crontab命令配置cron任务。所有 用户定义的crontab都被保存在**/var/spool/cron**目录中，并使用创建它们的用户身份来执行。

以某位用户身份创建一个crontab项目，登录为该用户， 然后输入crontab -e命令，使用由VISUAL或EDITOR环境变量指定的编辑器来编辑该用户的crontab。该文件使用 的格式和/etc/crontab相同。当对crontab所做的改变被保存后，该crontab文件会根据该用户名被保存在 /var/spool/cron/文件中。

**创建crontab** 

  创建新的crontab，然后提交给crond进程。 同时，新创建crontab的一个副本已经被放 在/var/spool/cron目录中，文件名就是用户名。

```shell
#以普通账号zhangsan登录系统，创建用户crontab条目。
[root@rhel ～]# su - zhangsan
#以普通用户zhangsan登录系统
[zhangsan@rhel ～]$ crontab -e
#使用“crontab -e”命令打开vi编辑器,编辑用户zhangsan的crontab条目
8 * * * * cp /home/zhangsan/aa /home/zhangsan/bb
#在vi编辑器内输入以上crontab条目

#切换为root用户登录
[root@rhel ～]# cat /var/spool/cron/zhangsan
8 * * * * cp /home/zhangsan/aa /home/zhangsan/bb
#可以看到/var/spool/cron/zhangsan的内容就是刚才用“crontab -e”编辑的内容。记住：普通用户没有权限打开该文件


```

## 系统启动过程步骤

1. BIOS 自检

2. 启动 GRUB 2

3. 加载内核

4. 执行systemd进程

5. 初始化系统环境

6. 执行 /bin/login 程序

 

# 十一章以后：

## LVM机制的基本概念（PV VG LV）

![img](https://gitee.com/CTLQAQ/picgo/raw/master/clip_image002.jpg)

### **简述NFS的含义**

​		**NFS（Network File System，网络文件系统）﻿NFS对于在同一个网络上的多个用户间共享目录和文件很有用途。通过使用NFS，用户和程序可以像访问本地文件一样访问远程系统上的文件。**﻿NFS本身的服务并没有提供文件传递的协议，但是NFS却能让我们进行文件的共享，这其中的原因，就是NFS使用RPC协议。﻿﻿NFS是一个文件系统，而RPC是负责信息的传输。﻿ NFS主要管理分享出来的目录，而至于文件的传递，就直接将它交给RPC协议来运作。

## **简述/etc/exports文件内容的格式**

/etc/exports文件控制着NFS服务器要导出的共享目录以及访问控制。 /etc/exports文件默认是空白的，没有任何内容。也就是说NFS服务器默认是不共享任何目录，需要手工编辑添加。

/etc/exports文件内容的格式如下所示。

**共享目录 客户端（导出选项）**

共享目录：在/etc/exports文件中添加的共享目录必须使用绝对路径，不可以使用相对路径。而且该目录必须事先创建好，该目录将作为NFS服务器上的共享目录并提供给客户端使用

客户端：客户端是指可以访问NFS服务器共享目录的客户端计算机，客户端计算机可以是一台计算机，也可以是一个网段，甚至是一个域.。

![image-20201206210429780](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201206210429780.png)

##  简述 Openssh替代 telnet的主要原因

​		OpenSSH默认使用**RSA密钥**，它采用**安全、加密的网络**连接工具代替telnet、 ftp、 rlogin、 rsh和rcp工具。﻿telnet和ftp使用纯文本口令，并以明文发送。这些信息可能会被截取，口令可能会被检索，未经授权的人员可能会使用截取的口令登录用户的系统，而对系统产生危害

## 简述VNC软件的组成部分

​		VNC（virtual network computing 虚拟网络计算机）软件要由两个部分组成：服务端的**VNC server**和客户端的**VNC viewer**。用户需先将VNC server安装在被远程操控的计算机上后，才能在主控端执行VNC viewer进行远程操控

# 第十二章

## 测试网络连通可以使用哪些命令?

ping、netstat、ifconfig、traceroute

## DNS服务使用什么端口号?

53

scp /root/a root@192.168.0.100:/root/b  mv /root/b /root/c﻿用root账号把本地文件root/a传送到192.168.0.100远程主机

## 简述网卡配置文件的内容

```properties
/etc/sysconfig/network-scripts 其中文件ifcfg-eno16777736包含一块网卡的配置信息
TYPE=Ethernet 网卡类型:以太网
PROXY_METHOD=none 代理方式:关闭状态
BROWSER_ONLY=no 只是浏览器(yes|no)
BOOTPROTO=static 设置网卡获得ip地址的方式(static|dhcp|none|bootp)
DEFROUTE=yes 设置为默认路由(yes|no)
IPV4_FAILURE_FATAL=no 是否开启IPV4致命错误检测(yes|no)
IPV6INIT=yes IPV6是否自动初始化
IPV6_AUTOCONF=yes IPV6是否自动配置
IPV6_DEFROUTE=yes IPV6是否可以为默认路由
IPV6_FAILURE_FATAL=no 是不开启IPV6致命错误检测
IPV6_ADDR_GEN_MODE=stable-privacy IPV6地址生成模型
NAME=eth0 网卡物理设备名称
UUID=6e89ea13-f919-4096-ad67-cfc24a79a7e7 UUID识别码
DEVICE=eth0 网卡设备名称
ONBOOT=no 开机自启(yes|no)
IPADDR=192.168.103.203 IP地址
NETNASK=255.255.255.0 子网掩码,也可使用掩码长度表示(PREFIX=24)
GATEWAY=192.168.103.1 网关
DNS1=114.114.114.114 首选DNS
DNS2=8.8.8.8 备用DNS

```

