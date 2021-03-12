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

   

# Linux系统是怎么创建进程的？

fork() ，创建一个新进程需要通过内核调用sys_clone来完成，大体的过程便是先对父进程的数据和环境进行复制，然后修改新进程的一些属性，比如pid号等，之后通过do_fork函数和ret_from_fork函数完成对新进程的创建以及运行。



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



# 参考

http://www.cyc2018.xyz/