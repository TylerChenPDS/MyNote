# K8S运行部署

## deployment

```yml
# API 版本号
apiVersion: apps/v1
# 类型，如：Pod/ReplicationController/Deployment/Service/Ingress
kind: Deployment
metadata:
  # Kind 的名称
  name: nginx-app
spec:
  selector:
    matchLabels:
      # 容器标签的名字，发布 Service 时，selector 需要和这里对应
      app: nginx
  # 部署的实例数量
  replicas: 2
  template:
    metadata:
      labels:
        app: nginx
    spec:
      # 配置容器，数组类型，说明可以配置多个容器
      containers:
      # 容器名称
      - name: nginx
        # 容器镜像
        image: nginx:1.17
        # 只有镜像不存在时，才会进行镜像拉取
        imagePullPolicy: IfNotPresent
        ports:
        # Pod 端口
        - containerPort: 80
```

**应用**	

```
kubectl apply -f nginx.yml 
```

**删除由nginx.yml创建的所有服务**

```
kubectl delete -f nginx.yml 
```

 **删除名字中带有http的pod**

```shell
kubectl delete pods $(kubectl get pods | grep http | tr -s ' ' | cut -d ' ' -f1)
```

**查看所有弄得及其部属信息**

```
kubectl get pods -o wide
```

**出于安全考虑， 默认配置下Kubernetes不会将Pod调度到Master节点。 如果希望将k8s-master也当作Node使用， 可以执行如下命令：**  

```
kubectl taint node k8s-master node-role.kubernetes.io/master-
```

**如果要恢复Master Only状态， 执行如下命令：**  

```
kubectl taint node k8s-master node-role.kubernetes.io/master=""
```

**给某个结点打上标签，并查看**

```
kubectl label node node01 disktype=ssd

kubectl get node --show-labels
```

**为什么要打标签？**

​		默认配置下， Scheduler会将Pod调度到所有可用的Node。 不过有些情况我们希望将Pod部署到指定的Node， 比如将有大量磁盘I/O的Pod部署到配置了SSD的Node； 或者Pod需要GPU， 需要运行在配置了GPU的节点上。Kubernetes是通过label来实现这个功能的。    



**使用标签将对应pod部署到打标签的node上**

```yml
# API 版本号
apiVersion: apps/v1
# 类型，如：Pod/ReplicationController/Deployment/Service/Ingress
kind: Deployment
metadata:
  # Kind 的名称
  name: nginx-app
spec:
  selector:
    matchLabels:
      # 容器标签的名字，发布 Service 时，selector 需要和这里对应
      app: nginx
  # 部署的实例数量
  replicas: 2
  template:
    metadata:
      labels:
        app: nginx
    spec:
      nodeSelector:
        disktype: ssd
      # 配置容器，数组类型，说明可以配置多个容器
      containers:
        # 容器名称
        - name: nginx
          # 容器镜像
          image: nginx:1.17
          # 只有镜像不存在时，才会进行镜像拉取
          imagePullPolicy: IfNotPresent
          ports:
            # Pod 端口
            - containerPort: 80
```

**删除标签**

```
kubectl label node k8s-node1 disktype-
```

## DaemonSet  

Deployment部署的副本Pod会分布在各个Node上， 每个Node都可能运行好几个副本。 DaemonSet的不同之处在于： 每个Node上最多只能运行一个副本  



kubectl edit deployment nginx-deployment  

## JOB

容器按照持续运行的时间可分为两类： **服务类容器和工作类容器**。  **服务类容器通常持续提供服务， 需要一直运行， 比如HTTPServer、 Daemon等。 工作类容器则是一次性任务， 比如批处理程序，完成后容器就退出。**  

```yml
apiVersion: batch/v1
kind: Job
metadata:
  name: pi
spec:
  template:
    metadata:
      name: pi
    spec:
      containers:
      - name: pi
        image: busybox
        command: ["echo",  "hello world"]
      restartPolicy: Never
```

restartPolicy指定什么情况下需要重启容器。 对于Job， 只能设置为Never或者OnFailure。 对于其他controller（比如Deployment） ，可以设置为Always。  

**应用后查看输出**

![image-20201228145521921](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201228145521921.png)

### 并行job

![image-20201228150044166](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201228150044166.png)

上面配置的含义是： 每次运行两个Pod， 直到总共有6个Pod成功完成 。

### 定时JOB

![image-20201228150210443](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201228150210443.png)

然后：vim  /etc/kubernetes/manifests/kube-apiserver.yaml 在里面加上一句话

![image-20201228150649590](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201228150649590.png)

然后重启kubectl服务

systemctl restart kubelet.service  

然后再apply就可以成功了



# Service

通过service访问pod。Kubernetes Service从逻辑上代表了一组Pod， 具体是哪些Pod则是由label来挑选的。 **Service有自己的IP， 而且这个IP是不变的。 客户端只需要访问Service的IP， Kubernetes则负责建立和维护Service与Pod的映射关系。 无论后端Pod如何变化， 对客户端不会有任何影响， 因为Service没有变**  

## 一个栗子

```yml
# API 版本号
apiVersion: apps/v1
# 类型，如：Pod/ReplicationController/Deployment/Service/Ingress
kind: Deployment
metadata:
  # Kind 的名称
  name: nginx
spec:
  selector:
    matchLabels:
      # 容器标签的名字，发布 Service 时，selector 需要和这里对应
      app: nginx
  # 部署的实例数量
  replicas: 3
  template:
    metadata:
      labels:
        app: nginx
    spec:
      # 配置容器，数组类型，说明可以配置多个容器
      containers:
      # 容器名称
      - name: nginx
        # 容器镜像
        image: nginx:1.17
        # 只有镜像不存在时，才会进行镜像拉取
        imagePullPolicy: IfNotPresent
        ports:
        # Pod 端口
        - containerPort: 80
```



**创建service的配置文件**

```yml
#serviceapi版本
apiVersion: v1
kind: Service
metadata:
  name: nginx-svc
spec:
# 挑选那些label为run：nginx的pod作为service的后端
  selector:
  #这个和上面 第18行对应
    app: nginx
  ports:
    - protocol: TCP
      port: 8080
      targetPort: 80
```

sed -i 's/httpd/nginx/g' service 将里面的http都替换成nginx

**然后使用下面这个命令可以查看对应的service**

kubectl describe service nginx-svc

![image-20201230153441568](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230153441568.png)

## 外网访问Service

Kubernetes提供了多种类型的Service， 默认是ClusterIP。  

**ClusterIP**

Service通过Cluster内部的IP对外提供服务， 只有Cluster内的节点和Pod可访问， 这是默认的Service类型。

**NodePort**  

Service通过Cluster节点的静态端口对外提供服务。 Cluster外部可以通过`<NodeIP>:<NodePort>`访问Service。  

**LoadBalancer**  

Service利用cloud provider特有的load balancer对外提供服务，cloud provider负责将load balancer的流量导向Service。 目前支持的cloud provider有GCP、 AWS、 Azur等。  

### NodePort栗子

```yml
#serviceapi版本
apiVersion: v1
kind: Service
metadata:
  name: nginx-svc
spec:
# 挑选那些label为run：nginx的pod作为service的后端
  selector:
  #这个和上面 第18行对应
    app: nginx
  type: NodePort
  ports:
    - protocol: TCP
      port: 8080
      targetPort: 80
```

重新apply一下之后，![image-20201230161336001](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230161336001.png)

然后可以通过

```
http://192.168.96.123:30174/  http://192.168.96.124:30174/ http://192.168.96.124:30174/
```

通过三个节点IP+30174端口都能够访问nginx-svc。  



NodePort端口可以自己定义

![image-20201230161643565](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230161643565.png)

- nodePort是节点上监听的端口。  
- port是ClusterIP上监听的端口。  
- targetPort是Pod监听的端口。  

# Rolling Update   滚动更新



kubectl apply每次更新应用时， Kubernetes都会记录下当前的配置， 保存为一个revision（版次） ， 这样就可以回滚到某个特定revision。  默认配置下， Kubernetes只会保留最近的几个revision， 可以在Deployment配置文件中通过**revisionHistoryLimit**属性增加revision数量  



apply的时候，加上--record可以把版本信息记录下来

```
kubectl apply -f http.v1.yml --record
```

然后可以查看版本信息

```
kubectl rollout history deployment httpd  
```

![image-20201230163050128](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230163050128.png)

然后可以回滚到上一个版本

```
kubectl rollout undo deployment httpd --to-revision=1  
```

# Health Check  健康检查

## Liveness探测  

Liveness探测让用户可以自定义判断容器是否健康的条件。 如果探测失败， Kubernetes就会重启容器。  

![image-20201230163909130](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230163909130.png)

启动进程首先创建文件/tmp/healthy， 30秒后删除， 在我们的设定中， 如果/tmp/healthy文件存在， 则认为容器处于正常状态， 反之则发生故障。  

**探测的方法是**： 通过cat命令检查/tmp/healthy文件是否存在。 如果命令执行成功， 返回值为零， Kubernetes则认为本次Liveness探测成功； 如果命令返回值非零， 本次Liveness探测失败。  

**initialDelaySeconds**： 10指定容器启动10之后开始执行Liveness探测， 我们一般会根据应用启动的准备时间来设置。 比如某个应用正常启动要花30秒， 那么initialDelaySeconds的值就应该大于30。  

**periodSeconds**： 5指定每5秒执行一次Liveness探测。Kubernetes如果连续执行3次Liveness探测均失败， 则会杀掉并重启容器。  

## Readiness探测  

用户通过Liveness探测可以告诉Kubernetes什么时候通过重启容器实现自愈； Readiness探测则是告诉Kubernetes什么时候可以将容器加入到Service负载均衡池中， 对外提供服务。  

![image-20201230164206599](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230164206599.png)

## Liveness探测和Readiness探测   比较

1. Liveness探测和Readiness探测是两种Health Check机制， 如果不特意配置， Kubernetes将对两种探测采取相同的默认行为， 即通过判断容器启动进程的返回值是否为零来判断探测是否成功  
2. 两种探测的配置方法完全一样， 支持的配置参数也一样。不同之处在于探测失败后的行为： Liveness探测是重启容器；Readiness探测则是将容器设置为不可用， 不接收Service转发的请求。 
3. Liveness探测和Readiness探测是独立执行的， 二者之间没有依赖， 所以可以单独使用， 也可以同时使用。 用Liveness探测判断容器是否需要重启以实现自愈； 用Readiness探测判断容器是否已经准备好对外提供服务。   

## Health Check在Scale Up中的应用  

执行Scale Up操作时， 新副本会作为backend被添加到Service的负载均衡中， 与已有副本一起处理客户的请求。   从容器启动到真正能够提供服务是需要一段时间的。 我们可以通过Readiness探测判断容器是否就绪， 避免将请求发送到还没有准备好的backend  



![image-20201230165250919](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230165250919.png)

这里使用httpGet进行探测，Kubernetes对于该方法探测成功的判断条件是http请求的返回代码在200～400之间。  path指定访问路径。  schema指定协议， 支持HTTP（默认值） 和HTTPS。  

作用是：容器启动10秒之后  ，如果http://[container_ip]:8080/healthy返回代码不是200～400， 表示容器没有就绪， 不接收Service web-svc的请求。  每隔5秒探测一次。  直到返回代码为200～400， 表明容器已经就绪， 然后将其加入到web-svc的负载均衡中， 开始处理客户请求。 然后， 探测会继续以5秒的间隔执行， 如果连续发生3次失败， 容器又会从负载均衡中移除， 直到下次探测成功重新加入。  

## Health Check在滚动更新中的应用  

一个正常运行的多副本应用， 接下来对应用进行更新（比如使用更高版本的image）  

1，正常情况下新副本需要10秒钟完成准备工作， 在此之前无法响应业务请求。 

2， 由于人为配置错误， 副本始终无法完成准备工作（比如无法连接后端数据库） 。  

因为新副本本身没有异常退出， 默认的Health Check机制会认为容器已经就绪， 进而会逐步用新副本替换现有副本， 其结果就是： 当所有旧副本都被替换后， 整个应用将无法处理请求  



# 数据管理  

## Volume  

Volume的生命周期独立于容器， Pod中的容器可能被销毁和重建， 但Volume会被保留。 当Volume被mount到Pod， Pod中的所有容器都可以访问这个Volume。   

### emptyDir  

emptyDir Volume的生命周期与Pod一致。  当Pod从节点删除时， Volume的内容也会被删除。 但如果只是容器被销毁而Pod还在， 则Volume不受影响。  

配置栗子：

![image-20201230171606514](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201230171606514.png)

emptyDir是Host上创建的临时目录， 其优点是能够方便地为Pod中的容器提供共享存储， 不需要额外的配置。 它不具备持久性， 如果Pod不存在了， emptyDir也就没有了。 根据这个特性， emptyDir特别适合Pod中的容器需要临时共享存储空间的场景  

### hostPath  

hostPath Volume的作用是将Docker Host文件系统中已经存在的目录mount给Pod的容器。 大部分应用都不会使用hostPath Volume， 因为这实际上增加了Pod与节点的耦合。

### 外部Storage Provider     

如果Kubernetes部署在诸如AWS、 GCE、 Azure等公有云上， 可以直接使用云硬盘作为Volume。  





# PersistentVolume & PersistentVolumeClaim  

PersistentVolume（PV） 是外部存储系统中的一块存储空间， 由管理员创建和维护。 与Volume一样， PV具有持久性， 生命周期独立于Pod。 

 PersistentVolumeClaim（PVC） 是对PV的申请（Claim） 。 PVC通常由普通用户创建和维护。 需要为Pod分配存储资源时， 用户可以创建一个PVC， 指明存储资源的容量大小和访问模式（比如只读） 等信息， Kubernetes会查找并提供满足条件的PV。  有了PersistentVolumeClaim， 用户只需要告诉Kubernetes需要什么样的存储资源， 而不必关心真正的空间从哪里分配、 如何访问等底层细节信息。 这些Storage Provider的底层信息交给管理员来处理， 只有管理员才应该关心创建PersistentVolume的细节信息。  

## NFS PersistentVolume  

1. 首先在master节点上搭建nfs服务器

2. 然后创建nfs-pv1.yml

   ![image-20210107145048477](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107145048477.png)

   1. capacity指定PV的容量为1GB。  

   2. accessModes指定访问模式为ReadWriteOnce， 支持的访问模式
      有3种：

      ReadWriteOnce表示PV能以read-write模式mount到单个节点，
      ReadOnlyMany表示PV能以read-only模式mount到多个节点，
      ReadWriteMany表示PV能以read-write模式mount到多个节点。  

   3. persistentVolumeReclaimPolicy指定当PV的回收策略为Recycle， 支持的策略有3种： Retain表示需要管理员手工回收；Recycle表示清除PV中的数据， 效果相当于执行rm -rf/thevolume/*；Delete表示删除Storage Provider上的对应存储资源  

   4. storageClassName指定PV的class为nfs。 相当于为PV设置了一个分类， PVC可以指定class申请相应class的PV    

   5. 指定PV在NFS服务器上对应的目录。  

3. 执行命令`kubectl apply -f nfs-pv1.yml`

4. ![image-20210107145648931](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107145648931.png)

   STATUS为Available， 表示mypv1就绪， 可以被PVC申请。  

5. 接下来创建nfs-pvc1.yml

   ![image-20210107145810165](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107145810165.png)

   ![image-20210107145932156](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107145932156.png)

6. 接下来就可以在Pod中使用存储了  

   ![image-20210107150019134](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107150019134.png)

7. 实验

   ![image-20210107150111974](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107150111974.png)

## 回收pv

1. 首先删除pvc

   `kubectl delete pvc mypvc1`

   Kubernetes启动了一个新Podrecycler-for-mypv1， 这个Pod的作用就是清除PV mypv1的数据。 此时
   mypv1的状态为Released，  当数据清除完毕， mypv1的状态重新变为Available， 此时可以被新的PVC申请  。

## PV动态供给  

提前创建了PV， 然后通过PVC申请PV并在Pod中使用， 这种方式叫作**静态供给**（Static Provision） 。  

如果没有满足PVC条件的PV， 会动态创建PV  ，叫做 **动态供给**

动态供给是通过StorageClass实现的， StorageClass定义了如何创建PV

## 为MySQL数据库提供持久化存储的栗子

### 1, 首先创建PV和PVC， 配置说明如下  

mysql-pv.yml

![image-20210107154243083](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107154243083.png)

mysql-pvc.yml  

![image-20210107154303977](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107154303977.png)



执行命令

```
kubectl apply -f mysql-pv.yml
kubectl apply -f mysql-pvc.yml
```

### 2 部署mysql

![image-20210107154551493](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107154551493.png)

PVC mysql-pvc Bound的PV mysql-pv将被mount到MySQL的数据目录var/lib/mysql  

### 3 测试

mysql服务会被部署到一个node节点上，

```
kubectl run -it --rm --image=mysql:5.6 --restart=Never mysql-cli
```

向数据库种插入一条数据。关掉部署mysql那个节点。过段时间，发现应用被部署到另外一个节点上，然后查询，发现数据还在。

# Secret &Configmap  

## Secret 

Secret会以密文的方式存储数据， **避免了直接在配置文件中保存敏感信息**。 Secret会以**Volume**的形式被mount到Pod， 容器可通过文件的方式使用Secret中的敏感数据； 此外， 容器也可以环境变量的方式使用这些数据。  

### 创建Secret

1. 通过--from-literal  

   `kubectl create secret generic mysecret --from-literal=username=***`

2. 通过--form-file

   ![image-20210107172527176](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107172527176.png)

3. 通过--from-env-file：  ![image-20210107172542052](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107172542052.png)

4. 通过YAML配置文件  

   ![image-20210107172606120](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107172606120.png)

然后`kubectl apply -f mysecret.yml`

文件中的敏感数据必须是通过base64编码后的结果。`echo admin | base64`

`echo -n YWFhY2EK | base64 --decode `//将密码反编码

### 查看secret

```
kubectl get secret //查看存在的secret
kubectl describe secret myscret //查看myscret的具体信息，和key
kubectl edit secret mysecret //查看编码后的value
echo -n YWFhY2EK | base64 --decode //将密码反编码
```

### 在pod中使用secret

Pod可以通过Volume或者环境变量的方式使用Secret。  

#### 通过Volume 



![image-20210107173525172](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107173525172.png)

**创建Pod并在容器中读取Secret**  

![image-20210107173607267](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107173607267.png)

**也可以自定义存放数据的文件名**  

![image-20210107173914027](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107173914027.png)





以Volume方式使用的Secret支持动态更新： Secret更新后， 容器中的数据也会更新 。

#### 环境变量方式  

通过Volume使用Secret， 容器必须从文件读取数据， 稍显麻烦，Kubernetes还支持通过环境变量使用Secret。  

1，创建pod

![image-20210107174116135](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107174116135.png)

创建Pod并读取Secret  

![image-20210107174203283](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107174203283.png)

环境变量读取Secret很方便， 但无法支撑Secret动态更新。

## ConfigMap    

Secret可以为Pod提供密码、 Token、 私钥等敏感数据； 对于一些非敏感数据， 比如应用的配置信息， 则可以用ConfigMap。  ConfigMap的创建和使用方式与Secret非常类似， 主要的不同是数据以明文的形式存放。  

### 创建ConfigMap

与Secret一样， ConfigMap也支持四种创建方式：  

前三种不过就是前面的命令变成了`kubectl create configmap myconfigmap  `。

通过yml方式

![image-20210107174638268](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107174638268.png)

### 使用

用法和secret也是一样的

### Volume

![image-20210107174833218](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107174833218.png)

#### 环境变量

![image-20210107174812794](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107174812794.png)

# Kubernetes的包管理器  Helm  

## 为什么需要Helm

对于一个mysql服务，k8s需要部署：下面东西

1. Service， 让外界能够访问到MySQL  

   ![image-20210107175623559](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107175623559.png)

2. Secret， 定义MySQL的密码  

   ![image-20210107175644015](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107175644015.png)

3. PersistentVolumeClaim， 为MySQL申请持久化存储空间  

   ![image-20210107175706785](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210107175706785.png)

4. Deployment， 部署MySQL Pod， 并使用上面的这些支持对象  