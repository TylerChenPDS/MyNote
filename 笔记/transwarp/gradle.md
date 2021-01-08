# 安装配置

https://gradle.org/releases/ 下载6.7

解压后配置环境变量

![image-20210108112032763](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108112032763.png)

然后配置path

`%GRADLE_HOME%\bin`

# 构建第一个java项目

https://docs.gradle.org/current/samples/sample_building_java_applications.html

```
gradle init //初始化项目
gradle run //运行项目
gradle build //（归档）打包成jar 和 zip  app/build/distributions/app.tar and 				app/build/distributions/app.zip.
gradle build --scan  //查看build幕后做的事情
```

