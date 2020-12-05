# 1， 安装

1、确保CentOS上安装好gcc：yum -y install gcc

2、确保CentOS上安装好gcc-c++：yum -y install gcc-c++

3、查看gcc的版本：gcc -v  有版本信息，安装对了

4、卸载老版本，卸载命令从官网上拷贝：

yum remove docker   docker-client  docker-client-latest  docker-common  docker-latest  docker-latest-logrotate  docker-logrotate  docker-engine

5、安装下面yum-config-manager命令的支持工具

yum install -y yum-utils

6、设置Docker的yum源，注意这里不要用官网上的仓库，服务器在国外，很慢，要用国内镜像，如阿里云：

yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

7、更新yum连接索引

yum makecache fast

8、安装Docker CE

yum -y install docker-ce docker-ce-cli containerd.io

9、启动Docker

systemctl start docker

systemctl status docker

10、测试安装好没的

docker version

# 2，docker 容器的使用

## 2.1 镜像加速器

进入阿里云->控制台->容器镜像服务->镜像中心->镜像加速器

```json
sudo mkdir -p /etc/docker
sudo tee /etc/docker/daemon.json <<-'EOF'
{
  "registry-mirrors": ["https://mwv1zevv.mirror.aliyuncs.com"]
}
EOF
sudo systemctl daemon-reload
sudo systemctl restart docker
```

## 2.2 拉取镜像

**docker pull [可选的选项参数] 镜像仓库名称:tag标记   ，  tag标记不写，默认latest**

docker image pull redis:6.0.5

docker pull hub.c.163.com/public/redis:2.8.4

## 2.3 查看镜像

docker  images 或 docker image ls

**images子命令主要支持如下选项:**

-a,  --all=“true | false”: 列出所有(包括临时文件)镜像文件，默认为否;

-f,   --filter : 过滤列出的镜像，如dangling=true只显示没有被使用的镜像;

找出tag为的

docker images -f  "dangling=true"

根据repository名称和tag模糊过滤

docker images --filter reference='busy*:*libc'

## 2.4 **使用tag命令添加镜像标签**

**docker tag ubuntu:latest myubuntu:latest** 

​		使用docker  tag命令来为本地镜像任意添加新的标签。再次使用docker images 列出本地主机上镜像信息，可以看到多了一个myubuntu:latest标签的镜像。

## 2.5 **搜索镜像**

docker search 命令可以搜索Docker Hub官方仓库中的镜像。

语法为：docker search  [option]  keyword

**搜索官方提供的带redis关键字的镜像，如下所示:**

```shell
docker search --filter=is-official=true redis
```

**搜收藏数量大于等于90的redis的镜像**

```shell
docker search --filter=stars=90 redis
```



## 2.6 **删除和清理镜像**

使用docker  rmi或docker  image  rm命令可以删除镜像，

命令格式为docker rmi IMAGE  [IMAGE...]， 其中IMAGE可以为**标签或ID**。-f，-force: 强制删除镜像，即使有容器依赖它;



使用Docker一段时间后，系统中可能会遗留一些临时的镜像文件，以及一些没有被使用的镜像，

可以通过docker  image prune 命令来进行清理。

**支持选项包括:**

-a, -all:删除所有无用镜像，不光是临时镜像;

-filter filter: 只清理符合给定过滤器的镜像;

-f，-force:强制删除镜像，而不进行提示确认。



```
docker rmi `docker images -aq` //删除所有镜像
```



## 2.7 创建镜像

docker [container] commit [OPTIONS]  CONTAINERID  [REPOSITORY[:TAG]]，

-a，--author="":作者信息;

-C，--change=[] :提交的时候执行**Dockerfile**指令

包括CMD | ENTRYPOINT |ENV | EXPOSE |LABEL | ONBUILD |USER | VOLUME WORKDIR 等;

-m，--message="":提交消息;

-p，--pause=true:提交时暂停容器运行。

```shell
docker container commit -a "zhonghao" -m "创建新镜像测试" 98da88460690 contest:1
```

## 2.8 **导出和载入镜像**

**导出本地的hello-world:latest镜像为文件hhh.tar**

```shell
docker save -o hhh.tar hello-world:latest
```

生成的hhh.tar可以分享给他人，这个文件就是导出的镜像文件了。

**从文件hhh.tar导人镜像到本地镜像列表**

```shell
docker load -i hhh.tar
//或者
docker load < hhh.tar
```

这将导入镜像及其相关的元数据信息(包括标签等)。导人成功后，可以使用docker  images命令进行查看。

## 2.9 上传镜像到阿里云

![](https://gitee.com/CTLQAQ/picgo/raw/master/33.png)在容器镜像服务中，创建镜像，然后点击管理，里面有详细步骤。

## 2.10 **创建本地私有仓库和上传镜像**

```shell
docker run -d -p 5000:5000 --restart=always --name registry2 registry:2
```

这样一个docker私服就已经创建并启动好了。

**将镜像上传到docker私服**

```shell
docker tag centos:7 localhost:5000/centos:7
docker push localhost:5000/centos
```

**获取私有仓库镜像**

```shell
docker pull 127.0.0.1:5000/centos:7
docker pull localhost:5000/centos:7
docker pull 192.168.17.128:5000/centos:7
```

**用ip地址，报错的话尝试：**

**在/etc/docker下的daemon.json**

```json
{
	"insecure-registries":["192.168.17.128:5000"]
}
```

```shell
systemctl daemon-reload
systemctl restart docker
```

**修改了配置文件要重启docker**

**查看私服镜像所有仓库**

```shell
curl http://localhost:5000/v2/_catalog
```

**查看仓库中镜像的所有标签列表**

```shell
curl http://localhost:5000/v2/centos/tags/list
```

# 3 docker 容器的使用

## 3.1 创建和运行

docker [container] create 命令新建一个容器。docker  [container]  start 命令来启动它。y

```shell
docker create -it centos:7
docker run -dit --name centos1  centos:7  /bin/bash   //将create 和 start并起来
```

其中，-t选项让Doker分配一个伪终端并绑定到容器的标准输入上, -i主则让容器的标准输人保持打开。需要让Docker容器在后台以守护态(Daemonized)形式运行,此时，可以通过**添加-d参数**来实现。

**bugs:**

**1、创建运行容器的时候，报警告信息：WARNING: IPv4 forwarding is disabled. Networking will not work.**

​		出于安全考虑，Linux系统默认是禁止数据包转发的。所谓转发即当主机拥有多于一块的网卡时，其中一块收到数据包，根据数据包的目的ip地址将包发往本机另一网卡，该网卡根据路由表继续发送数据包。这通常就是路由器所要实现的功能。配置Linux系统的ip转发功能，首先保证硬件连通，然后打开系统的转发功能/proc/sys/net/ipv4/ip_forward，该文件内容为0，表示禁止数据包转发，1表示允许，将其修改为1。

vim  /usr/lib/sysctl.d/00-system.conf

加入  net.ipv4.ip_forward=1

systemctl restart network，最好也重启docker服务：systemctl restart docker



## 3.2 容器退出

- exit：停止运行中的容器，再退出伪终端
- 快捷键：ctrl+p+q，不停止容器实例的运行，退出伪终端，这种方法退出后怎么再进去

## 3.3 **终止容器**

​		可以使用**docker  [container]  stop** 来终止一个运行中的容器。该命令会首先向容器发送SIGTERM信号，等待一段超时时间后(默认为10秒)，再发送SIGKILL信号来终止容器:

​		此外，还可以通过**docker [container] kill**直接发送 SIGKILL信号来强行终止容器。

## 3.4 **进入容器**

​	在**使用-d参数**让容器启动后会进入后台，**或者使用ctrl+q+p暂时退出容器，**用户都将无法看到容器中的信息，也无法进行操作。这个时候如果需要进入或重新进入容器进行操作，应该使用**attach或exec**命令。

```shell
docker attach 213wqeq213ddsadasdsaas
```

​		使用attach命令有时候并不方便。当多个窗口同时attach到同一个容器的时候，所有窗口都会同步显示;当某个窗口因命令阻塞时，其他窗口也无法执行操作了。

```
docker exec -it  243c32535da7  /bin/bash
```

## 3.5 删除容器

```shell
docker ps -a
docker rm xxxxxx
```

两条命令实现停用并删除容器：

```shell
docker kill $(docker ps -q) 
docker rm $(docker ps -aq)
```

## 3.6 **导入导出容器**

​		导出容器是指，导出一个已经创建的容器到一个文件，**不管此时这个容器是否处于运行状态。**可以使用docker [container] export 命令，该命令格式为: docker [container] export [-o|--output [=""]]  CONTAINER
其中，可以通过-o选项来指定导出的tar文件名。

```shell
$ docker ps -a
$ docker export  -o test.tar  xxxxxx
```

**docker  commit和docker export命令的主要区别总结：**

1）docker export不管容器运行否都可以导出，docker  commit只有运行中的容器才可以使用。

2）docker export生成的容器快照文件将丟弃所有的历史记录和元数据信息(即仅保存容器当时的快照状态),docker  commit生成的镜像存储文件将保存完整记录，体积更大。

​		导出的文件又可以使用docker [container] import 命令导入变成镜像，该命令简单用法为:docker import  xxx.tar

```shell
$ docker import  test.tar  
$ docker tag xxxxx   ctos:7  /bin/bash
```

注意：导出的容器快照文件，会丢失元数据信息，所以，我们在基于导入的镜像创建新容器的时候，docker run命令，不能省略后面的子命令。对于centos镜像来讲子命令:/bin/bash

## 3.7 **查看容器信息**

```shell
$ docker  inspect  test/123dsa123rasd4
```

查看某容器的具体信息，会以json格式返回包括容器Id、创建时间、路径、状态、镜像、配置等在内的各项信息

```shell
$ docker top test
```

这个子命令类似于Linux系统中的top命令，会打印出容器内的进程信息，包括PID、用户、时间、命令等。



docker  [container]  stats   [OPTIONS] [CONTAINER...]子命令，会显示CPU、内存、存储、网络等使用情况的统计信息。支持选项包括:

-a, -all:输出所有容器统计信息，默认仅在运行中;

-no-stream:不持续输出，默认会自动更新持续实时结果;

-no- trunc:不截断输出信息。

## 3.8 **复制文件到容器和复制文件到宿主机**

```shell
docker   cp [OPTIONS] CONTAINER:SRC_PATH   DEST_ PATH
```

-a, -archive: 打包模式，复制文件会带有原始的uid/gid信息;

-L，-follow-link :跟随软连接。当原路径为软连接时，默认只复制链接信息，使用该选项会复制链接的目标内容。

# 4 数据卷、数据容器

## 4.1 数据卷

​		数据卷(Data Volumes)是一个在容器内的特殊目录，  这个目录和宿主机里的某个目录有特殊的映射关系，类似于Linux中的**mount**行为。

**数据卷可以提供很多有用的特性:**

- 数据卷可以在容器之间共享和重用，容器间传递数据将变得高效与方便;
- 对数据卷内数据的修改会立马生效，无论是容器内操作还是宿主机本地操作;
- 对数据卷的更新不会影响镜像，解耦开应用和数据;
- 数据卷会一直存在，直到没有容器使用，可以安全地卸载它。

### 4.1.1 **创建数据卷**

```shell
docker volume create   test 
docker volume ls
docker volume inspect //命令查看详情。
```

### 4.1.2 删除数据卷

docker volume prune 会删除未装入到某个容器或者服务的所有卷，所以谨慎使用！

docker volume rm 允许删除指定卷。

### 4.1.3 使用数据卷

用docker [container] run 命令的时候，可以使用**--mount**选项来使用数据卷。

**--mount选项支持三种类型的数据卷，包括:**

- volume:普通数据卷，映射到主机/var/lib/docker/volumes路径下; 
- bind:绑定数据卷，映射到主机指定路径下;
- tmpfs:临时数据卷，只存在于内存中。

```shell
docker container run -dit --name centos  --mount  type=volume, source=testvolume, destination=/test  centos:7
```

```shell
docker container run -dit --name centos1  --mount  type=bind,source=/root/abc,destination=/test centos:7
```

**上述type=bind的命令等同于下面的写法 :**

```shell
docker container run -dit --name centos2  -v  /root/abc:/test/  centos:7
```

```shell
docker run -it -v /dbdata --name dbdata centos:7 
```

**这里没明确指定宿主机中的目录，会在/var/lib/docker/volumes下自动生成，名字随机**

**注意，地目录的路径必须是绝对路径，容器内路径可以为相对路径。果目录不存在，Docker会自动创建。**

## 4.2 **数据卷容器**

​		如果用户需要在多个容器之间共享一些持续更新的数据，最简单的方式是使用数据卷容器。数据卷容器也是一个容器，**但是它的目的是专门提供数据卷给其他容器挂载。**

首先，创建一个数据卷容器dbdata,并让宿主机创建一个数据卷挂载到centos容器里的目录/dbdata:

```shell
$ docker run -it -v /dbdata --name dbdata centos:7   
```

然后，可以在其他容器中使用--volumes-from来挂载dbdata容器中的数据卷，

```shell
$ docker run -it --volumes-from dbdata --name db1 centos:7   
$ docker run -it --volumes-from dbdata --name db2 centos:7   
```

​		此时，容器db1和db2都挂载同一个数据卷到相同的/dbdata目录，三个容器任何一方在该目录下的写人，其他容器都可以看到。如果删除了挂载的容器(包括dbdata、db1和db2 ),数据卷并不会被自动删除。

### 4.2.1 **利用数据卷容器来迁移数据**

**可以利用数据卷容器对其中的数据卷进行备份、恢复，以实现数据的迁移。**

**1、备份**

使用下面的命令来备份dbdata数据卷容器内的数据卷:

```shell
$ docker  run --volumes-from dbdata -v  $(pwd):/backup --name worker centos:7  tar  cvf  /backup/backup.tar  /dbdata
```

首先利用 centos:7镜像创建了一个容器worker。

使用--volumes-from dbdata 参数来让worker容器挂载dbdata容器的数据卷(即dbdata 数据卷);

使用-v $(pwd):/backup参数来挂载本地的当前目录到worker容器的/backup目录。

worker容器启动后，使用tar cvf /backup/backup.tar  /dbdata 命令将/bdata下内容备份为容器内的/backup/backup.tar,

即宿主主机当前目录下的backup.tar

**2、恢复**

如果要恢复数据到一个容器，可以按照下面的操作。

然后创建另一个新的容器，并使用untar解压备份文件到所挂载的容器卷中:

```shell
$ docker run --volumes-from dbdata -v $(pwd):/backup  centos:7  tar xvf  /backup/backup.tar -C /
```

# 5 端口映射与容器互联

Docker提供的**两个很方便的功能，能满足服务访问的基本需求:**

- 一个是允许映射容器内应用的服务端口到本地宿主主机;
- 另一个是互联机制实现多个容器间便捷互访，多个容器中应用进行快速交互。

## 5.1 **从外部访问容器**

```shell
docker run -d -p 8081:8080 tomcat:8.0.52
```

**小p后面跟着： 宿主机端口号：容器里的应用使用的端口号**

```shell
docker run -d -P tomcat:8.0.52
```

​     **//大P表示： 宿主机端口号取随机：容器里的应用使用的端口号**

```shell
docker run -d -p 8084:8080 -p 8082:8080  tomcat:8.0.52
```

**可以设置多组小p，映射多对端口号**

```shell
docker port 346cf7a666b5
//或者
docker ps
```

**查看端口配置**

```shell
docker inspect 346cf7a666b5
```

**创建出来的容器是有自己的内部网络和IP地址，使用docker [container] inspect + 容器ID可以获取容器的具体信息。**

## 5.2 **容器互联**

​		容器的互联(linking)是一种让**多个容器中的应用进行快速交互的方式。**它会在两个互联的容器之间创建连接关系，不用通过把端口映射到宿主机，通过宿主机作为桥梁来进行交互。

**使用--link参数可以让容器之间安全地进行交互。**

```shell
docker run --name db  -e MYSQL_ROOT_PASSWORD=admin  -d mysql:5.7 
docker run -d -p 80:8080 --name web --link db:dbbm  tomcat:8.0.52
```

**此时，db容器和web容器建立互联关系。  --link 参数的格式为--link name:alias，其中name是要链接的容器的名称，alias是别名。**

**cat /etc/hosts** //查看是否建立成功

![](https://gitee.com/CTLQAQ/picgo/raw/master/34.png)



# 6 Dockerfile

## 6.1 基础知识

**我们可以使用Dockerfile来快速创建自定义的镜像。**

**命指令句有分为:**

- 配置指令
- 操作指令

**指令语句的基础知识：**

1：每条保留字指令都必须为大写字母且后面要跟随至少一个参数

2：指令按照从上到下，顺序执行

3：#表示注释

4：每条指令的执行都会创建一个新的镜像层

**Dockerfile来创建镜像的过程：**

- **新建一个目录/root/dockertest，作为我的上下文目录，一般情况下我们会默认放Dockerfile文件放这个目录**

- **新建和编写Dockerfile文件**

  FROM centos:7     //我们自己新建的镜像，一般都是在一个基础镜像上面，加工改进而来的。

  VOLUME ["/votest1","/votest2"]     //创建数据卷。

  CMD ["/bin/bash"]    //镜像被docker run命令创建并启动起来后，默认执行的第一条命令。

- **用docker build命令创建新镜像放到本地镜像仓库**

  ```shell
  docker build  -t  builder/firstimage:1.0 .
  ```

  -t  builder/firstimage:1.0：指定了最终生成的镜像的名称

  该命令将读取宿主机上指定的上下文路径下(包括子目录)的Dockerfile，默认情况下， Dockerfile放在上下文目录里，

  **注意后面还有个点儿，表示将当前目录作为Dcoker构建镜像的上下文的目录，**

- **刚刚通过Dockerfile新建的新的镜像，就可以run起来**

  docker run -it  builder/firstimage:1.0

## 6.2 配置指令

![](https://gitee.com/CTLQAQ/picgo/raw/master/35.png)

### 6.2.1 **ARG和FROM**

**ARG**定义创建镜像过程中使用的变量。

格式为ARG < name> [ =default value]。

在执行docker build命令时， 可以通过**--build-arg <参数名>=<值>**来**覆盖**Dockerfile 中定义的变量值，${变量名}来取值。当镜像编译成功后，ARG指定的变量将不再存在。

**FROM**指定所创建镜像的基础镜像。

```
FROM <image> [AS <name>] 或 FROM < image>:<tag> [AS <name>] 
```

例子：

```dockerfile
ARG VERSION=6
FROM centos:${VERSION}
```

```shell
docker build --build-arg VERSION=7  -t  builder/firstimage:1.0 .
```

### 6.2.2 **LABEL**

LABEL指令可以为生成的镜像添加元数据标签信息。

```dockerfile
LABEL <key>=<value> <key>=<value> <key>=<value>

#such as:
LABEL version="1.0.0"
LABEL author="发发编程"  date="2020-06-22"
LABEL description="Message ..... "
```

这些信息可以用来辅助过滤出特定镜像

```shell
docker images --filter "label=author=发发编程"
```

**注意：**

**1、等式中等号两边不能有空格**

**2、每一个LABEL指令都会生成一个镜像层，在实践中，最好把多个标签合并成一个LABEL指令。**

### 6.2.3 **EXPOSE** 

功能为暴漏容器运行时的监听端口给外部。

```dockerfile
EXPOSE  <port>  [<port>/<protocol>...]

# 例如
EXPOSE 22 80 8443
```

### 6.2.4 **ENV**

指定环境变量，在镜像生成过程中会被后续RUN指令使用，并且通过镜像启动的容器中也会存在。

```dockerfile
ENV <key> <value> 或ENV <key>=<value> 

##例如
ENV APP_VERSION=1.0.0
ENV APP_HOME=/usr/local/app
ENV PATH $PATH:/usr/local/bin11    
```

指令指定的环境变量在**运行时**可以被覆盖掉，如**docker run --env  =  built_image**，和前面的ARG相似。

**注意：**当一条ENV指令中同时为多个环境变量赋值并且值也是从环境变量读取时，**会为变量都赋值后再更新**。

如下面的指令:

ENV key1=value2

ENV key1=value1  key2=${key1}

**最终结果为key1=value1  key2=value2**

### 6.2.5 **ENTRYPOINT** 

Dockerfile中有ENTRYPOINT ["tail","-f","/usr/local/aaa"]这句，那么你启动的时候镜像就执行了这个里面的内容。在启动容器的时候提供 **--entrypoint** 选项，会覆盖**Dockerfile中有ENTRYPOINT**。

**ENTRYPOINT和CMD一般情况下是结合起来用：**

```dockerfile
FROM  centos
ENTRYPOINT ["/bin/ls"]
CMD ["-l", "/tmp"]  
```

相当于：

```shell
docker run --entrypoint=/bin/ls centos:7 -l /tmp       
```

当指定了 ENTRYPOINT 后，CMD 的含义就发生了改变，不再是直接的运行其命令，而是将 CMD 的内容作为参数传给 ENTRYPOINT 指令，换句话说实际执行时，将变为：< ENTRYPOINT> "< CMD>"

**和CMD一样，每个Dockerfile中只能有一个ENTRYPOINT，当指定多个时，只有最后一个起效。**

### 6.2.6 **VOLUME**

创建一个数据卷挂载点。

格式为VOLUME ["/data"] 。

运行容器时可以从本地主机或其他容器**挂载数据卷**，一般用来存放数据库和需要持久保持的数据等。

### 6.2.7 **WORKDIR**

指定工作目录

格式为 WORKDIR < 工作目录路径>

使用 WORKDIR 指令可以来指定工作目录（或者称为当前目录），后面的指令工作的当前目录就是这个指定的目录，如该目录不存在，WORKDIR 会帮你建立目录。

**还有一个作用，我们docker run 启动容器后，默认所在的目录，也会是这个指定的目录。**

### 6.2.8 **USER**

指定当前用户

USER 指令是改变环境状态并影响以后的层。USER 改变之后层的执行 RUN, CMD 以及 ENTRYPOINT 这类命令的身份。USER 只是帮助你切换到指定用户而已，这个用户必须是事先建立好的，否则无法切换

![](https://gitee.com/CTLQAQ/picgo/raw/master/36.png)

实战中，建立 redis 用户后，我们推荐使用 gosu 来切换另一个用户

```dockerfile
RUN groupadd -r redis && useradd -r -g redis redis \
	wget -O /usr/local/bin/gosu \ "https://github.com/tianon/gosu/releases/download/1.7/gosu-amd64" \
	 && chmod +x /usr/local/bin/gosu \
	 && gosu nobody true
CMD [ "exec", "gosu", "redis", "redis-server" ]
```

### 6.2.9 **ONBUILD**

​		ONBUILD 是一个特殊的指令，它后面跟的是其它指令，比如 RUN, COPY 等，而这些指令，在当前镜像构建时并不会被执行。只有当以当前镜像为基础镜像，去构建下一级镜像的时候才会被执行。Dockerfile 中的其它指令都是为了定制当前镜像而准备的，唯有 ONBUILD 是为了帮助别人定制镜像而准备的。

**1、先编写一个Dockerfile文件，内容如下：**

```dockerfile
FROM centos:7
LABEL author="发发编程"
ONBUILD RUN mkdir /mydir
```

利用上面的dockerfile文件构建镜像： docker build -t imagea .

利用imagea镜像创建容器： docker run --name test1 -it imagea /bin/bash

我们发现test1容器的根目录下并没有mydir目录。说明ONBUILD指令指定的指令并不会在自己的构建中执行。

**2, 再编写一个新的Dockerfile文件，内容如下**

```dockerfile
FROM imagea
LABEL author="发发编程111"
```

注意，该构建准备使用的基础镜像是上面构造出的镜像imagea

利用上面的dockerfile文件构建镜像： docker build -t imageb

利用imagea镜像创建容器： docker run --name test2 -it imageb /bin/bash

我们发现test2容器的根目录下有mydir目录，说明**ONBUILD**触发器执行了。 

**注意：**什么时候执行的ONBUILD指令指定的指令，**FROM指令执行之后。**

### 6.2.10 **STOPSIGNAL**

​		这个指令和执行docker stop有相关性，执行docker stop时，docker会首先向容器内的当前主程序发送一个SIGTERM信号，用于容器内程序的退出。容器在收到SIGTERM后不会马上退出， 那么stop命令会在等待一段时间（默认是10s）后，再向容器发送SIGKILL信号，将容器杀死，变为退出状态。

​	STOPSIGNAL允许我们覆盖发送到容器的默认信号值是SIGTERM，语法：STOPSIGNAL signals

### 6.2.11 **HEALTHCHECK 健康检查**

HEALTHCHECK 指令是告诉 Docker 应该如何进行判断容器的状态是否正常。

通过HEALTHCHECK 指令，用来判断容器主进程的服务状态是否还正常，从而比较真实的反应容器实际状态。

当在一个镜像指定了 HEALTHCHECK 指令后，用其启动容器，初始状态会为 starting，

在 HEALTHCHECK 指令检查成功后变为 healthy，如果连续一定次数失败，则会变为 unhealthy。

格式：

- HEALTHCHECK  [选项]  CMD <命令>：设置检查容器健康状况的命令
- HEALTHCHECK NONE：如果基础镜像有健康检查指令，使用这行可以屏蔽掉其健康检查指令

HEALTHCHECK 支持下列选项：

- --interval=<间隔>：两次健康检查的间隔，默认为 30 秒；
- --timeout=<时长>：健康检查命令运行超时时间，如果超过这个时间，本次健康检查就被视为失败，默认 30 秒；
- --retries=<次数>：当连续失败指定次数后，则将容器状态视为 unhealthy，默认 3 次。

**注意：**

和 CMD, ENTRYPOINT 一样，HEALTHCHECK 只可以出现一次，如果写了多个，只有最后一个生效。

**命令的返回值决定了该次健康检查的成功与否：**0：成功；1：失败；2：保留，不要使用这个值。

举例：

```dockerfile
FROM nginx:1.19.0
RUN apt-get install -y curl && rm -rf /var/lib/apt/lists/*
HEALTHCHECK --interval=5s --timeout=3s CMD curl -fs http://localhost/ || exit 1
```

这里我们设置了每 5 秒检查一次（这里为了试验所以间隔非常短，实际应该相对较长），如果健康检查命令超过 3 秒没响应就视为失败，并且使用 curl -fs http://localhost/ || exit 1 作为健康检查命令。

**curl 是常用的命令行工具，用来请求 Web 服务器。它的名字就是客户端（client）的 URL 工具的意思。**

**-f参数将不输出错误信息。-s不输出内容。&&:用来执行条件成立后执行的命令 ||:用来执行条件不成立后的执行命令**

```shell
docker build -t myweb:v1 .
docker run -d --name web -p 80:80 myweb:v1
```

## 6.3 操作指令	

![](https://gitee.com/CTLQAQ/picgo/raw/master/38.png)

### 6.3.1 RUN

格式为RUN 或RUN["executable", "param1", "param2"]。 

使用RUN 格式，默认使用的终端是/bin/sh -c;

指定使用其他终端类型可以通过第二种方式实现，

**例如：**

```dockerfile
RUN ["/bin/bash"，"-c","echo hello"] 
```

每条RUN指令将在当前镜像基础上执行指定命令，并提交为新的镜像层。

当命令较长时可以使用**\**来换行。

**例如:**

```dockerfile
RUN  apt-get install -y libsnappy-dev zliblg-dev libbz2-dev \
&& rm -rf /var/cache/apt \
&& rm -rf /var/lib/apt/lists/*
```

### 6.3.2 **CMD 容器启动命令**

简单来理解CMD 相当于启动docker时候后面添加的参数。

```shell
docker run -it centos:7 /bin/bash
docker run -it centos:7 cat /etc/os-release 
```

CMD 指令的格式：

- shell 格式：CMD <命令>

CMD echo "docker so easy"

- exec 格式：CMD ["可执行文件", "参数1", "参数2"...]

CMD ["echo","docker so easy"]

- 参数列表格式：CMD ["参数1", "参数2"...]。在指定了 ENTRYPOINT 指令后，用 CMD 指定具体的参数。

**shell格式和Exec格式区别：**

- **Exec格式**指令会被解析为JSON数组，因此必须用双引号。

- **Exec格式**使用exec执行，不会启动shell环境。

如果使用 **shell 格式**的话，默认将在shell 终端中运行命令，实际的命令会被包装为 sh -c 的参数的形式进行执行。

**比如：**

CMD echo $HOME

**在实际执行中，会将其变更为：**

CMD [ "sh", "-c", "echo $HOME" ]

这就是为什么我们可以使用环境变量的原因，因为这些环境变量会被 shell 进行解析处理。



**进一步理解：**前面我们说了，可以把Docker当成极度精简后的虚拟机，

**但是本质上Docker 不是虚拟机**，运行起来的容器本身就是一个进程。

既然是进程，那么在启动容器的时候，需要指定所运行的程序及参数。

CMD 指令就是用于指定默认的容器主进程的启动命令的。这也是为什么前面讲容器和容器里的应用是同生共死的关系。

### 6.3.3 **COPY 复制文件**

格式：

- COPY <源路径>... <目标路径>
- COPY ["<源路径1>",... "<目标路径>"]

<源路径> （就是本地宿主机上的目录，构建镜像的那个上下文目录）中的文件/目录

<目标路径> 可以是容器内的绝对路径，也可以是**相对于工作目录**的相对路径（工作目录是用 WORKDIR 指令来指定）。目标路径不需要事先创建，**如果目录不存在会在复制文件前先行创建缺失目录**。

### 6.3.4  **ADD 更高级的复制文件**

ADD 指令和 COPY 的格式和性质基本一致。但是在 COPY 基础上增加了一些功能。

​		比如 <源路径> 可以是一个 URL，这种情况下，Docker 引擎会试图去下载这个链接的文件放到 <目标路径> 去。下载后的文件权限自动设置为 600，如果这并不是想要的权限，那么还需要增加额外的一层 RUN 进行权限调整，另外，如果下载的是个压缩包，需要解压缩，也一样还需要额外的一层 RUN 指令进行解压缩。所以不如直接使用 RUN 指令，然后使用 wget 或者 curl 工具下载，处理权限、解压缩、然后清理无用文件更合理。因此，这个功能其实并不实用，而且不推荐使用。

​		**如果 <源路径> 为一个宿主机上的 tar 压缩文件的话，压缩格式为 gzip, bzip2 以及 xz 的情况下，ADD 指令将会自动解压缩这个压缩文件到 <目标路径> 去。**



## 6.4 .dockerignore

```dockerfile
FROM centos:7
COPY .   /abc
CMD ["/bin/bash"]  
```

使用 Dockerfile 构建镜像时最好是将 Dockerfile 放置在一个新建的空目录下。

然后将构建镜像所需要的文件添加到该目录中。

不需要的文件就不要放进去，放进去的文件会被传送给容器。

但是你硬要多放，为了提高构建镜像的效率，你可以在目录下新建一个 .dockerignore 文件来指定要忽略的文件和目录。

![](https://gitee.com/CTLQAQ/picgo/raw/master/37.png)

## 6.5 **多阶段构建**

​		Docker 17.05版本以后，新增了Dockerfile多阶段构建。所谓多阶段构建，实际上是允许一个Dockerfile 中出现多个 FROM 指令。多个 FROM 指令并不是为了生成多根的层关系，最后生成的镜像，仍以最后一条 FROM 为准，之前的 FROM 会被抛弃，那么之前的FROM 又有什么意义呢？

​		每一条 FROM 指令都是一个构建阶段，多条 FROM 就是多阶段构建，虽然最后生成的镜像只能是最后一个阶段的结果，但是，能够将前置阶段中的文件拷贝到后边的阶段中，说白了，把构建环境和运行环境分离，这就是多阶段构建的最大意义。

​		之前没优化的做法，全部构建指令放入一个 Dockerfile在一个 Dockerfile 中，包括项目及其依赖库的编译、测试、打包等流程，这里可能会带来的一些问题：

- Dockerfile 特别长，可维护性降低
- 镜像层次多，镜像体积较大，部署时间变长
- 源代码存在泄露的风险

**以安装nginx为例**

**1、未优化**

```dockerfile
FROM centos:7
EXPOSE 80
VOLUME ["/usr/local/nginx/html"]
ADD  nginx-1.19.1.tar.gz /mnt
RUN yum install -y gcc pcre-devel zlib-devel make
WORKDIR /mnt/nginx-1.19.1
RUN ./configure --prefix=/usr/local/nginx
RUN make
RUN make install
CMD ["/usr/local/nginx/sbin/nginx", "-g", "daemon off;"]

# 创建镜像并查看镜像大小
# docker build -t nginx:v1 .
# docker images
```

**2、 清理中间缓存并尽量减少镜像层数**

```dockerfile
FROM centos:7
EXPOSE 80
VOLUME ["/usr/local/nginx/html"]
ADD  nginx-1.19.1.tar.gz /mnt
WORKDIR /mnt/nginx-1.19.1
RUN yum install -y gcc pcre-devel zlib-devel make && yum clean all && ./configure --prefix=/usr/local/nginx && make && make install
CMD ["/usr/local/nginx/sbin/nginx", "-g", "daemon off;"]
```

**3、使用多阶段构建方法**

我们需要的只不过是编译之后的软件包，那么我们就可以在一容器编译安装以后，将编译安装之后的安装包拷贝到另一个容器中，这样就减小了不需要的开销。并且将压缩包删除。

```dockerfile
FROM centos:7 as build   
ADD  nginx-1.19.1.tar.gz /mnt
WORKDIR /mnt/nginx-1.19.1
RUN  yum install -y gcc pcre-devel zlib-devel make && yum clean all && ./configure --prefix=/usr/local/nginx && make && make install && rm -fr /mnt/nginx-1.19.1

# 创建镜像，将编译好的文件直接拿过来用
FROM centos:7   
COPY --from=build  /usr/local/nginx /usr/local/nginx
EXPOSE 80
VOLUME ["/usr/local/nginx/html"]
CMD ["/usr/local/nginx/sbin/nginx", "-g", "daemon off;"]
```

# 实战

## 构建 nginx

1, 创建nginx.conf

```
worker_processes auto;
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;
 
#pid        logs/nginx.pid;
 
 
events {
    worker_connections  1024;
}
 
 
http {
    include       mime.types;
    default_type  application/octet-stream;
 
    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';
 
    #access_log  logs/access.log  main;
 
    sendfile        on;
    #tcp_nopush     on;
 
    #keepalive_timeout  0;
    keepalive_timeout  65;
 
    #gzip  on;
 
    client_max_body_size   20m;
    server {
        listen       80;
       # server_name  tylerchen;
 
        #charset koi8-r;
 
        #access_log  logs/host.access.log  main;
     location / {
        root   /usr/share/nginx/html;
        index  dist/index.html dist/index.htm;
        try_files $uri $uri/ /index.html;
        }
	location /api/ {
            proxy_pass http://101.132.103.184:9999/;
	    proxy_redirect off;
	    proxy_cookie_path / /api/;
        }
        #error_page  404              /404.html;
 
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
 
        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}
 
        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}
 
        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }
 
 
    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;
 
    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}
}

```

2, 创建Dockerfile

创建数据卷目录app，和Dockerfile存放目录

```shell
mkdir -p /root/dockerfiles/nginx-test/app
cd /root/dockerfiles/nginx-test
vim Dockerfile
#将需要展示的html等文件放到 /root/dockerfiles/nginx-test/app下面
cp -r dist/* app
```

dockerfile文件配置如下：

```dockerfile
ARG VERSION=latest
FROM nginx:${VERSION}
MAINTAINER tylerchen
COPY ./app  /usr/share/nginx/html/
COPY ./nginx.conf /etc/nginx/nginx.conf
RUN echo 'echo init ok!!'
```

```shell
docker build --build-arg VERSION=1.19.1 -t mynginx:1.0.0 . 
```

```shell
docker run --name mynginx -p 80:80 -v /root/dockerfiles/nginx-test/app:/usr/share/nginx/html -d mynginx:1.0.0
```

## 安装mysql5.7

```shell
docker search mysql:5.7
#在运行docker容器时可以加如下参数来保证每次docker服务重启后容器也自动重启
docker run -itd -p 3306:3306 --restart=always -e MYSQL_ROOT_PASSWORD=chentailiang.123 --name mysql mysql:5.7

#已经在运行的：docker update --restart=always <CONTAINER ID>

```

# 7 docker 安全配置

## 7.1  **Docker基本架构**

Docker目前采用了标准的C/S架构，包括客户端、服务端两大核心组件，同时通过镜像仓库来存储镜像。客户端和服务端既可以运行在一个机器上，也可通过socket或者RESTful API来进行通信，如图所示：

![](https://gitee.com/CTLQAQ/picgo/raw/master/47.png)

**服务端**

**服务端主要包括四个组件:**

**1	dockerd:**为客户端提供RESTful API服务，响应来自客户端的请求，通过服务端的/run/containerd/containerd.sock监听客服端请求。

**2	docker-proxy:**当需要进行容器端口映射时，docker-proxy完成网络映射配置;  可以查看进程信息，看到。

**3	containerd:**containerd是容器技术标准化之后的产物，为了能够兼容OCI标准，   将容器运行时及其管理功能从Docker Daemon剥离。理论上，即使不运行dockerd，   也能够直接通过containerd来管理容器，比如对容器的启动、停止、删除等。

**3	containerd-shim:**是containerd的子进程，启动一个容器就有对应的一个containerd-shim产生，每个containerd-shim为runC容器提供支持，同时作为容器内进程的0号根进程。

**镜像仓库**

镜像是使用容器的基础，Docker 使用镜像仓库(Registry)在大规模场景下存储和分发Docker镜像。

## 7.2 **Linux操作系统的命名空间(namespace)**

命名空间( namespace)是Linux内核的一个强大特性，在Linux内核中提供了多个namespace，为容器虚拟化的实现带来极大便利。**利用这一特性，每个容器都可以拥有自己单独的命名空间，运行在其中的应用都像是在独立的操作系统环境中一样。命名空间机制保证了容器之间彼此互不影响。**

**1）进程命名空间**

Linux通过**进程命名空间管理进程号**，对于同一进程， 在不同的命名空间中，看到的进程号不相同。每个进程命名空间有一套自己的进程号管理方法。进程命名空间是一个父子关系的结构，子空间中的进程对于父空间是可见的。

**2） IPC命名空间**

容器中的进程交互还是采用了Linux常见的进程间交互方法( Interprocess Communication，IPC)，包括信号量、消息队列和共享内存....等方式。同一个IPC命名空间内的进程可以彼此可见，允许进行交互，不同空间的进程则无法交互。

**3）网络命名空间**

有了进程命名空间后，不同命名空间中的进程号可以相互隔离，但是网络端口还是共享本地系统的端口。

Docker采用**虚拟网络设备**(Virtual Network Device，VND)的方式，将不同命名空间的网络设备连接到一起。默认情况下，Docker在宿主机上创建多个虚机网桥(如默认的网桥docker0)，容器中的虚拟网卡通过网桥进行连接，如图所示：

**4）挂载命名空间**

挂载(Mount，MNT)命名空间可以将一个进程的根文件系统限制到一个特定的目录下。每个命名空间中的进程所看到的文件目录彼此是隔离的。例如，不同命名空间中的进程，**都认为自己独占了一个完整的根文件系统( rotfs)**，但实际上，不同命名空间中的文件彼此隔离，不会造成相互影响，同时也无法影响宿主机文件系统中的其他路径。

**5）UTS命名空间**

UTS ( UNIX Time-sharing System)命名空间允许**每个容器拥有独立的主机名和域名**，从而可以虚拟出一个有独立主机名和网络空间的环境，就跟网络上一台独立的主机一样。如果没有手动指定主机名称，Docker 容器的主机名就是返回的容器ID的前6字节前

**6）用户命名空间**

每个容器可以有不同的用户和组id，也就是说，可以在容器内使用特定的内部用户执行程序，而非本地系统上存在的用户。每个容器内部都可以有最高权限的root帐号，但跟宿主主机不在一个命名空间。通过使用隔离的用户命名空间，可以提高安全性，避免容器内的进程获取到额外的权限;

## 7.3  **控制组**

控制组(CGroups)是Linux内核的另一个特性，主要用来对共享资源进行隔离、限制、审计等。只有将分配到容器的资源进行控制，才能避免多个容器同时运行时对宿主机系统的资源竞争。控制组技术最早是由Google 的程序员在2006年提出的，Linux 内核自2.6.24开始原生支持，可以提供对容器的内存、CPU、磁盘IO等资源进行限制和计费管理。

**具体来看，控制组提供如下功能:**

**资源限制(resource limiting):**可将组设置一定的内存限制。比如:内存子系统可以为进程组设定一个内存使用上限，一旦进程组使用的内存达到限额再申请内存，就会出发Out of Memory警告。

**优先级(prioritization):**通过优先级让一些组优先得到更多的CPU等资源。

**资源审计( accounting):**用来统计系统实际上把多少资源用到适合的目的上，可以使用cpuacct子系统记录某个进程组使用的CPU时间。

**隔离(isolation):**为组隔离命名空间，这样使得一个组不会看到另一个组的进程、网络连接和文件系统。

**控制(control):** 执行挂起、恢复和重启动等操作。

**容器每次启动后，都会调用setCapabilities方法完成对各个命名空间的配置，安装Docker后/sys/fs/cgroup/memory/docker目录下就是对Docker组应用的各种限制，包括全局限制，在子目录里的是对单个容器的单独限制，运行的容器就会有对应的子目录。**

用户可以通过修改这些文件值来控制组，从而限制Docker应用资源。**具体见7.5的核心配置**

## 7.4	**联合文件系统**

联合文件系统(UnionFS) 是一种轻量级的高性能分层文件系统，它支持将文件系统中的修改信息作为一次提交，并层层叠加，同时可以将不同物理位置的目录挂载到同一个目录下，这个目录内部就是一个虚拟文件系统。

**1）Docker存储原理**

最新的Docker版本中默认使用的是OverlayFS，于2014年正式合入Linux-3.18主线内核的文件系统，overlay和overlay2是docker提供的存储驱动，现在Docker官方推荐使用overlay2。Docker镜像自身就是由多个文件层组成，每一层有基于内容的唯一的编号(层ID)。

可以通过docker  history查看一个镜像由哪些层组成。例如查看ubuntu:18.04镜像由5层组成，每层执行了不同的命令，如下所示:

![](https://gitee.com/CTLQAQ/picgo/raw/master/48.png)

对于Docker镜像来说，这些层的内容都是不可修改的、只读的。而当Docker利用镜像启动一个容器时，**将在镜像文件系统的最顶端再挂载一个新的可读写的层给容器。**容器中的内容更新将会发生在可读写层。当所操作对象位于较深的某层时，需要先复制到最上层的可读写层。当数据对象较大时，往往意味着较差的IO性能。因此，对于IO敏感型应用，一般推荐将容器修改的数据通过volume（因为volume是存在宿主机上的，不会保存到镜像文件的某层）方式挂载，而不是直接修改镜像内数据。

**2）Docker存储结构**

所有的镜像和容器都存储都在Docker指定的存储目录下，默认路径是**/var/lib/docker**。镜像和容器的存储主要涉及**image和ovlerlay2**两个目录。

我们只需要关心/var/lib/docker/image和/var/lib/docker/overlay2，**可以先到/var/lib/docker/image，在进入到overlay2里**：这里的关键地方是imagedb和layerdb目录，专门用来存储元数据的地方，在docker中，image是由多个layer组合而成的，一个layer可以被多个image引用。

**那如何才能确认image包含了哪些layer呢？答案就在imagedb这个目录中去找。**

比如上面启动的nginx容器，我们可以先找到这个容器对应的镜像：

![](https://gitee.com/CTLQAQ/picgo/raw/master/49.png)

**我们打印**/var/lib/docker/image/overlay2/imagedb/content/sha256这个目录：

![](https://gitee.com/CTLQAQ/picgo/raw/master/50.png)

用cat查看， 看到很长的json，将其格式化：https://www.sojson.com/ (json格式化工具)

![](https://gitee.com/CTLQAQ/picgo/raw/master/51.png)

可以看到rootfs的**diff_ids**是一个包含了多个元素的数组，其实这些个元素正是组成nginx镜像的多个layerID，从上往下看，就是底层到顶层，**也就是说第一个layerID是image的最底层。**

然后进入：/var/lib/docker/image/overlay2/layerdb/sha256 目录

在这里，我们仅仅发现**d0f104dc0a1..**这个最底层的layer，那么剩余layer为什么会没有呢？那是因为docker使用了chainID的方式去保存这些layer信息，简单来说就是chainID(n)=SHA256(chain(n-1) diffID(n))，

- 如果该镜像层是最底层，该层的 diffID 便是 chainID。
- 该镜像层的 chainID 计算公式为 chainID(n)=SHA256(chainID(n-1) diffID(n))。

下面我们来演示下计算过程

第一层：由于是最底层，chainID1=diffID

第二层：套用上面的公式

chainID(2)=SHA256(chain(1) diffID(2)

可以用在线SHA256工具： http://www.jsons.cn/sha/

**依次类推可拿到镜像对应的所有层的chainID，**

**chainID对应的目录里边的cache-id关联这真实的rootfs的位置：**

![](https://gitee.com/CTLQAQ/picgo/raw/master/52.png)

**这个id就是对应/var/lib/docker/overlay2/xxxx子目录，**

这些子目录下的diff目录，统一合并挂载的虚拟目录，就是整个镜像的rootfs。

**3）合并挂载逻辑原理**

OverlayFS将单个Linux主机上的**两个目录合并成一个目录**。这些目录被称为层，统一过程被称为**联合挂载**。OverlayFS底层目录称为lowerdir， 高层目录称为upperdir。合并统一视图称为merged。当需要修改一个文件时，将文件从只读的Lower复制到可写的Upper进行修改，结果也保存在Upper层。

**在Docker中，底下的只读层就是image，可写层就是Container。**

当启动一个容器，在/var/lib/docker/overlay2目录下生成一层容器层，核心目录包括diff，work，merged

**diff记录每一文件层的数据，merged合并的目录，work记录合并的工作记录。**

### **实战overlayFS的存储逻辑**

```shell
mkdir -p lower upper merger worker
echo '1.txt lower' > ./lower/1.txt
echo '3.txt lower' > ./lower/3.txt
echo '1.txt upper' > ./upper/1.txt
echo '2.txt upper' > ./upper/2.txt
#加载overlay支持，modprobe命令用于自动处理可载入模块:
modprobe overlay 
mount -n -t overlay overlayfs:/overlay -o lowerdir=lower/,upperdir=upper/,workdir=worker/ merger
-n：一般而言，mount 在挂上后会在 /etc/mtab 中写入一些信息。没的东西写用这个选项取消这个动作。
-t：指定文件系统的型态，通常不必指定。这里要指定overlay
-o：指定操作参数

```

创建后的目录结构

![](https://gitee.com/CTLQAQ/picgo/raw/master/53.png)

merger 里面的1.txt使用的是upper层的文件，也就是说高层会有更高的优先级。

在merger里面创建 merger.txt  :    echo   “test” > ./merger/merger.txt

![](https://gitee.com/CTLQAQ/picgo/raw/master/54.png)

会发现，新创建的文件是放在upper层的。

## 7.5  安全配置

**Docker容器的安全性,很大程度上依赖于Linux系统自身。**Docker通过**CGroups**实现宿主机中不同容器的资源限制与审计，包括对CPU、内存、I/O等物理资源进行均衡化配置，防止单个容器耗尽所有资源造成其他容器或宿主机的拒绝服务，保证所有容器的正常运行。容器资源的控制主要是通过**cgroup**来进行的，给用户暴露出来的操作接口是**文件系统**，以文件和目录的方式组织在操作系统的 **/sys/fs/cgroup** 路径下。

### **1）cpu限额**

我们一般不直接修改cpu目录中的参数，在cpu的目录下新建一个目录，进入目录后可以看到自动给我们生 成了与cpu父级进程中相同的文件，我们可以通过修改这个目录中的文件使设定生效。

（1）dd if=/dev/zero of=/dev/null &			##吃掉cpu的所有资源，测试吞吐量 注意：即使是双核，一个被占满的情况下另一个也不会帮忙处理，但是在三核的情况下就会发生资源的争抢

![](https://gitee.com/CTLQAQ/picgo/raw/master/55.png)

会发现，dd命令几乎占用了所有的cpu资源

（2）在 **/sys/fs/cgroup/cpu**目录下创建test文件夹，里面会生成一系列文件，

```shell
echo 20000 > cpu.cfs_quota_us  ##限制cpu的资源上限 即最多占用cpu20%的资源
echo 48425 > tasks					##把进程的pid写入文件
```

写完之后的效果

![](https://gitee.com/CTLQAQ/picgo/raw/master/56.png)

**用在容器上：**

--cpu-shares

cpu的限额需要再容器启动时直接添加参数设定，我们可以通过--help来查看相关参数

docker run -it --name vm1 **--cpu-period=100000 --cpu-quota 20000** ubuntu:18.04

\##表示在period设定的时间内，以微秒为单位，设定cpu的占用上限为20% 

dd if=/dev/zero of=/dev/null &	

##在进入容器后执行此命令占用容器的所有资源 ##ctrl+p+q将容器打入后台，top查看cpu的占用情况

### **2）内存占用控制**

**首先安装一个cgroup软件，libcgroup-tools-0.41-11.el7.x86_64，可以直接通过yum安装**

yum  install -y libcgroup-tools-0.41-11.el7.x86_64

```shell
cd /sys/fs/cgroup/memory/			##进入cgroup内存控制
mkdir test						##和cpu一样建立一个子进程来控制容器服务
echo 314572800 > memory.limit_in_bytes    ##限制300M
cd /dev/shm   ## tempfs中的一种，这个文件系统不在磁盘，在内存上 
# 这样执行不可以 配置的限额不会起作用。dd if=/dev/zero of=file bs=1M count=300   ## 拷贝300M的数据到内存上
cgexec -g memory:test dd if=/dev/zero of=file bs=1M count=400		##创建文件时使我们test里的内存限制生效
free -m   ## 查看空闲内存，按道理来说，我们已经限制了内存使用上限300M
#可用空间没有减少，说明配置生效了。
```

![](https://gitee.com/CTLQAQ/picgo/raw/master/58.png)

**限制容器内存的占用：**

```shell
docker run -it --memory 200M --memory-swap=200M --name vm1 ubuntu:18.04
##创建容器，限制内存为200M，进入后crtl+p+q 
##打入后台运行后查看docker里的内存文件
```

### **3）block IO限制**

Block IO 指的是磁盘的读写，docker 可通过设置权重、限制 bps 和 iops 的方式控制容器读写磁盘的带宽。

**block IO 权重**

默认情况下，所有容器能平等地读写磁盘，可以通过设置 **--blkio-weight** 参数来改变容器 block IO 的优先级。

--blkio-weight ，设置的是相对权重值，默认为 500。

在下面的例子中，container_A 读写磁盘的带宽是 container_B 的两倍。

docker run -it --name container_A --blkio-weight 600 ubuntu:18.04

docker run -it --name container_B --blkio-weight 300 ubuntu:18.04

**限制 bps 和 iops**

bps 是 byte per second，每秒读写的数据量。

iops 是 io per second，每秒 IO 的次数。

**可通过以下参数控制容器的 bps 和 iops：**

--device-read-bps，限制读某个设备的 bps。

--device-write-bps，限制写某个设备的 bps。

--device-read-iops，限制读某个设备的 iops。

--device-write-iops，限制写某个设备的 iops。

**--device-write-bps限制写设备的bps，限制读写速度** 目前的block IO限制只对direct IO有效。(不使用文件缓存) 在创建容器时加上这个参数，oflag=direct 不经过缓存，直接写磁盘 **docker run -it --name vm2** **--device-write-bps /dev/sda:10MB** **ubuntu:18.04**

![](https://gitee.com/CTLQAQ/picgo/raw/master/59.png)

### **4）限制容器的磁盘使用空间**

为每个容器创建单独用户，限制每个用户的磁盘使用量；

### **5）设置特权级运行的容器:--privileged=true**

有的时候我们需要容器具备更多的权限,比如操作内核模块,控制swap交换分区,挂载USB磁盘,修改MAC地址等。

**用一个false的vm1容器和一个true的vm2容器来做对比:**

**docker run -it --name vm1 ubuntu:18.04** **docker run -it --name vm2 --privileged=true ubuntu:18.04** **docker inspect vm1 | grep Pri**		**##查看privileged的状态**

ip命令的安装：apt update && apt install -y iproute2

**ip link set down eth0  # 关闭网卡**

**--privileged=true 的权限非常大,接近于宿主机的权限,为了防止用户的滥用,需要增加限制,只提供给容器必须的权限。**

此时Docker 提供了权限白名单的机制,使用**--cap-add**添加必要的权限。

**docker run -it --cap-add=NET_ADMIN --name vm1 ubuntu:18.04**

capabilities手册地址： http://man7.org/linux/man-pages/man7/capabilities.7.html

# 8 docker 网络功能

## 8.1 **Docker网络启动过程**

​		**默认配置的情况下**，Docker服务启动时会首先在主机上自动创建一个docker0虚拟网桥，实际上是一个Linux网桥。网桥可以理解为一个软件交换机，负责挂载其上的接口之间进行包转发。

​		同时，Docker随机分配一个本地未占用的私有网段中的一个地址给docker0接口。比如典型的172.17.0.0/16 网段此后启动的容器内的网络接口也会自动分配一个该网段的址。

​		当创建一个Docker容器的时候，同时会创建了一对veth pair互联接口。当向任一个接口发送包时，另外一个接口自动收到相同的包。互联接口的一端位于容器内，即eth0，另一端在本地并被挂载到docker0网桥，名称以veth开头(例如vethAQI2QT)。**通过这种方式，主机可以与容器通信，容器之间也可以相互通信。**如此一来，Docker 就创建了在主机和所有容器之间一个虚拟共享网络，如图：

![](https://gitee.com/CTLQAQ/picgo/raw/master/57.png)

## 8.2.网络相关参数

```json
{
    “bridge”:"",  ————将容器附加到网桥
    “default-gateway”:"",  ——————容器默认网关IPv4地址
    “default-gateway-v6”:"",  ——————容器默认网关IPv6地址
    “dns”: [“192.168.1.1”], ——————设定容器DNS的地址，在容器的 /etc/resolv.conf文件中可查看。
    “dns-opts”: [],   ——————容器中 /etc/resolv.conf 文件外的其他设置
    “dns-search”: [],  ————设定容器的搜索域，当设定搜索域为 .example.com 时，
    在搜索一个名为 host 的 主机时，DNS不仅搜索host，还会搜索host.example.com 。 
    注意：如果不设置， Docker 会默认用主机上的 /etc/resolv.conf 来配置容器。
    “icc”: false,  ——————禁止容器间通信（默认为true）
    “ip”:“0.0.0.0”,  ————————绑定容器端口时的默认IP（默认0.0.0.0）
    “iptables”: false,  ———————禁止iptables规则添加（默认为true）
    “ipv6”: false,  ——————禁止IPv6网络
    “ip-forward”: false,  —————默认true, 启用 net.ipv4.ip_forward ,进入容器后使用 sysctl -a | grepnet.ipv4.ip_forward 查看
    “mtu”: 0,  ——————设置容器网络MTU
    “bip”:“192.168.88.0/22”,   ——————————指定网桥IP，注意这个参数仅仅是配置docker0的，对其他自定义的网桥无效。
}
```

**下面的命令选项既可以在启动服务时指定，也可以Docker容器启动(使用docker run命令)时候指定。**配置在---/etc/docker/daemon.json值**在Docker服务启动的时候会成为默认值,后续执行该命令时可以覆盖设置的默认值:

**--dns=IP_ADDRESS:**使用指定的DNS服务器;

**--dns-option="":**指定DNS选项;

**--dns-search=DOMAIN:**指定DNS搜索域。

设定容器的搜索域，当设定搜索域为.example.com时，在搜索一个名为host的主机时，DNS不仅搜索host,还会搜索host.example.com。



**还有些选项只能在docker run 命令执行时使用，因为它针对容器的配置:**

**-h HOSTNAME or --hostname=HOSTNAME:** 配置容器主机名;

**--ip="":**指定容器内接口的IP地址;

**--link=CONTAINER_NAME:ALIAS:**添加到另一个容器的连接;

**-p SPEC or --publish=SPEC:**映射容器端口到宿主主机;

**-P or --publish-all=truelfalse:**映射容器所有端口到宿主主机。

**--net=bridge | none | container:NAME_or_ID | host | user_ defined_network:**配置容器的网络模式;



## 8.3  容器启动后，支持的五种网络模式:

### 1  --net=bridge

**--net=bridge:**默认配置。为容器创建独立的网络命名空间，分配网卡、IP 地址等网络配置，并通过veth接口对将容器挂载到一个虚拟网桥(默认为docker0)上;

![](https://gitee.com/CTLQAQ/picgo/raw/master/60.png)

从上面的网络模型可以看出，容器从原理上是可以与宿主机乃至外界的其他机器通信的。同一宿主机，容器之间都是连接掉docker0这个网桥上，它可以作为虚拟交换机使容器可以相互通信。然而，由于宿主机的IP地址与容器veth pair的 IP地址均不在同一个网段，故仅仅依靠veth pair和namespace的技术，**还不足以使宿主机以外的网络主动发现容器的存在。**为了使外界可以访问容器中的进程，docker采用了端口绑定的方式，也就是通过iptables的NAT，将宿主机上的端口流量转发到容器内的端口上。如：

```
docker run --name db -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 -d mysql:5
```

在宿主机上，可以通过iptables -t nat -L -n，查到一条DNAT规则：

DNAT tcp -- 0.0.0.0/0 0.0.0.0/0 tcp dpt:3306 to:172.17.0.5:3306

上面的172.17.0.5即为bridge模式下，创建的容器IP。

很明显，bridge模式的容器与外界通信时，必定会占用宿主机上的端口，从而与宿主机竞争端口资源，对宿主机端口的管理会是一个比较大的问题。同时，由于容器与外界通信是基于三层上iptables NAT，性能和效率上的损耗是可以预见的。

### 2 **--net=none:**

**--net=none:**为容器创建独立的网络命名空间，但不进行网络配置，即容器内没有创建网卡、IP地址等;也就是说，none模式为容器做了最少的网络设置，但是俗话说得好“少即是多”，在没有网络配置的情况下，通过第三方工具或者手工的方式，任意定制容器的网络，提供了最高的灵活性。

**自行配置网络，让容器达到跟平常一样具有访问网络的权限。通过这个过程，可以了解 Docker 配置网络的细节。**

```shell
docker run -i -t --rm --net=none centos:7 /bin/bash
#在本地主机查找容器的进程 id，并为它创建网络命名空间。
docker inspect -f '{{.State.Pid}}'   容器id
pid=11350
mkdir -p /var/run/netns
ln -s /proc/$pid/ns/net /var/run/netns/$pid
#检查桥接网卡的 IP 和子网掩码信息
ip addr show docker0
	inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
#创建一对 “veth pair” 接口 A 和 B，绑定 A 到网桥  docker0  ，并启用它
ip link add A type veth peer name B
brctl addif docker0 A     (没有bgctl需要安装bridge-utils工具包)
ip link set A up
#将B放到容器的网络命名空间，命名为 eth0，启动它并配置一个可用 IP（桥接网段）和默认网关
ip link set B netns $pid
ip netns exec $pid ip link set dev B name eth0
ip netns exec $pid ip link set eth0 up
ip netns exec $pid ip addr add 172.17.0.99/16 dev eth0
ip netns exec $pid ip route add default via 172.17.0.1
#就是 Docker 配置网络的具体过程。
#当容器结束后，Docker 会清空容器，容器内的 eth0 会随网络命名空间一起被清除，A 接口也被自动从  docker0  卸载。
```

### 3 --net=container:NAME_or_ID

​		新创建的容器共享指定的已存在容器的网络命名空间，两个容器内的网络配置共享，但其他资源(如进程空间、文件系统等)还是相互隔离的;

![](https://gitee.com/CTLQAQ/picgo/raw/master/61.png)

​		在这种模式下的容器可以通过localhost来访问同一网络命名空间下的其他容器，传输效率较高。而且这种模式还节约了一定数量的网络资源，但它并没有改变容器与外界通信的方式。在一些特殊的场景中非常有用，例如，kubernetes的pod，kubernetes为pod创建一个基础设施容器，同一pod下的其他容器都共享这个基础设施容器的网络命名空间，相互之间以localhost访问，构成一个统一的整体。

### 4 --net=host

**--net=host:**不为容器创建独立的网络命名空间，

​	容器内看到的网络配置(网卡信息、路由表、Iptables 规则等)均与主机上的保持一致。 注意其他资源还是与主机隔离的。

![](https://gitee.com/CTLQAQ/picgo/raw/master/67.png)

采用host模式的容器，不再需要通过linuxbridge等方式转发或数据包的拆封，性能上有很大优势。

当然，这种模式有优势，也就有劣势，主要包括以下几个方面：

1）最明显的就是容器不再拥有隔离、独立的网络栈。容器会与宿主机竞争网络栈的使用，并且容器的崩溃就可能导致宿主机崩溃，在生产环境中，这种问题可能是不被允许的。

2）容器内部将不再拥有所有的端口资源，一些端口已经被宿主机的其他服务占用掉了。

### 5 **--net=user_defined_network**

**--net=user_defined_network:** 开发者可以使用任何docker支持的第三方网络driver来定制容器的网络。

自行用network相关命令创建一个网络，同一个网络内的容器彼此可见：

docker network create bri1

则docker会自动生成如下的iptables规则，保证不同网络上的容器无法互相通信。

\# -A DOCKER-ISOLATION -i br-8dba6df70456 -o docker0 -j DROP

\# -A DOCKER-ISOLATION -i docker0 -o br-8dba6df70456 -j DROP

## 8.4 容器的访问控制

​		容器的访问控制主要通过Linux上的iptables防火墙软件来进行管理和实现。iptables是Linux系统流行的防火墙软件，在大部分发行版中都自带。

**1）容器访问外部网络**

​		从前面的描述中，我们知道容器默认指定了网关为docker0网桥上的内部接口。docker0内部接口同时也是宿主机的一个本地接口。因此，容器默认情况下可以访问到宿主机本地网络。如果容器要想通过宿主机访问到外部网络，则需要宿主机进行辅助转发。

在宿主机Linux系统中，检查转发是否打开，代码如下:

```shell
$ sysctl net.ipv4.ip_forward
net. ipv4. ip_ forward = 1
#如果为0，说明没有开启转发，则需要手动打开:
$ sysctl -w net.ipv4.ip_forward=1
#Docker服务启动时会默认开启--ip-forward=true，自动配置宿主机系统的转发规则。
```

**2）容器之间访问**

容器之间相互访问需要两方面的支持:

网络拓扑是否已经连通。默认情况下，所有容器都会连接到docker0网桥上，这意味着默认情况下拓扑是互通的;

本地系统的防火墙软件iptables 是否允许访问通过。这取决于防火墙的默认规则是允许(大部分情况)还是禁止。

下面分两种情况介绍容器间的访问。

**(1)访问所有端口**

当启动Docker服务时候，默认会添加一条“允许”转发策略到iptables的FORWARD链上。

通过配置--icc=true l false (默认值为true)参数可以控制默认的策略。

为了安全考虑，可以在Docker配置文件中配置icc:"false"来默认禁止容器之间的相互访问。

**(2)访问指定端口**

在通过-icc=false禁止容器间相互访问后，修改了全局配置文件后，

仍可以通过**--link=CONTAINER_NAME:ALIAS**选项来允许访问指定容器的开放端口。

## 8.5 配置容器网桥

**/etc/docker/daemon.json**这些值都可以在服务启动的时候进行配置:

**"bridge":"",  ————将容器附加到网桥**

**"bip":"192.168.88.1/24",   ———指定网桥的IP地址，注意这个参数仅仅是配置docker0的，对其他自定义的网桥无效。**

**"mtu": 1500,  ——————设置容器网络MTU**

由于目前Docker网桥是Linux网桥，用户可以使用brctl show来查看网桥和端口连接信息:

**$ brctl  show**

每次创建一个新容器的时候，Docker从可用的地址段中选择一个空闲的IP地址分配给容器的eth0端口，并且使用本地主机上docker0接口的IP作为容器的默认网关.

**自定义网桥**

```
除了默认的docker0网桥， 用户也可以指定其他网桥来连接各个容器。
如果服务已经运行，就需要先停止服务，并删除旧的网桥:
$ systemctl stop docker 
$ ip link set dev docker0 down
$ brctl delbr docker0
然后创建一个网桥bridge0:
$  brctl addbr bridge0
$  ip addr add 192.168.5.1/24 dev bridge0
$ ip link set dev bridge0 up
查看确认网桥创建并启动:
$ ip addr show bridge0
配置Docker服务，默认桥接到创建的网桥上:
/etc/docker/daemon.json里的："bridge":"bridge0",  ————将容器附加到网桥
$ systemctl start docker，启动Docker服务。
新建一个容器，可以看到它已经桥接到了了bridge0上。
可以继续用brctl show，另外，在容器中可以使用ip addr和ip route命令来查看IP地址配置和路由信息。

8.创建点对点的链接
在默认情况下，Docker 会将所有容器连接到由docker0提供的虚拟网络中。
用户有时候需要两个容器之间可以直连通信，而不用通过主机网桥进行桥接。
解决办法很简单:创建一对peer接口，分别放到两个容器中，配置成点到点链路类型即可。
下面将通过手动操作完成Docker配置容器网络的过程。
首先启动两个容器:
$ docker run --net=none --name box1  -it --rm  centos:7  bash
$ docker run --net=none --name box2  -it --rm  centos:7  bash
找到进程号，然后创建网络命名空间的跟踪文件:
$ docker  inspect -f   '{{.State.Pid}}'  容器ID
1：11347
2：11433
$ mkdir -p /var/run/netns
$ ln -s /proc/11433/ns/net   /var/run/netns/11433
创建一对peer接口：
$ ip link  add A  type  veth peer  name  B
添加IP地址和路由信息:
$ ip link set A netns  上面查询出来的pid号1
$ ip netns exec 上面查询出来的pid号1  ip link set A up
$ ip netns exec  上面查询出来的pid号1  ip addr add 10.1.1.1/24 dev A
$ ip netns exec 上面查询出来的pid号1  ip route add 10.1.1.2/24 dev A
$ ip link set B netns 上面查询出来的pid号2
$ ip netns exec 上面查询出来的pid号2  ip link set B up
$ ip netns exec 上面查询出来的pid号2  ip addr add 10.1.1.2/24 dev B
$ ip netns exec 上面查询出来的pid号2  ip route add 10.1.1.1/24 dev B
现在这两个容器就可以相互ping通，并成功建立连接。
点到点链路不需要子网和子网掩码。
此外，也可以不指定--net=none来创建点到点链路。
这样容器还可以通过原先的网络来通信。
利用类似的办法，可以创建一个只跟主机通信的容器。
但是一般情况下，更推荐使用--icc=false命令来关闭容器之间的通信。
```

## 8.6 libnetwork插件化网络功能

### **1、容器网络模型**

Docker网络架构是建立在一个接口集合之上，该接口集合被称为容器网络模型(Container Networking Mode,CNM)。设计CNM的目的是提供程序的可移植性。 网络模型结构图如下：

![](https://gitee.com/CTLQAQ/picgo/raw/master/68.png)

**CNM包含下面三部分：**

**Sandbox** — sandbox包含container的网络栈配置，比如container的接口管理、路由表和DNS设置，就是Linux的网络命名空间。

**Endpoint** — Endpoint用于把Sandbox加入Network，表示网络上可以挂载容器的接口，会分配IP地址;

**Network** — Network可以通过Linux bridge、VLAN等方式实现，可以连通多个接入点的一个子网。



**CNM提供了两种可插拔的、开放的接口：**

**1, 网络驱动 — docker 网络驱动使得network正常运行。**

当前docker engine或者cluster可以使用多个网络驱动，但是每个具体Docker network只能通过一个网络驱动实例化。

有多种驱动可以选择，比如：

Host---使用host驱动, container使用主机的网络栈。

当容器网络堆栈不应与Docker主机隔离但又希望隔离容器的其他方面

Bridge---Bridge驱动在主机上创建一个可以被docker管理的Linux 网桥。

当您需要多个容器在同一个Docker宿主机上进行通信时，使用自定义的桥接网络模式（bridge）是最佳选择。 

Null: 不提供网络服务，容器启动后无网络连接;

Overlay---Overlay驱动创建一个overlay 网络，它支持多主机网络。

当您需要在不同Docker守护进程上运行的容器进行通信时，覆盖网络模式（overlay）是最佳选择。

macvlan：每个container都有一个虚拟的MAC地址。需要容器看起来像网络上的物理主机时

Remote: 扩展类型，预留给其他外部实现的方案。以上模式都不能满足您的需求

**2, IP地址管理驱动 — docker拥有一个本地IP地址管理驱动。**

如果没有指定network和endpoint，那么它会为之提供默认子网或地址。

也可以通过network、container和service创建命令手动分配IP地址。

也存在Remote IPAM drivers，并且提供对当前IPAM工具的集成

### **2、Docker网络命令**

在libnetwork支持下，Docker 网络相关操作都作为network的子命令出现。

围绕着CNM生命周期的管理，主要包括以下命令:

- create:创建一个网络;
- connect:将容器接入到网络;
- disconnect:把容器从网络上断开;
- inspect:查看网络的详细信息。
- ls:列出所有的网络;
- prune:清理无用的网络资源;
- rm:删除一个网络。

#### **1）列出网络**

ls命令用于列出网络。命令格式为：docker network ls [OPTIONS] 

其中支持的选项主要有:

-f，--filter="": 指定输出过滤器，如driver-bridge;

--format="": 给定一个golang模板字符串，对输出结果进行格式化;

--no-trunc[=false]: 不截断地输出内容;

-q， --quiet[=false]:安静模式，只打印网络的ID。

实际上，在不执行额外网络命令的情况下，用户执行docker network ls 命令，一般情况下可以看到已创建的三个网络:

$ docker network ls

分别为三种驱动的网络: null、 host 和bridge。

#### **2）创建网络**

create命令用于创建一个新的容器网络。

Docker 服务在启动后，会默认创建一个bridge类型的网桥docker0，不同网络之间默认相互隔离。

创建网络命令格式为docker network create  [OPTIONS]  NETWORK。

支持参数包括:

-d，--driver="bridge":网络驱动类型，如bridge或overlay;

--gateway= []:网关地址;

--subnet=[]:指定网络地址段，CIDR 格式，如172.12.0.0/16

**例子：**

docker network create --subnet=192.168.10.0/24  net1     //用brctl工具命令创建的网桥没啥子区别

or

docker network create --driver="bridge" --subnet=192.168.10.0/24  net1

默认的bridge网络与自建bridge网络有区别，一般情况下生产环境中不建议使用默认bridge网络：

- 默认的bridge网络端口不会自行发布，必须使用-p参数才能为外界访问，而自建的bridge网络时，container的端口可直接被相同网络下的其他container访问。
- 默认的bridge网络container之间的如果需要通过名字访问，需要--link参数，而自建的bridge网络，container之间可以通过名字互访，不需要link。
- 可以动态将运行中的容器与自定义网络连接或断开，不需要重启。如果使用默认网桥，则需要停止容器，再通过不同选项重新创建容器。
- 可以针对自定义网桥进行个性化配置，而不会影响其它使用该网桥的容器。
- https://docs.docker.com/network/bridge/#differences-between-user-defined-bridges-and-the-default-bridge

#### **3）接入网络**

connect命令将一个容器连接到一个已存在的网络上。

连接到网络上的容器可以跟同一网络中其他容器互通，同一个容器可以同时接入多个网络。

也可以在执行docker run命令时候通过--net参数指定容器启动后自动接入的网络。

接入网络命令格式为：docker network connect [OPTIONS] NETWORK CONTAINER。

支持参数包括:

 --alias=[]:为容器添加一个别名，此别名仅在所添加网络上可见;

 --ip="":指定IP地址，需要注意不能跟已接人的容器地址冲突;

 --ip6="":指定IPv6地址;

 --link value:添加链接到另外一个容器;

 --link-local-ip=[]: 为容器添加一个链接地址。

```shell
1. 将运行中的容器ctn1 连接 到网络net1中
$ docker network connect net1 ctn1

2. 在启动容器ctn2时，连接到网络net1上，
$ docker run -itd --network=net1 --name=ctn2 centos:7

3. 给容器分配指定ip(192.168.10.10)
$ docker network connect --ip 192.168.10.10 net1 ctn3

4. --alias：为容器在网络范围内添加一个别名。
在网络范围内添加别名是什么意思呢？下面我们通过一个栗子来看看它的用法：
# 运行container6容器，并给ctn4添加一个app的别名
docker run --net=net1 -itd --name=ctn4  --net-alias app centos:7
docker network connect --alias app net1 ctn4   // 启动容器6的时候，容器6不在net1中，连接的时候，设置别名

# 运行container7容器，同样给ctn5添加一个scoped-app的别名
docker run  -itd --name=ctn5  centos:7
docker network connect --alias app net1 ctn5

# 这时再从另外一个容器中查看效果
# 在ctn2中
docker exec -it ctn2 sh
ping app #  给我响应的是ctn4
exit

docker stop ctn4
docker exec -it ctn2  sh
ping app # 访问到了ctn5
可以看到，对多个容器添加网络范围内别名时，一定程度上可以实现网络HA功能。
```

#### **4）断开网络**

disconnect命令将一个连接到网络上的容器从网络上断开连接。

命令格式为：docker network disconnect  [OPTIONS]  NETWORK  CONTAINER。

支持参数包括-f，--force: 强制把容器从网络上移除。

```shell
1. 将运行中的容器ctn1断连 到网络net1中
$ docker network disconnect net1 ctn1
```

#### **5）查看网络信息**

inspect命令用于查看一个网络的具体信息(JSON 格式)，包括接人的容器、网络配置信息等。

命令格式为：docker network inspect [OPTIONS] NETWORK  [NETWORK... ]。

支持参数包括:

-f，--format="" :给定一个Golang模板字符串，对输出结果进行格式化，如只查看地址配置可以用-f  '{{.IPAM. Config}}';

-v，--verbose[=false]: 输出调试信息。

#### **6）清理无用网络**

prune命令用于清理已经没有容器使用的网络。

命令格式为：docker network prune  [OPTIONS]  [flags]，支持参数包括:

--filter="":指定选择过滤器;

-f，--force:强制清理资源。

#### **7）删除网络**

rm命令用于删除指定的网络。当网络上没有容器连接上时，才会成功删除。

命令格式为：docker network rm NETWORK  [NETWORK...]。



## 8.7、构建跨主机的容器网络

# 9 Docker Compose

## 9.1 compose 简介

Compose 项目是 Docker 官方的开源项目，其代码目前在https://github.com/docker/compose上开源。通过前面的学习，我们知道使用一个 Dockerfile 模板文件，可以让用户很方便的定义一个单独的应用容器。然而，在日常工作中，经常会碰到需要多个容器相互配合来完成某项任务的情况。例如要实现一个 Web 项目，除了 Web 服务容器本身，往往还需要再加上后端的数据库服务容器，甚至还包括负载均衡容器等。

**Compose** **就满足了上面的需求。**

它允许用户通过一个单独的 docker-compose.yml 模板文件（YAML 格式）来定义一组相关联的应用容器为一个项目（project）。

### **Compose** **中有几个重要的概念：**

- 任务 (task): 一个容器就是一个任务，有唯一的ID，同一个服务中多个任务ID依次增加。
- 服务 (service)：包括若干运行**相同镜像**的容器实例。
- 项目 (project)或者服务栈(stack)：由一组关联的服务组成的一个完整业务单元，在 docker-compose.yml 文件中定义。

Compose 的默认管理对象是项目，通过子命令对项目中的一组容器进行便捷地生命周期管理。Compose 项目由 Python 编写，实现上调用了 Docker 服务提供的 API 来对容器进行管理。因此，只要所操作的平台支持 Docker API，就可以在其上利用 Compose 来进行编排管理。

## 9.2 **安装与卸载**

Compose 支持 Linux、macOS、Windows 10 三大平台。Compose 可以通过 Python 的包管理工具 pip 进行安装，也可以直接下载编译好的二进制文件使用，甚至能够直接在 Docker 容器中运行。前两种方式是传统方式，适合本地环境下安装使用；最后一种方式则不破坏系统环境，更适合云计算场景。

容器安装：

Compose 既然是一个 Python 应用，自然也可以直接用容器来执行它。

```
$ curl -L https://github.com/docker/compose/releases/download/1.26.2/run.sh > /usr/local/bin/docker-compose

$ chmod +x /usr/local/bin/docker-compose
```

看run.sh脚本的内容，可以看到，它其实是下载了docker/compose镜像并运行



**有没有安装，查看一下版本信息：**

**$ docker-compose --version**  //执行这个命令会拉取镜像

**bash 补全命令**

```
$ curl -L https://raw.githubusercontent.com/docker/compose/1.26.2/contrib/completion/bash/docker-compose > /etc/bash_completion.d/docker-compose
```

这个命令再外网，可能拉取不下来，

先看看下面的docker-compose.yml例子，感官认识一下：

```yaml
version: "3.8"
services:
  webapp:
    image: tomcat:8.5
    ports:
      - "80:80"
    volumes:
      - "/data"
```

version: 指定 compose 文件的版本 ：https://docs.docker.com/compose/compose-file/

services: 定义所有的service信息,services下面的第一级别的 key 既是一个 service 的名称(如上面的web,db)

## 9.3 docker-compose.yml 指令

#### 1 **build**

指定 Dockerfile 所在文件夹的路径（可以是绝对路径，或者相对 docker-compose.yml 文件的路径）,Dockerfile 文件的文件名称也必须是默认文件名Dockerfile ,Compose 将会利用它自动构建这个镜像，然后使用这个镜像。

```yml
version: '3'
services:
  webapp:
    build: ./dir
```

也可以进一步使用 context 指令指定 Dockerfile 所在文件夹的路径，使用 dockerfile 指令指定 Dockerfile 文件名，使用 arg 指令指定构建镜像时的变量。

```yml
version: '3.8'
services:
  webapp:
    build:
      context: ./dir
      dockerfile: Dockerfile-alternate
      args:
        buildno: 1
```

#### **2）cap_add, cap_drop**

指定容器的内核能力（capacity）分配。例如，让容器拥有所有能力可以指定为：

```yml
cap_add:
  - ALL
#去掉 NET_ADMIN （比如：在线修改ip）能力可以指定为：
cap_drop:
  - NET_ADMIN
```

#### **3）command**

```yml
#覆盖容器启动后默认执行的命令，可以为字符串格式或数组格式。例如:
command: echo "hello world"
#或者:
command: ["bash", "-c"， "echo", "hello world"]
```

#### **4）configs**

仅用于 Swarm mode，详细内容请查看 Swarm内容。

#### **5）cgroup_parent**

指定父 cgroup 组，意味着将继承该组的资源限制。

例如，创建了一个 cgroup 组名称为 cgroups_1。

cgroup_parent: cgroups_1

#### **6）container_name**

指定容器名称。默认将会使用 `项目名称_服务名称_序号 `这样的格式。

container_name: docker-web-container

注意: 指定容器名称后，该服务将无法进行扩展（scale），因为 Docker 不允许多个容器具有相同的名称。

#### 7）depends_on

解决容器的依赖、启动先后的问题。以下例子中会先启动 redis db 再启动 web

```yml
version: '3'
services:
  web:
    build: .
    depends_on:
      - db
      - redis
  redis:
    image: redis
  db:
    image: postgres
```

注意：web 服务不会等待 redis db 「完全启动」之后才启动

#### **8）dns**

自定义 DNS 服务器。可以是一个值，也可以是一个列表。

```yml
dns: 8.8.8.8
dns:
  - 8.8.8.8
  - 114.114.114.114
```

#### **9）dns_search**

配置 DNS 搜索域。可以是一个值，也可以是一个列表。

```yml
dns_search: example.com
dns_search:
  - domain1.example.com
  - domain2.example.com
```

#### 10）tmpfs

挂载一个 tmpfs 文件系统到容器。

```yml
tmpfs: /run
tmpfs:
  - /run
  - /tmp
```

#### 11）env_file

从文件中获取环境变量，可以为单独的文件路径或列表。如果通过 docker-compose -f FILE 方式来指定 Compose 模板文件，则 env_file 中变量的路径会基于模板文件路径。如果有变量名称与 environment 指令冲突，则按照惯例，以后者为准。

```
env_file:
  - ./common.env
  - ./apps/web.env
  - /opt/secrets.env
```

环境变量文件中每一行必须符合格式，支持 # 开头的注释行。

\# common.env: Set development environment

PROG_ENV=development

#### **12）environment**

设置环境变量。你可以使用数组或字典两种格式。

只给定名称的变量会自动获取运行 Compose 主机上对应变量的值，可以用来防止泄露不必要的数据。

```yml
environment:
  RACK_ENV: development
  SESSION_SECRET:

environment:
  - RACK_ENV=development
  - SESSION_SECRET
  #如果变量名称或者值中用到 true|false，yes|no 等表达 布尔 含义的词汇，最好放到引号里，
#避免 YAML 自动解析某些内容为对应的布尔语义。这些特定词汇，包括
#y|Y|yes|Yes|YES|n|N|no|No|NO|true|True|TRUE|false|False|FALSE|on|On|ON|off|Off|OFF
```

#### 13）expose

暴露端口，但不映射到宿主机，只被连接的服务访问。仅可以指定内部端口为参数

```yml
expose:
    - "3000"
    - "8000"
```

#### 14）external_links （注意：不建议使用该指令。）

链接到 docker-compose.yml 外部的容器，甚至并非 Compose 管理的外部容器。

```yml
external_links:
  - redis_1
  - project_db_1:mysql
  - project_db_1:postgresql
```

#### 15）extra_hosts

类似 Docker 中的 --add-host 参数，指定额外的 host 名称映射信息。

```yml
extra_hosts:
 - "googledns:8.8.8.8"
 - "dockerhub:52.1.157.61"
```

会在启动后的服务容器中 /etc/hosts 文件中添加如下两条条目。

8.8.8.8 googledns

52.1.157.61 dockerhub

#### 16）healthcheck

通过命令检查容器是否健康运行。

```yml
healthcheck:
  test: ["CMD", "curl", "-f", "http://localhost"]
  interval: 1m30s
  timeout: 10s
  retries: 3
```

#### 17）image

指定为镜像名称或镜像 ID。如果镜像在本地不存在，Compose 将会尝试拉去这个镜像。

```yml
image: ubuntu
image: orchardup/postgresql
image: a4bc65fd
```

#### **18）labels**

为容器添加 Docker 元数据（metadata）信息。例如可以为容器添加辅助说明信息。

```yml
labels:
  com.startupteam.description: "webapp for a startup team"
  com.startupteam.department: "devops department"
  com.startupteam.release: "rc3 for v1.0"
```

#### **19）logging**

配置日志选项。

```yml
logging:
  driver: syslog
  options:
    syslog-address: "tcp://192.168.0.42:123"
#目前支持三种日志驱动类型。
driver: "json-file"
driver: "syslog"
driver: "none"
#options 配置日志驱动的相关参数。
options:
  max-size: "200k"
  max-file: "10"
```

#### **20）network_mode**

设置网络模式。使用和 docker run 的 --network 参数一样的值。

```yml
network_mode: "bridge"
network_mode: "host"
network_mode: "none"
network_mode: "service:[service name]"
network_mode: "container:[container name/id]"
```

#### **21）networks**

配置容器连接的网络。

```yml
version: '3'
services:
 proxy:
  build: ./proxy
  networks:
   - front
 app:
  build: ./app
  networks:
   - front
   - back
 db:
  image: postgres
  networks:
   - back
networks:
 front:
  # Use a custom driver
  driver: custom-driver-1
 back:
  # Use a custom driver which takes special options
  driver: custom-driver-2
  driver_opts:
   foo: "1"
   bar: "2"123456789101112131415161718192021222324252627
```

#### **22）pid**

跟主机系统共享进程命名空间。

打开该选项的容器之间，以及容器和宿主机系统之间可以通过进程 ID 来相互访问和操作。

pid: "host"

#### **23）ports**

暴露端口信息。

使用宿主端口：容器端口 (HOST:CONTAINER) 格式，或者仅仅指定容器的端口（宿主将会随机选择端口）都可以。

```yml
ports:
 - "3000"
 - "8000:8000"
 - "49100:22"
 - "127.0.0.1:8001:8001"
```

注意：当使用HOST:CONTAINER格式来映射端口时，如果你使用的容器端口小于 60 并且没放到引号里，可能会得到错误结果，因为YAML 会自动解析*xx:yy这种数字格式为 60 进制。为避免出现这种问题，建议数字串都采用引号包括起来的字符串格式。

#### **24）secrets**

存储敏感数据，例如 mysql 服务密码。

```yml
version: '3.8'
services:
  db:
    image: mysql:5.7.27
    volumes:
      - db_data:/var/lib/mysql
    environment:
      MYSQL_ROOT_PASSWORD_FILE: /run/secrets/db_root_password
      MYSQL_DATABASE: wordpress
      MYSQL_USER: wordpress
      MYSQL_PASSWORD_FILE: /run/secrets/db_password
    secrets:
      - db_root_password
      - db_password
  wordpress:
    depends_on:
      - db
    image: wordpress:latest
    ports:
      - "8888:80"
    environment:
      WORDPRESS_DB_HOST: db:3306
      WORDPRESS_DB_USER: wordpress
      WORDPRESS_DB_PASSWORD_FILE: /run/secrets/db_password
    secrets:
      - db_password
secrets:
  db_password:
    file: password.txt
  db_root_password:
    file: password.txt
volumes:
  db_data:
```

#### **25）ulimits**

指定容器的 ulimits 限制值。

例如，指定最大进程数为 65535，指定文件句柄数为 20000（软限制，应用可以随时修改，不能超过硬限制） 和 40000（系统硬限制，只能 root 用户提高）。

```yml
  ulimits:
    nproc: 65535
    nofile:
      soft: 20000
      hard: 40000
```

#### **26）volumes**

数据卷所挂载路径设置。可以设置宿主机路径 （HOST:CONTAINER） 或加上访问模式 （HOST:CONTAINER:ro）。

该指令中路径支持相对路径。

```yml
volumes:
 - /var/lib/mysql
 - cache/:/tmp/cache
 - ~/configs:/etc/configs/:ro
```



#### **27）其它指令，大多和docker run命令的参数对应**

**指定服务容器启动后执行的入口文件。**

entrypoint: /code/entrypoint.sh

**指定容器中运行应用的用户名。**

user: nginx

**指定容器中工作目录。**

working_dir: /code

**指定容器中搜索域名、主机名、mac 地址等。**

domainname: your_website.com

hostname: test

mac_address: 08-00-27-00-0C-0A

**允许容器中运行一些特权命令。**

privileged: true

**指定容器退出后的重启策略为始终重启。**

该命令对保持服务始终运行十分有效，在生产环境中推荐配置为 always 或者 unless-stopped。

restart: always

**以只读模式挂载容器的 root 文件系统，意味着不能对容器内容进行修改。**

read_only: true

**打开标准输入，可以接受外部输入。**

stdin_open: true

**模拟一个伪终端。**

tty: true

#### 28）读取变量

Compose 模板文件支持动态读取主机的系统环境变量和当前目录下的 .env 文件中的变量。

例如，下面的 Compose 文件将从运行它的环境中读取变量 ${MONGO_VERSION} 的值，并写入执行的指令中。

```yml
version: "3"
services:
db:
  image: "mongo:${MONGO_VERSION}"
```

若当前目录存在 .env 文件，执行 docker-compose 命令时将从该文件中读取变量。

在当前目录新建 .env 文件并写入以下内容。

\# 支持 # 号注释

MONGO_VERSION=3.6

## 9.4  例子

我们jpress这样一个开源网站的部署为例，来了解docker-compose的使用，

我们不必关注jpress的实现，在这里我们只需要将之当作一个普通的web应用即可。

**准备工作**

一共需要两个容器:

- Tomcat
- Mysql

然后需要jpress的war包，可以在jpress.io官网上下载。



### 1 编写Dockerfile

Tomcat容器中，要下载相关的war等，因此这里编写一个Dockerfile来做这个事，在一个空的文件夹下创建Dockerfile，内容如下:

```dockerfile
FROM tomcat:8.5
ADD jpress-v3.2.5.war  /usr/local/tomcat/webapps/
RUN cd /usr/local/tomcat/webapps/   && mv jpress-v3.2.5.war  jpress.war
```

### 2 **编写docker-compose.yml**

在相同的目录下创建docker-compose.yml内容如下:

```yml
version: "3.8"
services: 
  web: 
    build: .
    container_name: jpress
    ports: 
      - "8080:8080"
    volumes: 
      - /usr/local/tomcat/
    depends_on: 
      - db
  db: 
    image: mysql:5
    container_name: mysql
    command: --default-authentication-plugin=mysql_native_password
    restart: always 
    ports: 
      - "3306:3306"
    environment: 
      MYSQL_ROOT_PASSWORD: 123456
      MYSQL_DATABASE: jpress
```

默认情况下docker-compose up启动的容器都在前台，控制台将会同时打印所有容器的输出信息,

可以很方便进行调试,通过 Ctrl-C停止命令时，所有容器将会停止。

而如果使用docker-compose up -d命令，则将会在后台启动并运行所有的容器，一般推荐生产环境下使用该选项。进入到docker-compose.yml的目录下，执行以下命令:

```shell
docker-compose up -d
#停止项目：
docker-compose down
```

## 9.5 命令

**命令对象与格式**

对于 Compose 来说，大部分命令的对象既可以是项目本身，也可以指定为项目中的服务或者容器。如果没有特别的说明，命令对象将是**项目**，这意味着项目中所有的服务都会受到命令影响。

执行 docker-compose [COMMAND] --help  获取帮助

**命令选项**

-f, --file FILE 指定使用的 Compose 模板文件，默认为 docker-compose.yml，可以多次指定。

-p, --project-name NAME 指定项目名称，默认将使用所在**目录名称**作为项目名。

--x-network-driver DRIVER 指定网络后端的驱动，默认为 bridge

--verbose 输出更多调试信息。

-v, --version 打印版本并退出。

**命令使用说明**

#### config

验证 Compose 文件格式是否正确，若正确则显示配置，若格式错误显示错误原因。

#### down

此命令将会停止 up 命令所启动的容器，并移除网络。

#### exec

进入指定的容器。

如： docker-compose exec db /bin/bash

#### help

获得一个命令的帮助。

#### images

列出 Compose 文件中包含的镜像。

#### kill

格式为 docker-compose kill [options] [SERVICE...]。

通过发送 SIGKILL 信号来强制停止服务容器。

支持通过 -s 参数来指定发送的信号，例如通过如下指令发送 SIGINT 信号。

$ docker-compose kill -s SIGINT  web

#### logs

格式为 docker-compose logs [options] [SERVICE...]。

查看服务容器的输出。

默认情况下，docker-compose 将对不同的服务输出使用不同的颜色来区分。

可以通过 --no-color 来关闭颜色。该命令在调试问题的时候十分有用。

#### pause

格式为 docker-compose pause [SERVICE...]。

暂停一个服务容器。

#### unpause

格式为 docker-compose unpause [SERVICE...]。

恢复处于暂停状态中的服务。

#### port

格式为 docker-compose port [options] SERVICE PRIVATE_PORT。

查看某个容器端口所映射的公共端口。

选项：

--protocol=proto 指定端口协议，tcp（默认值）或者 udp。

--index=index 如果同一服务存在多个容器，指定命令对象容器的序号（默认为 1）。

\# docker-compose port db 3306

#### ps

格式为 docker-compose ps [options] [SERVICE...]。

列出项目中目前的所有容器。

选项：

-q 只打印容器的 ID 信息。

#### pull

格式为 docker-compose pull [options] [SERVICE...]。

拉取服务依赖的镜像。

选项：

--ignore-pull-failures 忽略拉取镜像过程中的错误。

#### push

推送服务依赖的镜像到 Docker 镜像仓库。

#### restart

格式为 docker-compose restart [options] [SERVICE...]。

重启项目中的服务。

选项：

-t, --timeout TIMEOUT 指定重启前停止容器的超时（默认为 10 秒）。

#### rm

格式为 docker-compose rm [options] [SERVICE...]。

删除所有（停止状态的）服务容器。推荐先执行 docker-compose stop 命令来停止容器。

选项：

-f, --force 强制直接删除，包括非停止状态的容器。一般尽量不要使用该选项。

-v 删除容器所挂载的数据卷。

#### start

格式为 docker-compose start [SERVICE...]。

启动已经存在的服务容器。

#### stop

格式为 docker-compose stop [options] [SERVICE...]。

停止已经处于运行状态的容器，但不删除它。通过 docker-compose start 可以再次启动这些容器。

选项：

-t, --timeout TIMEOUT 停止容器时候的超时（默认为 10 秒）。

#### top

查看各个服务容器内运行的进程。

#### version

格式为 docker-compose version。

#### build

格式为 docker-compose build [options] [SERVICE...]。

构建（重新构建）项目中的服务容器。

服务容器一旦构建后，将会带上一个标记名，例如对于 web 项目中的一个 db 容器，可能是 web_db。

可以随时在项目目录下运行 docker-compose build 来重新构建服务。

选项包括：

--force-rm 删除构建过程中的临时容器。

--no-cache 构建镜像过程中不使用 cache（这将加长构建过程）。

--pull 始终尝试通过 pull 来获取更新版本的镜像。

#### run

格式为 docker-compose run [options] [-p PORT...] [-e KEY=VAL...] SERVICE [COMMAND] [ARGS...]。

在指定服务上执行一个命令。

例如：

$ docker-compose run ubuntu ping docker.com

将会启动一个 ubuntu 服务容器，并执行 ping docker.com 命令。

默认情况下，如果存在关联，则所有关联的服务将会自动被启动，除非这些服务已经在运行中。

该命令类似启动容器后运行指定的命令，相关卷、链接等等都将会按照配置自动创建。

特点：

(1) 给定命令将会覆盖原有的自动运行命令；

(2) 不会自动创建端口，以避免冲突。

如果不希望自动启动关联的容器，可以使用 --no-deps 选项，例如

$ docker-compose run --no-deps web python manage.py shell

将不会启动 web 容器所关联的其它容器。

选项：

-d 后台运行容器。

--name NAME 为容器指定一个名字。

--entrypoint CMD 覆盖默认的容器启动指令。

-e KEY=VAL 设置环境变量值，可多次使用选项来设置多个环境变量。

-u, --user="" 指定运行容器的用户名或者 uid。

--no-deps 不自动启动关联的服务容器。

--rm 运行命令后自动删除容器，d 模式下将忽略。

-p, --publish=[] 映射容器端口到本地主机。

--service-ports 配置服务端口并映射到本地主机。

-T 不分配伪 tty，意味着依赖 tty 的指令将无法运行。

#### scale

格式为 docker-compose scale [options] [SERVICE=NUM...]。

设置指定服务运行的容器个数。

通过 service=num 的参数来设置数量。例如：

$ docker-compose scale web=3 db=2

将启动 3 个容器运行 web 服务，2 个容器运行 db 服务。

一般的，当指定数目多于该服务当前实际运行容器，将新创建并启动容器；反之，将停止容器。

#### up

格式为 docker-compose up [options] [SERVICE...]。

该命令十分强大，它将尝试自动完成包括构建镜像，（重新）创建服务，启动服务，并关联服务相关容器的一系列操作。

链接的服务都将会被自动启动，除非已经处于运行状态。

可以说，大部分时候都可以直接通过该命令来启动一个项目。

默认情况，docker-compose up 启动的容器都在前台，

控制台将会同时打印所有容器的输出信息，可以很方便进行调试。

当通过 Ctrl-C 停止命令时，所有容器将会停止。

如果使用 docker-compose up -d，将会在后台启动并运行所有的容器。**一般推荐生产环境下使用该选项。**

默认情况，如果服务容器已经存在，docker-compose up 将会尝试停止容器，

然后重新创建（保持使用 volumes-from 挂载的卷），以保证新启动的服务匹配 docker-compose.yml 文件的最新内容。

如果用户不希望容器被停止并重新创建，可以使用 docker-compose up --no-recreate。

这样将只会启动处于停止状态的容器，而忽略已经运行的服务。

如果用户只想重新部署某个服务，

可以使用 docker-compose up --no-deps -d  来重新创建服务并后台停止旧服务，

启动新服务，并不会影响到其所依赖的服务。

选项：

-d 在后台运行服务容器。

--no-color 不使用颜色来区分不同的服务的控制台输出。

--no-deps 不启动服务所链接的容器。

--force-recreate 强制重新创建容器，不能与 --no-recreate 同时使用。

--no-recreate 如果容器已经存在了，则不重新创建，不能与 --force-recreate 同时使用。

--no-build 不自动构建缺失的服务镜像。



# 10 docker Swarm

Docker Swarm 一句话说明：提供 Docker 容器集群服务，是 Docker 官方对容器云生态进行支持的核心方案。使用它，用户可以将多个 Docker 主机封装为单个大型的虚拟 Docker 主机，快速打造一套容器云平台。

注意：从Docker 1.12.0+ Swarm mode 已经内嵌入 Docker 引擎，成为了 docker 子命令 docker swarm，现在已经开始使用 Swarm mode，Swarm作为一个单独项目已经不在持续开发了。这里标题还是Docker三剑客之Swarm，但实质内容我们讲Docker内置的Swarm mode：

## 10.1 基本概念

Swarm 是 Docker 引擎内置（原生）的集群管理和编排工具。

使用 Swarm 集群之前需要了解以下几个概念。

**1）节点**

运行 Docker 的主机可以主动初始化一个 Swarm 集群或者加入一个已存在的 Swarm 集群，这样这个运行 Docker 的主机就成为一个 Swarm 集群的节点 (node) 。

节点分为**管理 (manager) 节点和工作 (worker)** 节点：

管理节点用于 Swarm 集群的管理，docker swarm 命令基本只能在管理节点执行（节点退出集群命令 docker swarm leave 可以在工作节点执行。一个 Swarm 集群可以有多个管理节点，但只有一个管理节点可以成为 leader，leader 通过 raft 协议实现。工作节点是任务执行节点，工作节点执行的任务就是由管理节点下发的服务 (service) 。管理节点默认也作为工作节点。也可以通过配置让服务只运行在管理节点。

**来自 Docker 官网的这张图片形象的展示了集群中管理节点与工作节点的关系：**

![](https://gitee.com/CTLQAQ/picgo/raw/master/69.png)

**2）服务和任务**

任务 （Task）是 Swarm 中的最小的调度单位，目前来说就是一个单一的容器。

服务 （Services） 是指一组任务的集合，服务定义了任务的属性。

服务有两种模式：

- replicated services 按照一定规则在各个工作节点上运行指定个数的任务。
- global services 每个工作节点上运行一个任务

两种模式通过 docker service create 的 --mode 参数指定。来自 Docker 官网的这张图片形象的展示了容器、任务、服务的关系。

![](https://gitee.com/CTLQAQ/picgo/raw/master/70.png)

## 10.2  创建swarm集群

我们以创建一个包含一个管理节点和两个工作节点的最小 Swarm 集群为例子。

提前准备好三个主机。

**1）初始化集群**

我们使用 docker swarm init 在本机初始化一个 Swarm 集群。

避免防火墙在创建集群过程中干扰，建议关掉在实验。

**停止firewall**

systemctl stop firewalld.service

**关闭SELINUX**

setenforce 0    //临时关闭

**清空iptables规则**

iptables -F

iptables-save

```
$ docker swarm init --advertise-addr 192.168.17.128
```

**2）增加工作节点**

上一步我们在本机初始化了一个 Swarm 集群，拥有了一个管理节点，

进入work1的主机：

```shell
#看上一步的输出
docker@worker2:~$ docker swarm join \
    --token SWMTKN-1-49nj1cmql0jkz5s954yi3oex3nedyz0fb0xx14ie39trti4wxv-8vxv8rssmk743ojnwacrr2e7c \
    192.168.17.128:2377
```

**3）查看集群**

经过上边的两步，我们已经拥有了一个最小的 Swarm 集群，包含一个管理节点和两个工作节点。

在管理节点使用 docker node ls 查看集群。

**$ docker node ls    只能在管理节点上执行**

## 10.3 **部署服务**

我们使用 docker service 命令来管理 Swarm 集群中的服务，该命令只能在管理节点运行。

**1）新建服务**

现在我们在上一节创建的 Swarm 集群中运行一个名为 nginx 服务。

docker service create --replicas 3 -p 80:80 --name nginx nginx:1.13.7-alpine

现在我们使用浏览器，输入任意节点 IP ,即可看到 nginx 默认页面。

**--replicas 3：** Number of tasks

**2）查看服务**

使用 docker service ls 来查看当前 Swarm 集群运行的服务。

**$ docker service ls**

使用 docker service ps 来查看某个服务的详情。

使用 docker service logs 来查看某个服务的日志。

**$ docker service logs nginx**

**3）删除服务**

使用 docker service rm 来从 Swarm 集群移除某个服务。

**$ docker service rm nginx**



## **10.4、在 Swarm 集群中使用 compose 文件**

正如之前使用 docker-compose.yml 来一次配置、启动多个容器，在 Swarm 集群中也可以使用 compose 文件 （docker-compose.yml） 来配置、启动多个服务。上一节中，我们使用 docker service create 一次只能部署一个服务，使用 docker-compose.yml 我们可以一次启动多个关联的服务。我们以在 Swarm 集群中部署 WordPress 为例进行说明。

```yml
version: "3.8"
services:
  wordpress:
    image: wordpress
    ports:
      - 80:80
    networks:
      - overlay
    environment:
      WORDPRESS_DB_HOST: db:3306
      WORDPRESS_DB_USER: wordpress
      WORDPRESS_DB_PASSWORD: wordpress
    deploy:
      mode: replicated
      replicas: 2
 
  db:
    image: mysql
    networks:
       - overlay
    volumes:
      - db-data:/var/lib/mysql
    environment:
      MYSQL_ROOT_PASSWORD: somewordpress
      MYSQL_DATABASE: wordpress
      MYSQL_USER: wordpress
      MYSQL_PASSWORD: wordpress
    deploy:
      placement:
        constraints: [node.role == manager]
 
  visualizer:
    image: dockersamples/visualizer:stable
    ports:
      - "8080:8080"
    stop_grace_period: 1m30s
    volumes:
      - "/var/run/docker.sock:/var/run/docker.sock"
    deploy:
      placement:
        constraints: [node.role == manager] 
volumes:
  db-data:
networks:
  overlay:
```



```yml
deploy    # v3 版本以上, 指定与部署和运行服务相关的配置, deploy 部分是 docker stack 使用的, docker stack 依赖 docker swarm
            endpoint_mode     # v3.3 版本中新增的功能, 指定服务暴露的方式
                vip      # Docker 为该服务分配了一个虚拟 IP(VIP), 作为客户端的访问服务的地址
                dnsrr    # DNS轮询, Docker 为该服务设置 DNS 条目, 使得服务名称的 DNS 查询返回一个 IP 地址列表
            labels                # 指定服务的标签，这些标签仅在服务上设置
            mode                  # 指定 deploy 的模式
                global                # 每个集群节点都只有一个容器
                replicated            # 用户可以指定集群中容器的数量(默认)
            placement             # 控制容器部署时的位置
            replicas              # deploy 的 mode 为 replicated 时, 指定容器副本的数量，说白了就是服务下运行几个容器
            resources             # 资源限制
                limits                # 设置容器的资源限制
                    cpus: "0.5"           # 设置该容器最多只能使用 50% 的 CPU 
                    memory: 50M           # 设置该容器最多只能使用 50M 的内存空间 
                reservations          # 设置为容器预留的系统资源(随时可用)
                    cpus: "0.2"           # 为该容器保留 20% 的 CPU
```

