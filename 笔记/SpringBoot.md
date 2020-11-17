# 1.介绍Spring Boot  2.4.0

Spring Boot帮助您创建可以运行的独立的，基于生产级的基于Spring的应用程序。我们对Spring平台和第三方库持固执己见的观点，这样您就可以以最小的麻烦开始使用。大多数Spring Boot应用程序只需要很少的Spring配置。

您可以使用Spring Boot创建Java应用程序，可以通过使用`java -jar`或更传统的`war`部署来启动Java应用程序。我们还提供了一个运行“ spring脚本”的命令行工具。

我们的主要目标是：

- 为所有Spring开发提供根本上更快且可广泛访问的入门体验。
- 开箱即用。Be opinionated out of the box but get out of the way quickly as requirements start to diverge from the defaults.
- 提供一系列大型项目通用的非功能性功能（例如嵌入式服务器，安全性，指标，运行状况检查和外部化配置）。
- 完全不需要代码生成，也不需要XML配置。

# 2 系统要求

Spring Boot 2.4.0需要[Java 8，](https://www.java.com/)并且与Java 15（包括）兼容。 还需要[Spring Framework 5.3.1](https://docs.spring.io/spring/docs/5.3.1/reference/html/)或更高版本。

为以下构建工具提供了明确的构建支持：

| **Build Tool** | **Version**                                                  |
| -------------- | ------------------------------------------------------------ |
| Maven          | 3.3+                                                         |
| Gradle         | 6 (6.3 or later). 5.6.x is also supported but in a deprecated form |

## 2.1 Servlet容器

Spring Boot支持以下嵌入式servlet容器：

| 名称         | Servlet版本 |
| :----------- | :---------- |
| Tomcat 9.0   | 4.0         |
| Jetty 9.4    | 3.1         |
| Undertow 2.0 | 4.0         |

您还可以将Spring Boot应用程序部署到任何Servlet 3.1+兼容的容器中。

# 3.安装Spring Boot

## 3.1 Java开发人员的安装说明

Spring Boot依赖项使用`org.springframework.boot` `groupId`。通常，您的Maven POM文件从`spring-boot-starter-parent`项目继承，并声明对一个或多个[“启动器”的](https://docs.spring.io/spring-boot/docs/2.4.0/reference/html/using-spring-boot.html#using-boot-starter)依赖关系。Spring Boot还提供了一个可选的[Maven插件](https://docs.spring.io/spring-boot/docs/2.4.0/reference/html/build-tool-plugins.html#build-tool-plugins-maven-plugin)来创建可执行jar。