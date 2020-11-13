# OverView：history, design philosophy, feedback, getting started.

- 截至Spring 5.1，Spring需要JDK 8+， Java SE 8 update 60 是最低的补丁版本。
- Spring拥有庞大且活跃的开源社区，根据真实的使用实例提供持续的反馈，这帮助Spring成功地演化。

## 设计理念

- 提供各层次的选择。 Spring允许您尽可能延迟设计决策。 例如，您可以通过配置切换持久化提供者，而无需更改代码。



# core 

## 1 The IoC Container

### 1.1 Introduction to the Spring IoC **Container** and Beans

​		Inversion of Control (IoC) 也被称为依赖注入dependency injection (DI)。**它是一个过程**，对象只通过构造函数参数、工厂方法的参数、从工厂方法构造、返回的对象实例上设置的属性来定义它们的依赖关系。**随后，当IOC容器创建一个bean的时候，会将这些依赖注入到对应的对象中。**

 `org.springframework.beans` and `org.springframework.context` 包，是IOC容器的基础包，`BeanFactory`接口提供了一种高级配置机制，能够管理任何类型的对象 `ApplicationContext` 是`BeanFactory`的子接口增加了很多特性：如AOP特性更容易集成，消息资源处理（国际化），时间发布，应用层特性的上下文环境如：`webApplicationContext`。



`Bean`在spring中是 一个由Spring IoC 容器初始化、装配、和管理的对象。