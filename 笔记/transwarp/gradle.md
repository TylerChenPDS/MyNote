https://blog.csdn.net/MrMyGod/article/details/104761696

# 安装配置

https://gradle.org/releases/ 下载6.7

解压后配置环境变量

![image-20210108112032763](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108112032763.png)

![image-20210108163138195](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108163138195.png)

可以将第二个变量配置成maven仓库的位置 

然后配置path

`%GRADLE_HOME%\bin`

# 构建第一个java项目

https://docs.gradle.org/current/samples/sample_building_java_applications.html

```
gradle init //初始化项目
gradle run //运行项目
gradle build //（归档）打包成jar 和 zip  app/build/distributions/app.tar and 				app/build/distributions/app.zip.
gradle build --scan  //查看build幕后的事情
```



## gradle init

输入`gradle init`命令后，当询问开始的时候，分别输入：`2: application`,`3: Java`,`1 : Groovy`

然后生成文件的目录结构如下：

![image-20210108133843562](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108133843562.png)

1. gradle的wrapper文件

2. 可以代替gradle命令的文件

3. gradle的配置文件

   ```
   rootProject.name = 'demo' //工程名称，
   include('app') //子项目名称
   ```

4. app的配置信息

   ```
   plugins {
       // Apply the application plugin to add support for building a CLI application in Java.
       id 'application'
   }
   
   repositories {
       // Use JCenter for resolving dependencies.
       jcenter()
   }
   
   dependencies {
       // Use JUnit test framework.
       testImplementation 'junit:junit:4.13'
   
       // This dependency is used by the application.
       implementation 'com.google.guava:guava:29.0-jre'
   }
   
   application {
       // Define the main class for the application.
       mainClass = 'demo.App'
   }
   ```



# 关于仓库的设置

https://docs.gradle.org/current/userguide/declaring_repositories.html

流行的公共仓库包括：Maven Central, binary Jcenter等，

## 使用不同的仓库

### 使用*Maven repository*

```
repositories {
    mavenCentral()
}
```

### 使用*Bintray’s JCenter Maven repository*

```
repositories {
    jcenter()
}
```

### 声明使用多种仓库源

```groovy
repositories {
    //先从本地仓库中寻找
    mavenLocal()
    //然后从这个仓库下载
    maven { url 'http://maven.aliyun.com/nexus/content/repositories/central/' }
    //最后从中央仓库下载
    mavenCentral()
}
```



# Grovvy简单使用

```groovy
println ("hello")
//可以省略末尾的分号，括号
println "hello11111"

//定义变量
def i = 18
println i
//定义字符串
def a = "aaaaaa"
println a

//定义list
def list = ['a','a']
list << "增加一个元素"
println list[2]

//map
def map = ['key' : 'hhh','key2': 'aaa']
map.key3 = '第三个key'
println map.key3

//定义闭包(就是代码块)
def bb= {
    println '闭包'
}
//定义一个函数 ，Closure是闭包类型
def method(Closure closure){
    closure()
}

method(bb)

//带参数的闭包
def bb2 = {
    v ->
        println "hello ${v}"
}

def method2(Closure closure){
    closure('带参数的闭包')
}
method2(bb2)
```





