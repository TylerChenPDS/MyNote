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

