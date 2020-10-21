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

![](./img/33.png)在容器镜像服务中，创建镜像，然后点击管理，里面有详细步骤。

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

