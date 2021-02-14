

# IOC 加载过程

![image-20210125183755706](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210125183755706.png)



![image-20210125184253843](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210125184253843.png)

大致流程如下

- ClassPathXmlApplicationContext 和 AnnotationConfigApplicationContext 通过BeanDefinitionReader 读取配置类

- 然后BeanDefinitionScanner读取需要扫描的类
- BeanDefinitionRegistry 读取类定义（BeanDefinition）并将其加载到BeanDefinitionMap中。
- 最终BeanFactory 负责把对象创建出来，并将其放到Map （一个单例池中）

扩展点 

BeanFactoryPostProcesor  和 BeanDefinitionRegistryPostProcessor 是为了解耦而定义的，其他组件如Mybatis的整合等都要用到这些。 



## IOC核心注解的使用

### @ComponentScan

```
@Configuration
@ComponentScan(basePackages = {"com.tuling.testcompentscan"})
public class MainConfig {
}

```

排除用法

```java
@Configuration
@ComponentScan(basePackages = {"com.tuling.testcompentscan"},excludeFilters = {
@ComponentScan.Filter(type = FilterType.ANNOTATION,value = {Controller.class}),
@ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE,value = {TulingService.class})
})
```

包含用法，**需要把useDefaultFilters属性设置为false（true表 示扫描全部的）**

```java
@Configuration
@ComponentScan(basePackages = {"com.tuling.testcompentscan"},includeFilters = {
@ComponentScan.Filter(type = FilterType.ANNOTATION,value = {Controller.class, Service.class})
},useDefaultFilters = false)
public class MainConfig {
}
```

@ComponentScan.Filter type的类型

> a)注解形式的FilterType.ANNOTATION @Controller @Service @Repository @Compent
> b)指定类型的 FilterType.ASSIGNABLE_TYPE @ComponentScan.Filter(type =
> FilterType.ASSIGNABLE_TYPE,value = {TulingService.class})
> c)aspectj类型的 FilterType.ASPECTJ(不常用)
> d)正则表达式的 FilterType.REGEX(不常用)
> e)自定义的 FilterType.CUSTOM

FilterType.CUSTOM 自定义类型如何使用

```java
public class TulingFilterType implements TypeFilter {
    @Override
    public boolean match(MetadataReader metadataReader, MetadataReaderFactory metadataReaderFactory) throws IOException {
        //获取当前类的注解源信息
        AnnotationMetadata annotationMetadata = metadataReader.getAnnotationMetadata();
        //获取当前类的class的源信息
        ClassMetadata classMetadata = metadataReader.getClassMetadata();
        //获取当前类的资源信息
        Resource resource = metadataReader.getResource();
        if(classMetadata.getClassName().contains("dao")) {
            return true;
        }
        return false;
    }
}
@ComponentScan(basePackages = {"com.tuling.testcompentscan"},includeFilters = {
@ComponentScan.Filter(type = FilterType.CUSTOM,value = TulingFilterType.class)
},useDefaultFilters = false)
public class MainConfig {
}
```

### 配置Bean的作用域

在不指定@Scope的情况下，所有的bean都是单实例的bean,而且是饿汉加载(容器启动实例就创建 好了)

指定@Scope为 prototype 表示为多实例的，而且还是懒汉模式加载（IOC容器启动的时候，并不会创建对象，而是 在第一次使用的时候才会创建）

```java
@Bean
@Scope(value = "prototype")
public Person person() {
    return new Person();
}

```

@Scope指定的作用域方法取值 

a) singleton 单实例的(默认) 

b) prototype 多实例的 

c) request 同一次请求 

d) session 同一个会话级别



Bean的懒加载@Lazy(主要针对单实例的bean 容器启动的时候，不创建对象，在第一次使用的时候才会创建该对象)

```java
@Bean
@Lazy
public Person person() {
    return new Person();	
}

```

### @Conditional进行条件判断等

@Conditional(value = TulingCondition.class)

//当切 容器中有tulingAspect的组件，那么tulingLog才会被实例化.

```java
public class TulingCondition implements Condition {

    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) 	{
        //判断容器中是否有tulingAspect的组件
        if(context.getBeanFactory().containsBean("tulingAspect")) {
            return true;
        }
        return false;
    }
}
public class MainConfig {
    @Bean
    public TulingAspect tulingAspect() {
        return new TulingAspect();
    }
    //当切 容器中有tulingAspect的组件，那么tulingLog才会被实例化.
    @Bean
    @Conditional(value = TulingCondition.class)
    public TulingLog tulingLog() {
        return new TulingLog();
    }
}
```

### 向IOC容器中添加组件的方式

1. 通过@CompentScan +@Controller @Service @Respository @compent 适用场景: 针对我们自己写的组件可以通过该方式来进行加载到容器中。

2. 通过@Bean的方式来导入组件(适用于导入第三方组件的类

3. 通过@Import来导入组件 （导入组件的id为全类名路径）

   ```java
   @Configuration
   @Import(value = {Person.class, Car.class})
   public class MainConfig {	
   }
   
   ```

   通过@Import 的ImportSeletor类实现组件的导入 (导入组件的id为全类名路径)

   ```java
   public class TulingImportSelector implements ImportSelector {
   	//可以获取导入类的注解信息
   	@Override
       public String[] selectImports(AnnotationMetadata importingClassMetadata) {
           return new String[]{"com.tuling.testimport.compent.Dog"};
           }
       }
   }
   @Configuration
   @Import(value = {Person.class, Car.class, TulingImportSelector.class})
   public class MainConfig {
   }
   ```

   通过@Import的 ImportBeanDefinitionRegister导入组件 (**可以指定bean的名称**)

   ```java
   public class TulingBeanDefinitionRegister implements ImportBeanDefinitionRegistrar {
       @Override
       public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
           //创建一个bean定义对象
           RootBeanDefinition rootBeanDefinition = new RootBeanDefinition(Cat.class);
           //把bean定义对象导入到容器中
           registry.registerBeanDefinition("cat",rootBeanDefinition);
       }
   }
   @Configuration
   @Import(value = {Person.class, Car.class, TulingImportSelector.class, TulingBeanDefinitionRegister.class})
   public class MainConfig {
   }
   ```

4. 通过实现FacotryBean接口来实现注册 组件

   ```java
   public class CarFactoryBean implements FactoryBean<Car> {
       //返回bean的对象
       @Override
       public Car getObject() throws Exception {
           return new Car();
       }
       //返回bean的类型
       @Override
       public Class<?> getObjectType() {
           return Car.class;
       }
       //是否为单例
       @Override
       public boolean isSingleton() {
           return true;
       }
   }
   ```

### Bean的生命周期

针对单实例bean的话，容器启动的时候，bean的对象就创建了，而且容器销毁的时候，也会调用Bean的销毁方法 

针对多实例bean的话,容器启动的时候，bean是不会被创建的而是在获取bean的时候被创建，而且bean的销毁不受 IOC容器的管理.

1. @Bean(initMethod = "init",destroyMethod = "destroy") 可以指定特定Bean初始化前，和销毁后的方法

2. 通过 InitializingBean和DisposableBean 的二个接口实现bean的初始化以及销毁方法

   ```java
   @Component
   public class Person implements InitializingBean,DisposableBean {
       public Person() {
           System.out.println("Person的构造方法");
       }
       @Override
       public void destroy() throws Exception {
           System.out.println("DisposableBean的destroy()方法 ");
       }
       @Override
       public void afterPropertiesSet() throws Exception {
           System.out.println("InitializingBean的 afterPropertiesSet方法");
       }
   }
   ```

3. 通过JSR250规范 提供的注解@PostConstruct 和@ProDestory标注的方法

   ```java
   @Component
   public class Book {
       public Book() {
           System.out.println("book 的构造方法");
       }
       @PostConstruct
       public void init() {
           System.out.println("book 的PostConstruct标志的方法");
       }
       @PreDestroy
       public void destory() {
           System.out.println("book 的PreDestory标注的方法");
       }
   }
   ```

4. 通过Spring的**BeanPostProcessor** （接口）的 bean的后置处理器会拦截所有bean创建过程 

   postProcessBeforeInitialization 在init方法之前调用 （接口中的方法）

   postProcessAfterInitialization 在init方法之后调用（接口中的方法）

### 指定外部 配置文件

```java
@Configuration
@PropertySource(value = {"classpath:person.properties"}) //指定外部文件的位置
public class MainConfig {
}
```

### 自动装配@Autowire  @Qualifier("tulingDao")

自动装配首**先时按照类型进行装配，若在IOC容器中发现了多个相同类型的组件，那么就按照 属性名称来进行装配**

比如，我容器中有二个TulingDao类型的组件 一个叫tulingDao 一个叫tulingDao2 那么我们通过@AutoWired 来修饰的属性名称时tulingDao，那么拿就加载容器的tulingDao组件，若属性名称为 tulignDao2 那么他就加载的时tulingDao2组件

假设我们需要指定特定的组件来进行装配，我们可以通过使用@Qualifier("tulingDao")来指定装配的组件

```
@Autowired
@Qualifier("tulingDao")
private TulingDao tulingDao2;
```

### `*****aware`

我们自己的组件 需要使用spring ioc的底层组件的时候,比如 ApplicationContext等，我们可以通过实现XXXAware接口来实现,

下面实例，使用了ApplicationContext 和 自己设置BeanName

```java
@Component
public class TulingCompent implements ApplicationContextAware,BeanNameAware {
private ApplicationContext applicationContext;
    @Override
    public void setBeanName(String name) {
        System.out.println("current bean name is :【"+name+"】");
    }
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }
}

```

### @Profile注解 来根据环境来激活标识不同的Bean

@Profile标识在类上，那么只有当前环境匹配，整个配置类才会生效 

@Profile标识在Bean上 ，那么只有当前环境的Bean才会被激活

没有标志为@Profile的bean 不管在什么环境都可以被激活

```java
//标识为测试环境才会被装配
@Bean
@Profile(value = "test")
public DataSource testDs() {
    return buliderDataSource(new DruidDataSource());
}

```

## 问题

1.描述下BeanFactory。 

2.BeanFactory和ApplicationContext的区别？ 

3.简述SpringIoC的加载过程。

4.简述Bean的生命周期。 

5.Spring中有哪些扩展接口及调用时机。

# 编译源代码

源码下载地址

https://github.com/spring-projects/spring-framework/tree/v5.2.7.RELEASE

步骤

https://note.youdao.com/ynoteshare1/index.html?id=c18f0f8b8bae3dd7e4ff292f043fe932&type=note

遇到的问题及解决

https://blog.csdn.net/fxp850899969/article/details/78229758

https://www.it610.com/article/1295306604677242880.htm





# IOC容器加载源码阅读笔记

## Bean定义

先看下什么是Bean定义 

它是用来描述Bean的，里面存放着关于Bean的一系列信息，比如Bean的作用域，Bean所对应的Class，是 否懒加载，是否Primary等等





![image-20210126200456898](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126200456898.png)



![image-20210126200840485](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126200840485.png)

## 进入this()

![image-20210126201807860](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126201807860.png)

//**会隐式调用父类的构造方法，初始化DefaultListableBeanFactory**  

可以先进入到其父容器里面看下 GenericApplicationContext

![image-20210126202004525](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126202004525.png)

DefaultListableBeanFactory是相当重要的，从字面意思就可以看出它是一个Bean的工厂

出去后然后调用下面这句话

### this.reader = new AnnotatedBeanDefinitionReader(this); 

里面主要做了2件事

1. 注册内置BeanPostProcessor
2. 注册相关的BeanDefinition

继续往里面探索 ，

![image-20210126212421712](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126212421712.png)

框中的方法，会注册所有registry中的postPreocessor,  这里的registry指的是 Annotated...Context ，它也实现了BeanDefinitionRegistry这个方法。

继续进入这个方法

![image-20210126213114723](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126213114723.png)

1. 判断容器中是否已经存在了ConfigurationClassPostProcessor Bean `org.springframework.context.annotation.internalConfigurationAnnotationProcessor`
2. 如果不存在（当然这里肯定是不存在的），就通过RootBeanDefinition的构造方法获得 ConfigurationClassPostProcessor的BeanDefinition，RootBeanDefinition是BeanDefinition的子类 

​	3. 执行registerPostProcessor方法，registerPostProcessor方法内部就是注册Bean，当然这里注册 其他Bean也是一样的流程。

继续进入registerPostProcessor这个方法中

![image-20210126213825230](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126213825230.png)

发现这个角色被打上了ROLE_INFRASTRUCTURE 这个标签，如下图注释，可以看到：这是一个后再角色，和终端用户无关

![image-20210126214004989](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126214004989.png)

上面第二张图的倒数第二句，发现它是个接口方法，通过调试工具可得：发现这个registry其实它的实现类是`AnnotationConfigApplicationContext`, 然后进入`AnnotationConfigApplicationContext`中发现里面没有对应的`registerBeanDefinition` 方法，然后找到他的父类，终于得到`registerBeanDefinition` 方法定义 如下：

![image-20210126214743791](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126214743791.png)



点进去看下，发现核心就2行

![image-20210126214924918](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126214924918.png)

**DefaultListableBeanFactory中的beanDefinitionMap，beanDefinitionNames也是相当重要**。这里仅仅是注册，可以简单的理解为把一些原料放入工厂，工厂还没有真正的去生产。

那么刚才注入进去的是个啥东西呢？

![image-20210126215331350](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126215331350.png)

就是最后一个红框里面的东西。



除了注册了**ConfigurationClassPostProcessor**，还注册了其他Bean，其他Bean也都实现了其他接口，比 如BeanPostProcessor等。 BeanPostProcessor接口也是Spring的扩展点之一。

### this.scanner = new ClassPathBeanDefinitionScanner(this);

由于常规使用方式是不会用到AnnotationConfigApplicationContext里面的scanner的，这里的scanner 仅仅是为了程序员可以手动调用AnnotationConfigApplicationContext对象的scan方法。所以这里就不看 scanner是如何被实例化的了。

## register(componentClasses);

这个commponentClasses就是配置类，一般只有一个，然后深入进入，对于里面的每一个配置类Class，都会执行下面的方法

![image-20210126220903865](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126220903865.png)

![image-20210126221150406](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126221150406.png)

![image-20210126221248591](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126221248591.png)

这个registerBeanDefinition是不是又有一种似曾相似的感觉，没错，在上面注册Spring内置的Bean的时 候，已经解析过这个方法了，这里就不重复了，此时，让我们再观察下beanDefinitionMap beanDefinitionNames两个变量，除了Spring内置的Bean，还有我们传进来的Bean，这里的Bean当然就 是我们的配置类

## refresh()

此时 Spring还没有进行扫描，只是实例化了一个工厂，注册了一些内置的Bean和我 们传进去的配置类

点进去发现里面又很多方法，只看重要的方法

### prepareBeanFactory

里面主要做的事为：

1. 设置了一个类加载器 
2. 设置了bean表达式解析器 
3.  添加了属性编辑器的支持
4. 添加了一个后置处理器：ApplicationContextAwareProcessor，此后置处理器实现了BeanPostProcessor接口 
5. 设置了一些忽略自动装配的接口 
6. 设置了一些允许自动装配的接口，并且进行了赋值操作
7. 注册默认的环境变量的单例bean

### invokeBeanFactoryPostProcessors(beanFactory);

![image-20210126223343674](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210126223343674.png)

### 进入invokeBeanFactoryPostProcessors() 里面

边看代码，边看解析：

1. 定义了一个Set，装载BeanName，后面会根据这个Set，来判断后置处理器是否被执行过了。
2. 定义了两个List，一个是regularPostProcessors，用来装载BeanFactoryPostProcessor，一个是registryProcessors用来装载 BeanDefinitionRegistryPostProcessor，其中BeanDefinitionRegistryPostProcessor扩展了BeanFactoryPostProcessor。 BeanDefinitionRegistryPostProcessor有两个方法，一个是独有的postProcessBeanDefinitionRegistry方法，一个是父类的 postProcessBeanFactory方法。
3.  循环传进来的beanFactoryPostProcessors，上面已经解释过了，一般情况下，这里永远都是空的，只有手动add beanFactoryPostProcessor，这里才会有数据。我们假设beanFactoryPostProcessors有数据，进入循环，判断postProcessor是 不是BeanDefinitionRegistryPostProcessor，因为BeanDefinitionRegistryPostProcessor扩展了BeanFactoryPostProcessor， 所以这里先要判断是不是BeanDefinitionRegistryPostProcessor，是的话，执行postProcessBeanDefinitionRegistry方法，然后 把对象装到registryProcessors里面去，不是的话，就装到regularPostProcessors。
4. 定义了一个临时变量：currentRegistryProcessors，用来装载BeanDefinitionRegistryPostProcessor。
5. . getBeanNamesForType，顾名思义，是根据类型查到BeanNames，这里有一点需要注意，就是去哪里找，点开这个方法的 话，就知道是循环**beanDefinitionNames（beanName的列表）**去找，这个方法以后也会经常看到。。一般情况下，只会找到一个，就是 ·、`org.springframework.context.annotation.internalConfigurationAnnotationProcessor`，也就是 `ConfigurationAnnotationProcessor`。
6. 循环postProcessorNames，其实也就是 org.springframework.context.annotation.internalConfigurationAnnotationProcessor，判断此后置处理器是否实现了 PriorityOrdered接口（ConfigurationAnnotationProcessor也实现了PriorityOrdered接口）， 如果实现了，把它添加到currentRegistryProcessors这个临时变量中，再放入processedBeans，代表这个后置处理已经被处理过了
7. 进行排序，PriorityOrdered是一个排序接口，如果实现了它，就说明此后置处理器是有顺序的，所以需要排序。当然目前这里只 有一个后置处理器，就是ConfigurationClassPostProcessor。
8. 把currentRegistryProcessors合并到registryProcessors，为什么需要合并？因为一开始spring只会执行 BeanDefinitionRegistryPostProcessor独有的方法，而不会执行BeanDefinitionRegistryPostProcessor父类的方法，即 BeanFactoryProcessor接口中的方法，所以需要把这些后置处理器放入一个集合中，后续统一执行BeanFactoryProcessor接口中 的方法。当然目前这里只有一个后置处理器，就是ConfigurationClassPostProcessor。
9. invokeBeanDefinitionRegistryPostProcessors(currentRegistryProcessors, registry);可以理解为执行currentRegistryProcessors中的ConfigurationClassPostProcessor中的postProcessBeanDefinitionRegistry 方法，这就是Spring设计思想的体现了，在这里体现的就是其中的热插拔，插件化开发的思想。
10. 



