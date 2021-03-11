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