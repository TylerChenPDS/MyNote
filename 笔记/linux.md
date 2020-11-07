# 1， 基本概念

## 1.1 交换分区

- 虚拟内存是为了满足物理内存的不足而提出的一种策略，它是利用磁盘空间虚拟出的一块逻辑内存，用作虚拟内存的磁盘空 间被称为交换分区（swap分区）。
- **内核会将暂时不用的内存块信息写到交换分区，**这样一来， 物理内存得到了释放，这块内存就可以用于其它用途，当 需要用到原始的内容时，这些信息会被重新从交换分区读 入物理内存。
- Linux的内存管理采取的是分页存取机制，为了保证物理 内存能得到充分的利用，内核会在适当的时候将物理内存 中不经常使用的数据块自动交换到虚拟内存中，而将经常 使用的信息保留到物理内存。
- Linux系统会不时地进行页面交换操作，以保持尽可能多的空闲物理内存，**即使并没有什么操作需要使用内存， Linux也会交换出暂时不用的内存页面，这可以避免等待交换所需的时间。**
- Linux进行页面交换是有条件的，不是所有页面在不用时 都交换到虚拟内存，Linux内核根据“**最近最常使用**”算 法，仅仅将一些不经常使用的页面文件交换到虚拟内存。

## 1.2 分区命名方案

​		Linux系统使用字母和数字的组合来指代硬盘分区，使用 一种更加灵活的分区命名方案，该命名方案是基于文件的， 文件名的格式为**/dev/xxyN**（比如/dev/sda1分区）。

- /dev：这是Linux系统中所有设备文件所在的目录名。因 为分区位于硬盘上，而硬盘是设备，所以这些文件代表了 在/dev上所有可能的分区；
- xx：分区名的前两个字母表示分区所在设备的类型，通常 是hd（IDE硬盘）或**sd**（SCSI硬盘）。
- y：这个字母表示分区所在的设备。例如，/dev/hda（第1 个 IDE 硬盘）或/dev/sdb（第2个SCSI硬盘）；
- N：最后的数字N代表分区。**前4个分区（主分区或扩展分 区）用数字1～4表示**，**逻辑驱动器从5开始**。例如， /dev/hda3是第1个IDE 硬盘上的第3个主分区或扩展分区； /dev/sdb6是第2个SCSI硬盘上的第2个逻辑驱动器。

## 1.3 磁盘分区和挂载目录

- Linux系统中的每一个分区都是构成支持一组文件和目录 所必需的存储区的一部分。它是通过挂载来实现的，**挂载 是将分区关联到某一目录的过程**，挂载分区使起始于这个 指定目录（称为挂载目录）的存储区能够被使用。
- 例如，如果分区/dev/sda5被挂载在目录/usr上，这意味着 所有在/usr下的文件和目录在物理上位于/dev/sda5。因此 文件/usr/bin/cal被保存在分区/dev/sda5上，而文件 /etc/passwd却不是。
- /usr目录下的目录还有可能是其它分区的挂载目录。例如， 某个分区（如/dev/sda7）可以被挂载到/usr/local目录下， 这意味着文件/usr/local/man/whatis将位于分区/dev/sda7 上，而不是分区/dev/sda5上。

# 2 基本芝士

## 2.1 小知识

### 2.1.1 命令排列

#### 1 命令1; 命令2

使用“；”命令时先执行命令1，不管命令 1是否出错，接下来就执行命令2。

```shell
ls;ls ../
```

#### 2  命令1&&命令2

使用“&&”命令时只有当命令1正确运行完 毕后，才能执行命令2 。

```shell
ls sadas && ls
# asdas 不存在，后面的ls不会执行
```

### 2.1.2 命令替换--命令1 $(命令2) 

Shell命令的参数可以由另外一 个命令的结果来替代，这种称之为命令替换。

```shell
命令1 $(命令2) 
命令1 `命令2`
## 例如
ls $(pwd)
ls `pwd`
```

### 2.1.3 命令别名 alias

```shell
#给以命令起别名
alias mylist="ls $(pwd)"
#取消别名
unalias mylist
```

### 2.1.4 管道

将 某个命令的输出信息当作某个命令的输入，由管 道符号“｜”来标识。

```shell
cat /etc/profile | grep java
```

### 2.1.5 错误重定向

**[命令] 2> [文件]**

```shell
[root@rhel ~]# cat /root/kk 2> /root/b
[root@rhel ~]# cat /root/b
cat: /root/kk: 没有那个文件或目录
```

同时实现输出和错误重定向

**[命令] &> [文件]**

如果命令可以正确执行，则输出到控制台，否则错误信息输出到指定文件

```shell
[root@rhel ~]# ls /boot &> /root/kk
[root@rhel ~]# cat /root/kk
config-3.3.4-5.fc17.i686.PAE
# /因为/boot目录下有文件，所以最终使用了输出重定向

[root@rhel ~]# ls /nn &> /root/oo
[root@rhel ~]# cat /root/oo
ls: 无法访问/nn: 没有那个文件或目录
#因为没有/nn目录，所以最终使用了错误重定向
```

## 2.2 目录和文件管理

### 2.2.1 文件类型

#### 1．普通文件

​		用“ls -lh”命令查看某个文件的属性， 可以看到有类似**“-rw-------”**的属性符 号，其属性第一个符号是“-”，这样的文 件在Linux系统中就是普通文件。

#### 2．目录文件

​		当在某个目录下执行“ls -lh”命令，看 到有类似“**drwxr-xr-x**”的属性符号，其 属性第一个符号是“d”，这样的文件在 Linux系统中就是目录文件。

#### 3．设备文件

Linux系统中的/dev目录中有大量的设备文 件，主要是块设备文件和字符设备文件

- 块设备文件。块设备的主要特点是可以随机读写，而最 常见的块设备就是磁盘，如/dev/hda1、 /dev/sda1等。用“ls -l”命令查看某个 文件的属性，可以看到有类似**“brw-rw--- -”**的属性符号，其属性第一个符号是 “b”，这样的文件在Linux系统中就是块 设备文件。
- 字符设备文件。最常见的字符设备文件是**打印机和终端**，可以接收字符流。/dev/null是一个非常有 用的字符设备文件，送入这个设备的所有 内容都被忽略。用“ls -l”命令查看某个 文件的属性，可以看到有类似**“crw--w--- -”**的属性符号，其属性第一个符号是 “ c”，这样的文件在Linux系统中就是字 符设备文件。

#### 4．管道文件

​		管道文件有时候也被叫做FIFO文件（FIFO 是先进先出的意思），管道文件就是从一 头流入，从另一头流出。用“ls -l”命令 查看某个文件的属性，可以看到有类似 “**prw-------**”的属性符号，其属性第一 个符号是“p”，这样的文件在Linux系统 中就是管道文件

#### 5．链接文件

链接文件有两种类型：软链接文件和硬链接文件。

- **软链接文件**。**软链接文件又叫符号链接文件，这个文件包含了另一个文件的路径名。**在对软链接文件进行读写的时候，系统会 自动地把该操作转换为对源文件的操作，但删除软链接文 件时，系统仅仅删除软链接文件，而不删除源文件本身。

  用“ls -l”命令查看某个文件的属性，可以看到有类似 “**lrwxrwxrwx**”的属性符号，其属性第一个符号是“l”， 这样的文件在Linux系统中就是软链接文件

- **硬链接文件。**硬链接是已存在文件的另一个文件，对硬链接文件进行读 写和删除操作时，结果和软链接相同。但如果删除硬链接 文件的源文件，硬链接文件仍然存在，而且保留了原有的内容。

软硬链接的区别：

- 硬链接记录的是目标的inode，软链接记录 的是目标的路径。软链接就像是快捷方式， 而硬链接就像是备份。
- 软链接可以做跨分 区的链接，而**硬链接由于inode的缘故，只 能在本分区中做链接。**所以软链接的使用 频率要高得多。

### 2.2.2 文件操作命令

#### 1 pwd

显示用户当前工作目录路径。

#### 2 wc

统计指定文件的**行数、单词数、字节数和 字符数**，并将统计结果显示输出到屏幕。 如果没有给出文件名，则从标准输入读取。

#### 3 ln

ln [选项] [源文件名] [链接文件名]

```shell
# 创建软链接
[root@rhel ~]#echo hello > a
[root@rhel ~]#ln –s a b
# 创建硬链接
[root@rhel ~]#echo hello > a
[root@rhel ~]#ln a b
```

## 2.3 常用操作命令

### 2.3.1 文本内容

#### 1 cat：显示文本

cat [选项] [文件]

```shell
#  把textfile1文件的内容加上行号后输入到textfile2文件中。
[root@rhel ~]# cat -n textfile1 > textfile2

#使用cat命令创建mm.txt。
[root@rhel ~]#cat >mm.txt<<EOF
>Hello
>Linux
>EOF //在此输入字符EOF，会自动回到shell提示符界面
```

#### 2 tail 和 head

```shell
#  查看/etc/passwd文件末尾3行数据内容。
[root@rhel ~]# tail -3 /etc/passwd
# 查看/etc/passwd文件的前3行数据内容。
[root@rhel ~]# head -3 /etc/passwd
```

#### 3 sort

sort [选项] [文件]

```shell
# 将文件textfile1数据排序，并显示在屏幕上。
[root@rhel ~]# sort textfile1

#读取textfile1文件内容，以倒序排序该文件并显示在屏幕上。
[root@rhel ~]# sort -r textfile1

```

#### 4 uniq

将文件内的重复行数据从输出文件中删除， 只留下每条记录的惟一样本。

uniq [选项] [文件] 

```shell
# 查看文件file3中重复的数据内容。
[root@rhel ~]# cat file3
aaa
aaa
bbb
[root@rhel ~]# uniq -d file3
aaa //file3文件中重复行数据的内容为aaa

# 查看文件file3中不重复的数据内容。
[root@rhel ~]# uniq -u file3
bbb //file3文件中不重复行数据的内容为bbb
```

#### 5 grep

查找文件内符合条件的字符串。

grep [选项] [查找模式] [文件名] 

```shell
# 在文件kkk中搜索匹配字符“test file”。
[root@rhel ~]# grep 'test file' kkk

# 显示所有以d开头的文件中包含“test”的行数据内容。
[root@rhel ~]# grep 'test' d*

# 在/root/aa文件中找出以b开头的行内容。
[root@rhel ~]#grep ^b /root/aa

# 在/root/aa文件中找出不是以b开头的行内容。
[root@rhel ~]# grep -v ^b /root/aa 

# 在/root/kkk文件中找出以le结尾的行内容。
[root@rhel ~]# grep le$ /root/kkk 

```

#### 6 find

​		将文件系统内符合条件的文件列出来，可 以指定文件的名称、类别、时间、大小以 及权限等不同信息的组合，只有完全相符 的文件才会被列出来。

find [路径] [选项] 

```shell
# 查找/boot目录下的启动菜单配置文件grub.cfg。
[root@rhel ~]# find /boot -name grub.cfg

#列出当前目录及其子目录下所有最近20天内更改过的文件。
[root@rhel ~]# find . -ctime -20
```

### 2.3.2 系统信息

#### 1 uname

显示计算机以及操作系统的相关信息，比 如计算机硬件架构、内核发行号、操作系 统名称、计算机主机名等。

uname [选项]

```shell
#显示操作系统的内核发行号。
[root@rhel ~]# uname -r
3.10.0-327.el7.x86_64
# 显示计算机硬件架构名称。
[root@rhel ~]# uname -m
X86_64

# uname -a
Linux rhel 3.10.0-327.el7.x86_64
#1 SMP Thu Oct 29 17:29:29 EDT 2015 x86_64 x86_64
x86_64 GNU/Linux
```

#### 2 hostname

显示或修改计算机的主机名。

```shell
# 显示当前计算机主机名。
[root@rhel ~]# hostname

# 修改计算机主机名为LINUX。这样重启后就会失效
[root@rhel ~]# hostname LINUX 

#真正的修改需要
vim /etc/hostname 
```

#### 3 free

显示系统的物理内存和swap的使用情况。

```shell
# 以MB为单位查看系统的物理内存和交换分区使用情况。
[root@rhel ~]# free -m

#显示系统的物理内存加上交换分区总的容量。
[root@rhel ~]# free -t
```

#### 4 du

​		显示目录或文件的磁盘占用量。逐级进入 指定目录的每一个子目录并显示该目录占 用文件系统数据块的情况。**如果没有给出 文件或目录名称，那么就对当前目录进行 统计。**

```shell
#以人能看懂的单位显示/root目录磁盘占用量。
[root@rhel ~]# du –sh /root
```

#### 5 wall

```shell
#向所有用户发出“下班以后请关闭计算机。”的信息。
[root@rhel ~]# wall '下班以后请关闭计算机'
```



### 2.3.3 时间和日期

#### 1 cal

cal [选项] [[[日] 月] 年]

```shell
# 显示本月的月历。
[root@rhel ~]# cal

#显示公元2001年年历。
[root@rhel ~]# cal 2001

#显示公元2007年9月的月历。
[root@rhel ~]# cal 9 2007

#以1月1日起的天数显示今年的年历。
[root@rhel ~]# cal -jy
```

![](./img/6.png)

## 2.4 用户和组群账户管理

### 2.4.1 用户账户

​		用户账户在Linux系统中是分角色的，由于 角色不同，每个用户的权限和所能完成的 任务也不同。而在实际的管理中，用户的角色是通过UID（用户ID号）来标识的，每个用户的UID都是不同的。

​		在Linux系统中有三大类用户，分别是root 用户、系统用户和普通用户。

- **root用户**。**root用户UID为0。**root用 户的权限是最高的，普通用户无法执行的 操作，root用户都能完成，所以也被称之 为超级用户。在系统中的每个文件、目录 和进程都归属于某一个用户，没有用户许 可，其它普通用户无法进行操作的，但对 root用户除外。
- **系统用户**也称为虚拟用户、伪用户或假用 户，这类用户不具有登录Linux系统的能力， 但却是系统运行不可缺少的用户，比如bin、 daemon、adm、ftp、mail等，这类用户都 是系统自身拥有的。**系统用户的UID为 1~999。**
- **普通用户**。这类用户能登录系统，在Linux系统上进行 普通操作，能操作自己目录的内容，其使 用系统的权限受限，这类用户都是系统管 理员创建的。普通用户的UID为1000~60000。

​     **/etc/passwd**文件是系统识别用户的一个重要文件， Linux系统中所有的用户都记录在该文件中。假设 用户以账户zhangsan登录系统时，系统首先会检 查/etc/passwd文件，看是否有zhangsan这个账户， 然后确定用户zhangsan的UID，通过UID来确认用 户的身份，如果存在则读取/etc/shadow文件中所 对应的密码。

​	**/etc/shadow**文件是/etc/passwd的影子文件，这 个文件并不是由/etc/passwd文件产生，这两个文 件应该是对应互补的。/etc/shadow文件内容包括 用户及被加密的密码以及其它/etc/passwd不能包 括的信息，比如用户账户的有效期限等。

任何用户都可以读取/etc/passwd文件内容，在 /etc/passwd文件中，每一行表示的是一个用户账 户的信息，一行有7个段位，每个段位用“:”分隔。 **zhangsan:x :1000:1000:张三:/home/zhangsan:/bin/bash**

(用户名：密码：UID：GID：用户名全程：主目录：登录用的shell)

### 2.4.2 用户相关的命令

#### 1 useradd/usermod

```shell
#创建用户账户zhangsan并设置密码。
[root@rhel ~]# useradd zhangsan
[root@rhel ~]# passwd zhangsan

#创建用户moon，并设置该用户UID为1510。
[root@rhel ~]# useradd -u 1010 moon

#创建用户newuser，并设置该用户主目录为/home/www。
[root@rhel ~]# useradd -d /home/www newuser

#创建用户pp，并指定该用户是属于组群root的成员。
[root@rhel ~]# useradd -g root pp

#创建用户abc，并设置该用户的Shell类型是/bin/ksh。
[root@rhel ~]# useradd -s /bin/ksh abc

```

usermod 只是和add命令类似



#### 2 userdel

使用userdel命令可以在Linux系统中删除用 户账户，甚至连用户的主目录也一起删除。

```shell
# 删除用户moon，并且在删除该用户的同时一起删除主目录。
 userdel -r moon
```

### 2.4.3 组群账户

具有某种共同特征的用户集合就是组群。通过组 群可以集中设置访问权限和分配管理任务。在Linux系统中，有两种组群分类方法。 一种方法将组群分为私有组群和标准组群。

- 私有组群：当创建一个新的用户账户时，如果 没有指定该用户属于哪一个组群，那么Linux就会 创建一个和该用户同名的组群，这个组群就是私 有组群，在这个私有组群中只包含这个用户。
- 标准组群：标准组群也称为普通组群，标准组 群可以包含多个用户账户，如果使用标准组群， 那么在创建一个新的用户账户时，应该指定该用 户属于哪一个组群。

另外一种方法将组群分为主要组群和次要组群。

- 主要组群：当一个用户账户属于多个组成员时， 登陆后所属的组群便是主要群组，其它组群是次 要群组，一个用户账户只能属于一个主要组群。
- 次要组群：次要群组也称附加群组，一个用户 账户可以属于多个次要群组。

/etc/group文件的内容包括组群名、组群密码、GID及该 组群所包含的用户，每个组群一条记录，一行有4个段位， 每个段位用“:”分隔。 **zhangsan:x :1000:** （组名：组群密码：GID：族群成员 ）密码为x表示密码被映射到/etc/g

### 2.4.4 对应命令

#### 1 gpasswd

设置一个组群的组群密码，或者是在组群 中添加、删除用户。

```shell
#把用户it添加到kk组群中。
[root@rhel ~]# gpasswd -a it kk

```

#### 2 id

```shell
# 查询用户ab的UID、GID 以及归属组群的情况。
[root@rhel ～]# id ab

```

## 2.5 磁盘分区

​		在Linux系统中，如果需要在某个磁盘上存 储数据，则需要将磁盘进行分区，然后创 建文件系统，最后将文件系统挂载到目录 下才可以。在安装Linux系统后需要添加更多的交换空 间，可以通过添加一个交换分区或添加一 个交换文件来实现。

### 2.5.1 什么是磁盘分区

- 磁盘分区是指对硬盘物理介质的逻辑划分。将磁盘分成多 个分区，不仅仅利于对文件的管理，而且不同的分区可以 建立不同的文件系统，这样才能在不同的分区上安装不同 的操作系统。
- 磁盘分区一共有3种：主分区、扩展分区和逻辑驱动器。 扩展分区只不过是逻辑驱动器的“容器”，实际上只有主 分区和逻辑驱动器才能进行数据存储。**在一块磁盘上最多只能有四个主分区，可以另外建立一个扩展分区来代替四个主分区的其中一个，然后在扩展分区下可以建立更多的 逻辑驱动器。**

### 2.5.2 磁盘分区fdisk 举例

对硬盘上的剩余空间进行分区，创建两个逻辑驱动器，容量分别为1GB和3GB。

1， 由于当时装系统的时候，已经将磁盘的空间都用完了，所以不能给sda分配逻辑分区了，所以在虚拟机上在装一个磁盘sdb

![](./img/8.png)

2, 命令

```shell
fisk /dev/sdb m n e 回车 +5G 
m n l 回车 +1G
m n l 回车 +3G
w
```

![](./img/7.png)

### 2.5.3 创建文件系统

​		首先需要确认文件系统的类型，然后才能 挂载使用，通过mount加载或者通过修改 /etc/fstab文件开机自动加载都可以实现 该功能。

- 对一个新的磁盘进行分区以后，还要对这些分区进行格式 化并创建文件系统。一个分区只有建立了某种文件系统后， 这个分区才能使用。建立文件系统的过程，就是用相应的 格式化工具格式化分区的过程，这个过程和在Windows 系统中格式化某个分区为NTFS分区的过程类似。
- 文件系统是指文件在硬盘上的存储方法和排列顺序。在 Linux系统中，每个分区都需要一个文件系统，都有自己 的目录层次结构。Linux系统最重要特征之一就是支持多 种文件系统，这样它更加灵活，并可以和其他种操作系统 共存。
- 随着Linux系统的不断发展，它所支持的文件系统类型也 在迅速扩充，其中有XFS、ext4、JFS、ReiserFS、ext2、 ext3、ISO9660、MSDOS，VFAT、NFS等。
- **如果在计算机上新增加了一块硬盘，需要格式化 成 Linux文件系统，最好选择xfs或ext4文件系统。** 使用mkfs命令可以在分区上创建各种文件系统。 mkfs命令本身并不执行建立文件系统的工作，而 是去调用相关的程序来执行。这里的文件系统是 要指定的，比如xfs、ext4、ext3、vfat或者是 msdos等。

mkfs [选项] [设备]

```shell
#格式化/dev/sda5分区，创建ext4文件系统。对计算机硬盘分好分区之后重启计算机，使用如下命令创建文件系统。
[root@rhel ~]# mkfs -t ext4 /dev/sda5
# sdb5上面有别的文件系统，强制覆盖
mkfs -t xfs -f  /dev/sdb5

```

### 2.5.4 挂载和卸载文件系统

​		如果要挂载一个分区，首先需要确认文件 系统的类型，然后才能挂载使用，通过使 用mount和umount命令可以实现文件系统的 挂载和卸载。

mount [选项] [设备] [挂载目录] 

```shell
#挂载分区/dev/sda5到/mnt/kk目录中。
mount /dev/sda5 /mnt/kk

#以只读方式挂载/dev/sda5分区到/mnt/kk目录中。
[root@rhel ~]# mount -o ro /dev/sda5 /mnt/kk

#卸载分区/dev/sda5文件系统。
[root@rhel ~]# umount /dev/sda5
# 查看挂载情况
[root@rhel ~]# df

```

#### 1 查看磁盘分区挂载情况 df

​		要查看Linux系统上的磁盘分区挂载情况，可以使 用df命令来获取信息。使用df命令可以显示每个 文件所在的文件系统的信息，默认是显示所有文 件系统。检查文件系统的磁盘空间使用情况，利 用该命令获取硬盘使用了多少空间、目前还剩下 多少空间等相关信息。

df [选项] [文件]

#### 2 开机自动挂载文件系统

​		只有将某个分区或是设备挂载以后才能使 用，但是当计算机重新启动以后，又需要 重新挂载，这个时候可以通过修改 /etc/fstab文件实现开机自动挂载文件系统。

- /etc/fstab文件包含了所有磁盘分区以及存储设备的信息。 其中包含了磁盘分区和存储设备如何挂载，以及挂载在什 么目录上的信息。/etc/fstab文件是一个简单的文本文件， 必须要以root用户登录才可以编辑该文件。
- 由于每一台计算机系统的磁盘分区和设备属性不同，所以 /etc/fstab文件也不一样，但是基本的结构总是相似的。 每一行都包含着一个设备或磁盘分区的信息，每一行又有 多个列的信息。
- **/dev/sda5 /mnt/kk ext4 defaults 0 0**
  - **设备** 使用设备名称和UUID号表示都可以。如果要查看某 设备的UUID号，可以用下面命令： ls –l /dev/disk/by-uuid 或 blkid
  - **挂载目录** Linux系统为每个设备或分区设定了挂载目录。
  - **文件系统类型** Linux系统为每个设备或分区指定了文件系统类型。
  - **挂载选项**
    - （1）auto和noauto （2）user和nouser （3）exec和noexec （4）ro （5）rw （6）sync和async （7）defaults （8）owner
  - **转储选项** dump选项检查文件系统并用一个数字来决定该文 件系统是否需要备份。如果它是0，dump将会忽 略该文件系统，不做备份。
  - **文件系统检查选项**  fsck选项通过检验第6项中的数字来决定以何种 顺序检查文件系统，如果它是0，fsck将不检查 该文件系统。根文件系统（“/”）的默认值为1， 其他文件系统可以为2-9。

设置开机自动挂载文件系统

编辑/etc/fstab文件，在该文件末尾添加下 列内容。 **/dev/sda5 /mnt/www xfs defaults 1 2**

### 2.5.5 使用交换空间

​		用户有时需要在安装Linux系统后添加更多的交换 空间，可以通过添加一个交换分区（推荐优先使 用）或添加一个交换文件来实现。交换空间的总 大小一般为计算机物理内存的1～2倍左右，计算 机物理内存越大，倍数越小。

#### 1 添加交换分区

```shell
# 创建交换分区
[root@rhel ~]# mkswap /dev/sda5
# 启用交换分
[root@rhel ~]# swapon /dev/sda5
# 查看内存容量
free -th

#确认已经启用交换分区
cat /proc/swaps

#编辑/etc/fstab
#文件如果要在系统引导时启用交换分区，编辑/etc/fstab文件添加.如下内容。然后在系统下次引导时，就会启用新建的交换分区
/dev/sda5 swap swap defaults 0 0

#删除交换分区

#禁用交换分区
[root@rhel ~]# swapoff /dev/sda5
# 然后删除/etc/fstab里面指定的内容

```

#### 2 添加交换文件

1， 创建/swapfile文件。将大小乘以1024来判定块的大小。例如，大小为68MB的交换文件的块 大小为65536。在Shell提示下以root用户身份输入以下命令，其中的count等于想要输 入的块大小。

```shell
 #Linux dd命令用于读取、转换并输出数据。dd可从标准输入或文件中读取数据，根据指定的格式来转换数据，再输出到文件、设备或标准输出。
 #if=文件名：输入文件名，默认为标准输入。即指定源文件。of=文件名：输出文件名，默认为标准输出。即指定目的文件。bs=bytes：同时设置读入/输出的块大小为bytes个字节。count=blocks：仅拷贝blocks个块，块大小等于ibs指定的字节数。//2097152  2G  4194304 4G
 dd if=/dev/zero of=/swapfile bs=1024 count=1048576  
```

2, 	创建交换文件

```shell
[root@rhel ~]# mkswap /swapfile
```

3, 	启用交换文件

```shell
swapon /swapfile
```

4, 查看交换文件是否启用

```shell
free -th
```

5, 	编辑/etc/fstab文件 添加如下内容

```
/swapfile swap swap defaults 0 0
```

6	禁用交换文件

```shell
[root@rhel ~]# swapoff /swapfile
```

7	删除/swapfile文件.

```shel;
[root@rhel ~]# rm -rf /swapfile
```

8, 	编辑/etc/fstab文件,删除对应的内容



## 2.6 软件包管理

​		在Linux系统中，最常用的软件包是RPM包和 tar包。要管理RPM软件包可以使用rpm和yum 命令，**yum命令自动化地收集RPM软件包的相 关信息，检查依赖性，并且一次安装所有依赖的软件包，无需繁琐地一次次安装。**

### 2.6.1 RPM软件包管理

#### 1 概念

- RPM（Red Hat Package Manager，Red Hat软 件包管理器）是一种开放的软件包管理系统，按 照GPL条款发行，可以运行于各种Linux系统上。
- RPM 维护一个已经安装软件包和它们的文件的数据库， 因此，可以在系统上使用查询和校验软件包功能。
- RPM允许把软件编码包装成源码包和程序包，然 后提供给终端用户，这个过程非常简单，这种对 用户的纯净源码、补丁和建构指令的清晰描述减 轻了发行软件新版本所带来的维护负担。**Linux系 统上的所有软件都被分成可被安装、升级或卸载 的RPM软件包。**

#### 2 命令

使用rpm命令可以在Linux系统中安装、删 除、刷新、升级、查询RPM软件包。

##### 1 安装RPM软件包

rpm -ivh [RPM软件包文件名称]

```shell
 #安装bind-9.9.4-29.el7.x86_64.rpm软件包。
 rpm -ivh bind-9.9.4-29.el7.x86_64.rpm
 # 在软件包bind-9.9.4-29.el7.x86_64.rpm已经安装的情况下仍旧安装该软件包。
 rpm -ivh --replacepkgs bind-9.9.4-29.el7.x86_64.rpm
 
```

##### 2	删除RPM软件包

rpm -e [RPM包名称]

```shell
#删除bind-chroot软件包。
rpm -e bind-chroot
```

##### 3 	升级RPM软件包

使用rpm -Uvh命令可以在Linux系统中升级 RPM软件包，升级软件包实际上是删除和 安装的组合。不管该软件包的早期版本是 否已被安装，升级选项都会安装该软件包。

rpm -Uvh [RPM软件包文件名称]

```shell
#升级bind-9.9.4-29.el7.x86_64.rpm软件包，升级软件包实际上是删除和安装的组合。
rpm -Uvh bind-9.9.4-29.el7.x86_64.rpm 
```

##### 4	刷新软件包

​		使用rpm -Fvh命令可以在Linux系统中刷新RPM 软件包。使用RPM刷新软件包时，系统会比较指 定的软件包的版本和系统上已安装的版本。**当 RPM的刷新选项处理的版本比已安装的版本更新， 它就会升级到更新的版本。如果软件包先前没有 安装**，RPM的刷新选项将**不会安装该软件包**，这 和RPM的升级选项不同。

rpm -Fvh [RPM软件包文件名称]

```shell
#  刷新bind-9.9.4-29.el7.x86_64.rpm软件包。
[root@rhel Packages]# rpm -Fvh bind-9.9.4-29.el7.x86_64.rpm
```

##### 5	查询

rpm -q [RPM包名称]  	查询指定RPM软件包是否已经安装

 rpm –qa 	 查询系统内所有已安装的RPM软件包。

rpm -qi [RPM包名称]  查询已安装RPM软件包的 描述信息

rpm -ql [RPM包名称] 	查询指定已安装RPM软件包 所包含的文件列表

rpm -qR [RPM包名称] 	查询RPM软件包的依赖关系

rpm -qf [文件名]	查询系统中指定文件属于哪个 RPM软件包

### 2.6.2 使用yum管理RPM软件包

​		在Linux系统中安装软件包使用rpm命令， 但是使用rpm命令安装软件包特别的麻烦， 原因在于需要手动寻找安装该软件包所需 要的一系列依赖关系。当软件包不用时需 要卸载的话，由于卸载掉了某个依赖关系 而导致其它的软件包不能用。在Linux系统 中使用yum命令，令Linux的软件安装变得 简单。

#### 1 概念

- yum（Yellow dog Updater Modified）起初是由 Terra Soft研发，其宗旨是自动化地升级、安装和删除RPM软件包，收集RPM软件包的相关信息， 检查依赖性并且一次安装所有依赖的软件包，无须繁琐地一次次安装。
- yum的关键之处是要有可靠的软件仓库，软件仓 库可以是HTTP站点、FTP站点或者是本地软件 池，但必须包含rpm的header，header包括了 RPM软件包的各种信息，包括描述、功能、提供 的文件以及依赖性等。正是收集了这些header并 加以分析，才能自动化地完成余下的任务。

特点

- 可以同时配置多个软件仓库；
- 简洁的配置文件/etc/yum.conf；
- 自动解决安装或者删除RPM软件包时遇到 的依赖性问题；
- 保持与RPM数据库的一致性。

#### 2 yum软件仓库配置文件

​		repo文件是Linux系统中yum源（软件仓库） 的配置文件，通常一个repo文件定义了一 个或者多个软件仓库的细节内容，比如从 哪里下载需要安装或者升级的软件包， repo文件中的设置内容将被yum读取和应 用。软件仓库配置文件默认存储在 **/etc/yum.repos.d**目录中。

#### 3 yum命令使用

 使用yum命令可以安装、更新、删除、显 示软件包。yum可以自动进行系统更新， 基于软件仓库的元数据分析，解决软件包 依赖性关系。

##### 使用yum命令进行软件管理

1.yum help 显示使用信息 
2.yum list 列出软件包 
3.yum search keyword 搜索关键字 
4.yum info packagename 列出软件包详细信息 
5.yum install packagename 安装软件包 
6.yum remove packagename 删除软件包 
7.yum update packagename 升级软件包

##### 使用yum命令安装软件包组

1.yum grouplist 列出所有可用组 

2.yum groupinfo 提供特定组的信息 

3.yum groupinstall 安装软件包组 

4.yum groupupdate 更新软件包组 

5.yum grouperase 删除软件包组

##### 使用yum命令安装本地rpm包

yum localinstall *.rpm 

##### 例子

```shell
【例9.16】无需确认、直接安装bind软件包。
[root@rhel ~]# yum -y bind
【例9.17】显示bind软件包的详细信息。
[root@rhel ~]# yum info bind
【例9.18】显示所有已经安装的软件包信息。
[root@rhel ~]# yum info installed
【例9.19】列出bind软件包。
[root@rhel ~]# yum list bind
【例9.20】列出bind软件包的依赖关系。
[root@rhel ~]# yum deplist bind
【例9.21】显示软件仓库的配置。
[root@rhel ~]# yum repolist
【例9.22】查看/etc/named.conf文件是属于哪个软件包的。
[root@rhel ~]# yum provides /etc/named.conf
【例9.23】删除bind软件包。
[root@rhel ~]# yum remove bind 
```

### 2.6.3 tar 

```shell
#备份/root/abc目录及其子目录下的全部文件，备份文件名为abc.tar。
#-c或--create 建立新的备份文件。
#-v或--verbose 显示指令执行过程。
#-f<备份文件>或--file=<备份文件> 指定备份文件。
[root@rhel ~]# tar cvf abc.tar /root/abc

#查看abc.tar备份文件的内容，并显示在显示器上。
#-t或--list 列出备份文件的内容。
[root@rhel ~]# tar tvf abc.tar

#将打包文件abc.tar解包出来。
#-x或--extract或--get 从备份文件中还原文件。
[root@rhel ~]# tar xvf abc.tar

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

#### tar调用bzip2

使用tar命令可以在归档或者是解包的同时 调用bzip2压缩程序。以“.bz2”结尾的文 件就是bzip2压缩的结果。与bzip2相对应 的解压缩程序是bunzip2。tar命令中使用-j 选项来调用bzip2。

```shell
#将目录/root/abc及该目录所有文件压缩成abc.tar.bz2文件。
[root@rhel ~]# tar jcvf abc.tar.bz2 /root/abc
```

## 2.7 日常管理和维护

### 2.7.1 进程管理

#### 进程和程序的区别

​		程序 只是一个静态的命令集合，不占系统的运行资源； 而进程是一个随时都可能发生变化的、动态的、 使用系统运行资源的程序。一个程序可以启动多个进程。

#### 进程的特征

- 动态性：进程的实质是程序在多道程序系统中的一次执行过程，进程是动态产生、动态消亡的。
-  并发性：任何进程都可以同其它进程一起并发执行。
- 独立性：进程是一个能独立运行的基本单位，同时也是**系统分配资源和调度的独立单位**。
- 异步性：由于进程间的相互制约，使得进程具有执行的间 断性，即进程按各自独立的、不可预知的速度向前推进。
- 结构特征：进程由**程序、数据和进程控制块**三部分组成。
- 多个不同的进程可以包含相同的程序：一个程序在不同的 数据集里就构成不同的进程，能得到不同的结果；但是执 行过程中，程序不能发生改变。

### 2.7.2 查看进程 

#### ps

​		使用该命令可以确定有哪些进 程正在运行以及进程运行的状态、进程是 否结束、进程有没有僵死，以及哪些进程 占用了过多的资源等。

```shell
#显示所有进程。
[root@rhel ~]# ps –e

#显示所有不带控制台终端的进程，并显示用户名和进程的起始时间。
[root@rhel ~]# ps -aux

#查看less进程是否在运行。
[root@rhel ～]# ps -ax|grep less

#显示用户名和进程的起始时间。
[root@rhel ~]# ps -u
#显示用户root的进程。
[root@rhel ~]# ps -u root

#显示tty1终端下的进程。
[root@rhel ~]# ps -t tty1

#显示进程号为1659的进程。
[root@rhel ~]# ps -p 1659


```

#### top

​		包括它们的内存和CPU使用量。执行top命令可以显示目 前正在系统中执行的进程，并通过它所提 供的互动式界面，用热键加以管理。要退 出top，按[q] 键即可。

#### 杀死进程 kill

​			要关闭某个应用程序可以通过杀死其进程的方式 实现，如果进程一时无法杀死，可以将其强制杀 死。使用kill命令可以杀死进程。在使用kill命令之 前，需要得到要被杀死的进程的PID（进程号）。 **用户可以使用ps命令获得进程的PID，然后用进 程的PID作为kill命令的参数。**

 kill –9 1659

强制杀死进程号为1659的进程。

### 2.7.3 任务计划

​		如果要在固定的时间上触发某个作业，就 需要创建任务计划，按时执行该作业，在 Linux系统中常用cron实现该功能。使用cron实现任务自动化可以通过**修改 /etc/crontab文件以及使用crontab**命令实 现，其结果是一样的。

#### /etc/crontab文件详解

- root用户通过修改/etc/crontab文件可以实现任务 计划，而普通用户却无法修改该文件。crond守 护进程可以在无需人工干预的情况下，根据时间 和日期的组合来调度执行重复任务。

- /etc/crontab文件是cron的默认配置文件，前面3 行是用来配置cron任务运行环境的变量。Shell变 量的值告诉系统要使用哪个Shell环境（在这个例 子里是/bin/bash）。PATH变量定义用来执行命 令的路径。cron任务的输出被邮寄给MAILTO变 量定义的用户名。如果MAILTO变量被定义为空 白字符串，电子邮件就不会被寄出。

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

##### /etc/crontab文件配置举例

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

##### /etc/cron.d目录

- 除了通过修改/etc/crontab文件实现任务计 划之外，还可以在/etc/cron.d目录中创建文 件来实现。
- 该目录中的所有文件和/etc/crontab文件使 用一样的配置语法。

#### crontab命令简介

- root以外的用户可以使用crontab命令配置cron任务。所有 用户定义的crontab都被保存在/var/spool/cron目录中，并使用创建它们的用户身份来执行。
- 以某位用户身份创建一个crontab项目，登录为该用户， 然后输入crontab -e命令，使用由VISUAL或EDITOR环境变量指定的编辑器来编辑该用户的crontab。该文件使用 的格式和/etc/crontab相同。当对crontab所做的改变被保存后，该crontab文件会根据该用户名被保存在 /var/spool/cron/文件中。

##### 创建crontab 

​		创建新的crontab，然后提交给crond进程。 同时，新创建crontab的一个副本已经被放 在/var/spool/cron目录中，文件名就是用户 名。

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

##### 编辑crontab 

- 如果希望添加、删除或编辑 /var/spool/cron/zhangsan文件，可以使用vi编辑 器像编辑其它任何文件那样修改 /var/spool/cron/zhangsan文件并保存退出。如果 修改了某些条目或添加了新的条目，那么在保存 该文件时，crond会对其进行必要的完整性检查。 如果其中的某个地方出现了超出允许范围的值， 它会提示用户。
- 最好在/var/spool/cron/zhangsan文件的每一个条 目之上加入一条注释，这样就可以知道它的功能、 运行时间，更为重要的是，知道这是哪位用户的 作业。

##### 列出crontab 

```shell
#以root用户列出zhangsan的crontab。
[root@rhel ~]# crontab -u zhangsan -l

#以普通用户zhangsan列出自己的crontab。
[zhangsan@rhel ~]$ crontab -l

#对/var/spool/cron/zhangsan文件做备份。
[zhangsan@rhel ~]$ crontab -l >/home/zhangsan/zhangsancron

```

##### 删除crontab 

删除crontab时也会删除/var/spool/cron目 录中指定用户的文件。

```shell
#以用户root删除zhangsan的crontab。
[root@rhel ~]# crontab –u zhangsan -r

#以普通用户zhangsan删除自己的crontab。
[zhangsan@rhel ~]$ crontab -r
```

##### 恢复丢失的crontab文件

- 如果不小心误删除了crontab文件，且在主 目录下还有一个备份，那么可以将其复制 到/var/spool/cron/，其中 是用户名。

- 如果由于权限问题无法完成复制，可以使 用以下命令，其中需要指定在用户主目录 中复制的副本文件名。

- crontab [文件]

```shell
#以zhangsan用户登录恢复丢失的crontab文件。
[zhangsan@rhel ~]$ crontab –r
#删除crontab文件
[zhangsan@rhel ~]$ crontab -l
no crontab for zhangsan
[zhangsan@rhel ~]$ crontab  /home/zhangsan/zhangsancron
#恢复丢失的crontab文件
[zhangsan@rhel ~]$ crontab -l
8 * * * * cp /home/zhangsan/aa /home/zhangsan/bb
#恢复以后可以看到丢失的crontab文件条目
```

### 2.7.4 Linux系统启动过程

#### 1．BIOS自检 

#### 2．启动GRUB 2 

#### 3．加载内核 

#### 4．执行systemd进程 

- systemd进程是系统所有进程的起点，内核 在完成核内引导以后，即在本进程空间内 加载systemd程序。

- systemd进程是所有进程的发起者和控制者。 因为在任何基于Linux的系统中，它都是第 一个运行的进程，所以systemd进程的编号 (PID)永远是1。
- Systemd进程有以下两个作用：
  - 扮演最终父进程的角色。因为systemd进程永远不会被 终止，所以系统总是可以确信它的存在，并在必要的时 候以它为参照。**如果某个进程在它衍生出来的全部子进 程结束之前被终止，就会出现必须以systemd为参照的 情况。此时那些失去了父进程的子进程就都会以 systemd作为它们的父进程**
  - 在进入某个特定的服务启动集合，即是 /etc/systemd/system/default.target，它的这个作用是由 运行目标target定义的。

#### 5．初始化系统环境 

#### 6．执行/bin/login程序

#### 





# 3 工具

## 3.1 vim

### 3.1.1 光标移动

| 命令   | 解释                                               |
| ------ | -------------------------------------------------- |
| Ctrl+f | 使光标往下移动一页屏幕                             |
| Ctrl+b | 使光标往上移动一页屏幕                             |
| $      | 使光标移动到光标所在行的行尾                       |
| ^      | 使光标移动到光标所在行的行首                       |
| G      | 使光标移动到文件尾（最后一行的第一个非空白字符处） |
| gg     | 使光标移动到文件首（第一行第一个非空白字符处）     |

### 3.1.2 命令模式

| 命令                | 解释                                                         |
| ------------------- | ------------------------------------------------------------ |
| dd                  | 删除光标所在行                                               |
| ndd                 | 从光标所在行开始删除n行，n代表数字                           |
| d$                  | 删除光标到行尾的内容（含光标所在处字符）                     |
| yy                  | 复制光标所在行                                               |
| nyy                 | 复制从光标所在行开始的n行，n代表数字                         |
| p                   | 粘贴                                                         |
| /关键字             | 查找字符：先按[/]键，再输入想查找的字符，如果第一次查找的关键字不是想要的， 可以一直按[n]键会往后查找下一个关键字，而按[N]键会往相反的方向查找 |
| J                   | 清除光标所在行与下一行之间的换行，行尾没有空格的话会自动添加一个空格 |
| nJ                  | 将当前行开始的n行进行合并，n代表数字                         |
| :s/str1/str2/       | 将光标所**在行第一个字符**str1替换为str2，str1和str2代表字符 |
| :s/str1/str2/g      | 将光标**所在行所有的字符**str1替换为str2，str1和str2代表字符 |
| :n1,n2s/str1/str2/g | 用str2替换从第n1行到第n2行中出现的str1，str1和str2代表字符， n1和n2代表数字 |
| :% s/str1/str2/g    | 用str2替换文件中所有的str1，str1和str2代表字符。和  **[:1,$s/str1/str2/g]**等价 |
| :set number         | 在文件中的每一行前面列出行号                                 |

## 3.2 bc