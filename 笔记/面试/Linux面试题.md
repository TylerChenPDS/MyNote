# shell

1. 使用Linux命令查询file1**中空行所在的行号**

   ```
   awk '/^$/{print NR}' sed.txt 
   ```

2. 有文件chengji.txt内容如下

   ```
   张三 40
   李四 50
   王五 60
   ```

   使用Linux命令计算第二列的和并输出

   ```
   awk -F" " '{printf $2 " "} END {print ""}' chengji.tx | awk -v all=0 -F " " '{for(i=NF;i>0;--i) a=$i + a } {print a}'
   ```

   思路：1，先将40 50 60 放到一行打印出来。2，计算他们的和

   ```
   cat chengji.txt | awk -F " " '{sum+=$2} END{print sum}'
   150
   ```

   思路：读取每一行的时候，将第二列加到sum，然后再最后一行输出sum

3. Shell脚本里如何检查一个文件是否存在？如果不存在该如何处理？

   ```shell
   #!/bin/bash
   if [ -f $1 ]
   then
     echo "文件存在"
   else
     echo "文件不存在"
   fi
   
   ```

4. 用shell写一个脚本，对文本中无序的一列数字排序

   ```
   9
   8
   7
   6
   5
   4
   3
   2
   10
   1
   ```

   ```
   cat test.txt |grep -v '^$'| sort -n
   ```

5. 请用shell脚本写出查找当前文件夹（/home）下所有的文本文件内容中包含有字符”shen”的文件名称

   ```
   ll /home | grep 'gao' | tr -s ' '|awk -F" " '{print $9}'
   或者
   ll /home | grep 'gao' | tr -s ' '| cut -d" " -f9
   ```

   

# Fork()创建进程，写时复制

fork() ，创建一个新进程需要通过内核调用sys_clone来完成，大体的过程便是先对父进程的数据和环境进行复制，然后修改新进程的一些属性，比如pid号等，之后通过do_fork函数和ret_from_fork函数完成对新进程的创建以及运行。

传统的fork()系统调用直接把所有的资源复制给新创建的进程。这种实现过于简单并且效率低下。写时拷贝是一种可以推迟甚至避免拷贝数据的技术。内核此时并不复制整个进程的地址空间，而是让父子进程共享同一个地址空间。只用在需要写入的时候才会复制地址空间，从而使各个进行拥有各自的地址空间。也就是说，资源的复制是在需要写入的时候才会进行，在此之前，只有以只读方式共享。这种技术使地址空间上的页的拷贝被推迟到实际发生写入的时候。

# 进程管理

## 查看进程的命令

ps

查看自己的进程  ps -l

查看系统所有进程 ps aux

查看特定的进程 ps aux | grep threadx

pstree

查看所有进程树  pstree -A

top 实时显示进程信息。

查看占用端口的进程 netstat -anp | grep port

## 孤儿进程

一个父进程退出，而它的一个或多个子进程还在运行，那么这些子进程将成为孤儿进程。孤儿进程将被 init 进程（进程号为 1）所收养，并由 init 进程对它们完成状态收集工作。由于孤儿进程会被 init 进程收养，所以孤儿进程不会对系统造成危害。

## 僵尸进程

一个子进程的进程描述符在子进程退出时不会释放，只有当父进程通过 wait() 或 waitpid() 获取了子进程信息后才会释放。如果子进程退出，而父进程并没有调用 wait() 或 waitpid()，那么子进程的进程描述符仍然保存在系统中，这种进程称之为僵尸进程。要消灭系统中大量的僵尸进程，只需要将其父进程杀死，此时僵尸进程就会变成孤儿进程，从而被 init 进程所收养，这样 init 进程就会释放所有的僵尸进程所占有的资源，从而结束僵尸进程





# 内存管理

## 虚拟内存(Virt) & 常驻内存(Resident) & 共享内存 (Shared)

- 虚拟内存
  - 进程“需要的”虚拟内存大小，包括进程使用的库、代码、数据，以及malloc、new分配的堆空间和分配的栈空间等；
  - 假如进程新申请10MB的内存，但实际只使用了1MB，那么它会增长10MB，而不是实际的1MB使用量。
  - **VIRT = SWAP + RES**
- RES
  - 进程当前使用的内存大小，包括使用中的malloc、new分配的堆空间和分配的栈空间，但不包括swap out量；
  - 关于库占用内存的情况，它只统计加载的库文件所占内存大小。
  - 包含其他进程的共享；、如果申请10MB的内存，实际使用1MB，它只增长1MB，与VIRT相反；
  - RES = CODE + DATA
- **SHR：**
  - 除了自身进程的共享内存，也包括其他进程的共享内存；

## VSS、RSS、PSS和USS

VSS >= RSS >= PSS >= USS

- VSS - Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）

  VSS=进程x分配的内存+共享库所占用的内存

- RSS - Resident Set Size 实际使用物理内存（包含共享库占用的内存）

  RSS=进程x正在使用的内存+共享库所占的内存

- PSS - Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存）

  PSS=进程x正在使用的内存+共享库所占内存 / 共享库进程数

- USS - Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）

  USS=进程x正在使用的物理内存

![image-20210322110030772](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210322110030772.png)

- VSS (reported as VSZ from ps) is the total accessible address space of a process. This size also includes memory that may not be resident in RAM like mallocs that have been allocated but not written to. VSS is of very little use for determing real memory usage of a process.

- RSS is the total memory actually held in RAM for a process. RSS can be misleading, because it reports the total all of the shared libraries that the process uses, even though a shared library is only loaded into memory once regardless of how many processes use it. RSS is not an accurate representation of the memory usage for a single process.


- PSS differs from RSS in that it reports the proportional size of its shared libraries, i.e. if three processes all use a shared library that has 30 pages, that library will only contribute 10 pages to the PSS that is reported for each of the three processes. PSS is a very useful number because when the PSS for all processes in the system are summed together, that is a good representation for the total memory usage in the system. When a process is killed, the shared libraries that contributed to its PSS will be proportionally distributed to the PSS totals for the remaining processes still using that library. In this way PSS can be slightly misleading, because when a process is killed, PSS does not accurately represent the memory returned to the overall system.


- USS is the total private memory for a process, i.e. that memory that is completely unique to that process. USS is an extremely useful number because it indicates the true incremental cost of running a particular process. When a process is killed, the USS is the total memory that is actually returned to the system. USS is the best number to watch when initially suspicious of memory leaks in a process.
  

翻译如下

- VSS 是进程可以访问的地址空间。它大小还包括可能不在RAM中驻留的内存，比如已经分配但没有写入的mallocs。VSS在确定进程的实际内存使用情况方面用处不大
- RSS是一个进程在RAM中实际占有的总内存。RSS可能会产生误导，因为它报告进程使用的所有共享库的总数，即使一个共享库只被加载到内存一次，而不管有多少进程使用它。RSS并不是单个进程内存使用情况的准确表示。
- PSS与RSS的不同之处在于它报告了共享库的比例大小，也就是说，如果三个进程都使用了一个有30个页面的共享库，那么这个库将只为三个进程每个报告的PSS贡献10个页面。PSS是一个非常有用的数字，因为当把系统中所有进程的PSS加在一起时，它可以很好地表示系统中总的内存使用量。当一个进程被终止时，为其PSS贡献的共享库将按比例分布到仍然使用该库的剩余进程的PSS总数中。这样的话，PSS可能会有点误导人，因为当一个进程被杀死时，PSS并不能准确地表示返回给整个系统的内存。
- USS是一个进程的全部私有内存，也就是这个进程唯一的内存。USS是一个非常有用的数字，因为它指示了运行特定进程的真正增量成本。当进程被终止时，USS是实际返回给系统的总内存。当最初怀疑进程中的内存泄漏时，USS是最值得注意的数字。



# 参考

http://www.cyc2018.xyz/

https://blog.csdn.net/whbing1471/article/details/105523704

[写时复制]https://blog.csdn.net/u012317833/article/details/39160219