# Spring 5.28

## spring概述

- Spring 是轻量级的开源的 JavaEE 框架
- Spring 可以解决企业应用开发的复杂性
- Spring 有两个核心部分：IOC 和 Aop 
  - （1）IOC：控制反转，把创建对象过程交给 Spring 进行管理 
  - （2）Aop：面向切面，不修改源代码进行功能增强

Spring 特点 

（1）方便解耦，简化开发 

（2）Aop 编程支持 

（3）方便程序测试 

（4）方便和其他框架进行整合 

（5）方便进行事务操作

（6）降低 API 开发难度

## 导入核心jar包

![image-20201215214149126](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201215214149126.png)

```xml
<dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>
        <!-- https://mvnrepository.com/artifact/org.springframework/spring-aop -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.springframework/spring-aspects -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aspects</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-expression</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.2</version>
        </dependency>


        <!-- https://mvnrepository.com/artifact/com.alibaba/druid -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.9</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/mysql/mysql-connector-java -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.47</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.springframework/spring-jdbc -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.springframework/spring-orm -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-orm</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.springframework/spring-tx -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>5.2.8.RELEASE</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.junit.jupiter/junit-jupiter-api -->
        <dependency>
            <groupId>org.junit.jupiter</groupId>
            <artifactId>junit-jupiter-api</artifactId>
            <version>5.2.0</version>
            <scope>test</scope>
        </dependency>


        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>5.3</version>
            <scope>compile</scope>
        </dependency>
    	<dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.10</version>
        </dependency>



    </dependencies>
```

完整的schema

```xml
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-4.2.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context-4.2.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop-4.2.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx-4.2.xsd">
```



## IOC

### 什么是 IOC

（1）控制反转，把对象创建和对象之间的调用过程，交给 Spring 进行管理 

（2）使用 IOC 目的：为了耦合度降低

#### Spring 提供 IOC 容器实现两种方式：（两个接口） 

（1）BeanFactory：IOC 容器基本实现，是 Spring 内部的使用接口，不提供开发人员进行使用 * 加载配置文件时候不会创建对象，在获取对象（使用）才去创建对象 

（2）ApplicationContext：BeanFactory 接口的子接口，提供更多更强大的功能，一般由开发人 员进行使用 * 加载配置文件时候就会把在配置文件对象进行创建

### 注入方式

- 第一种注入方式：使用 set 方法进行注入

- 第二种注入方式：使用有参数构造进行注入 （1）创建类，定义属性，创建属性对应有参数构造方法



```java
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class Book {
	private String bname;
	private String bauthor;
	public void setBname(String bname) {
		this.bname = bname;
	}
	public void setBauthor(String bauthor) {
		this.bauthor = bauthor;
	}
}
```



```xml
<!-- set注入 -->
<bean id="book" class="test.Book">
    <property name="bname" value="九阳神功"></property>
    <property name="bauthor" value="尼古拉斯"></property>
</bean>

<!-- 构造参数注入-->
<bean id="book1" class="test.Book">
    <constructor-arg name="bname" value="十阳神功"></constructor-arg>
    <constructor-arg name="bauthor" value="尼古拉斯二世"></constructor-arg>
</bean>
```

#### property 标签

##### 字面量

可以注入字面量或者null，也可以包含特殊字符

```xml
<!--null 值-->
<property name="address">
 <null/>
</property>

<!--属性值包含特殊符号
 1 把<>进行转义 &lt; &gt;
 2 把带特殊符号内容写到 CDATA
-->
<property name="address">
 <value><![CDATA[<<南京>>]]></value>
</property>
```

##### 注入bean

###### 注入内部bean

```java
public class UserService {
 	//创建 UserDao 类型属性，生成 set 方法
    private UserDao userDao;
    public void setUserDao(UserDao userDao) {
        this.userDao = userDao;
    }
}
```

```xml
<!--1 service 和 dao 对象创建-->
<bean id="userService" class="com.atguigu.spring5.service.UserService">
 <!--注入 userDao 对象
 name 属性：类里面属性名称
 ref 属性：创建 userDao 对象 bean 标签 id 值
 -->
 <property name="userDao" ref="userDaoImpl"></property>
</bean>
<bean id="userDaoImpl" class="com.atguigu.spring5.dao.UserDaoImpl"></bean>
```

###### 注入外部bean

```java
public class Dept {
    private String dname;
    public void setDname(String dname) {
        this.dname = dname;
    }
}
//员工类
public class Emp {
    //员工属于某一个部门，使用对象形式表示
    private Dept dept;
    public void setDept(Dept dept) {
        this.dept = dept;
    }
}
```

```xml
<!--内部 bean-->
<bean id="emp" class="com.atguigu.spring5.bean.Emp">
 	<!--设置对象类型属性-->	
     <property name="dept">
         <bean id="dept" class="com.atguigu.spring5.bean.Dept">
            <property name="dname" value="安保部"></property>
         </bean>
     </property>
</bean>

```

###### 注入List , Map , Set

```java
@Data
public class Stu {
     //1 数组类型属性
     private String[] courses;
     //2 list 集合类型属性
     private List<String> list;
     //3 map 集合类型属性
     private Map<String,String> maps;
     //4 set 集合类型属性
     private Set<String> sets;
}

```

```xml
<!--1 集合类型属性注入-->
<bean id="stu" class="com.atguigu.spring5.collectiontype.Stu">
 <!--数组类型属性注入-->
     <property name="courses">
         <array>
             <value>java 课程</value>
             <value>数据库课程</value>
         </array>
     </property>
 <!--list 类型属性注入-->
 <property name="list">
     <list>
         <value>张三</value>
         <value>小三</value>
     </list>
 </property>
 <!--map 类型属性注入-->
 <property name="maps">
     <map>
         <entry key="JAVA" value="java"></entry>
         <entry key="PHP" value="php"></entry>
     </map>
 </property>
 <!--set 类型属性注入-->
 <property name="sets">
     <set>
         <value>MySQL</value>
         <value>Redis</value>
     </set>
 </property>
</bean>

```

集合里面可以设置对象

```xml
<property name="courseList">
     <list>
         <ref bean="course1"></ref>
         <ref bean="course2"></ref>
     </list>
</property>
```

###### 集合元素提取 

引入工具util

![image-20201215222533627](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201215222533627.png)

```xml
<util:list id="bookList">
     <value>易筋经</value>
     <value>九阴真经</value>
     <value>九阳神功</value>
</util:list>
<!--2 提取 list 集合类型属性注入使用-->
<bean id="book" class="com.atguigu.spring5.collectiontype.Book">
 	<property name="list" ref="bookList"></property>
</bean>
```

### IOC 操作 Bean 管理

#### IOC 工厂bean

1、Spring 有两种类型 bean，一种普通 bean，另外一种工厂 bean（FactoryBean） 

2、普通 bean：在配置文件中定义 bean 类型就是返回类型 

3、工厂 bean：在配置文件定义 bean 类型可以和返回类型不一样



第一步 创建类，让这个类作为工厂 bean，实现接口 FactoryBean 
第二步 实现接口里面的方法，在实现的方法中定义返回的 bean 类型



```java
public class MyBean implements FactoryBean<Course> {
     //定义返回 bean
     @Override
     public Course getObject() throws Exception {
         Course course = new Course();
         course.setCname("abc");
         return course;
     }
     @Override
     public Class<?> getObjectType() {
         return null;
     }
     @Override
     public boolean isSingleton() {
         return false;
     }
    @Test
    public void test3() {
         ApplicationContext context =
         new ClassPathXmlApplicationContext("bean3.xml");
         Course course = context.getBean("myBean", Course.class);
         System.out.println(course);
    }
}


```

```xml
<bean id="myBean" class="com.atguigu.spring5.factorybean.MyBean">
</bea
```

#### bean 作用域

在 Spring 里面，默认情况下，bean 是单实例对象。在 spring 配置文件 bean 标签里面有属性（**scope**）用于设置单实例还是多实例。两个值：singleton 和 prototype

区别：

- 设置 scope 值是 singleton 时候，加载 spring 配置文件时候就会创建单实例对象 

- 设置 scope 值是 prototype 时候，不是在加载 spring 配置文件时候创建 对象，在调用 getBean 方法时候创建多实例对象

#### bean 生命周期

##### 普通生命周期

1. 通过构造器创建 bean 实例（无参数构造）
2. 为 bean 的属性设置值和对其他 bean 引用（调用 set 方法）
3. 调用 bean 的初始化的方法（需要进行配置初始化的方法）
4. bean 可以使用了（对象获取到了）
5. 当容器关闭时候，调用 bean 的销毁的方法（需要进行配置销毁的方法）

示例：

```java
public class Orders {
 //无参数构造
 public Orders() {
 	System.out.println("第一步 执行无参数构造创建 bean 实例");
 }
 private String oname;
 public void setOname(String oname) {
 	this.oname = oname;
 	System.out.println("第二步 调用 set 方法设置属性值");
 }
 //创建执行的初始化的方法
 public void initMethod() {
 	System.out.println("第三步 执行初始化的方法");
 }
 //创建执行的销毁的方法
 public void destroyMethod() {
 	System.out.println("第五步 执行销毁的方法");
 }
}
```

```xml
<bean id="orders" class="com.atguigu.spring5.bean.Orders" initmethod="initMethod" destroy-method="destroyMethod">
 <property name="oname" value="手机"></property>
</bean>
```

##### bean 的后置处理器(之于所有的bean)

1. 通过构造器创建 bean 实例（无参数构造）
2. 为 bean 的属性设置值和对其他 bean 引用（调用 set 方法）
3. **把 bean 实例传递 bean 后置处理器的方法 postProcessBeforeInitialization** 
4. 调用 bean 的初始化的方法（需要进行配置初始化的方法）
5. **把 bean 实例传递 bean 后置处理器的方法 postProcessAfterInitialization**
6. bean 可以使用了（对象获取到了）
7. 当容器关闭时候，调用 bean 的销毁的方法（需要进行配置销毁的方法）

```java
public class MyBeanPost implements BeanPostProcessor {
     @Override
     public Object postProcessBeforeInitialization(Object bean, String beanName)
    throws BeansException {
         System.out.println("在初始化之前执行的方法");
         return bean;
     }
	 @Override
     public Object postProcessAfterInitialization(Object bean, String beanName)
    throws BeansException {
         System.out.println("在初始化之后执行的方法");
         return bean;
     }
}
```

```java
<!--配置后置处理器-->
<bean id="myBeanPost" class="com.atguigu.spring5.bean.MyBeanPost"></bean>
```

#### 自动装配

根据指定装配规则（属性名称或者属性类型），Spring 自动将匹配的属性值进行注入

#### 引入外部配置文件

![image-20201215225048551](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201215225048551.png)

```xml

<!--引入外部属性文件-->
<context:property-placeholder location="classpath:jdbc.properties"/>
<!--配置连接池-->
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
     <property name="driverClassName" value="${prop.driverClass}"></property>
     <property name="url" value="${prop.url}"></property>
     <property name="username" value="${prop.userName}"></property>
     <property name="password" value="${prop.password}"></property>
</bean>
```



#### 基于注解的配置

```xml
<!--开启组件扫描
     1 如果扫描多个包，多个包使用逗号隔开
     2 扫描包上层目录
    -->
<context:component-scan base-package="test"></context:component-scan>


<!--示例 1
	只扫描目标包中的Controller注解
 use-default-filters="false" 表示现在不使用默认 filter，自己配置 filter
 context:include-filter ，设置扫描哪些内容
-->
<context:component-scan base-package="com.atguigu" use-defaultfilters="false">
 <context:include-filter type="annotation"

expression="org.springframework.stereotype.Controller"/>
</context:component-scan>
<!--示例 2
 只不扫描对应包中的controller注解
 下面配置扫描包所有内容
 context:exclude-filter： 设置哪些内容不进行扫描
-->
<context:component-scan base-package="com.atguigu">
 <context:exclude-filter type="annotation"

expression="org.springframework.stereotype.Controller"/>
</context:component-scan>
```



完全基于注解

```java
@Configuration //作为配置类，替代 xml 配置文件
@ComponentScan(basePackages = {"test"})
public class SpringConfig {
	@Test
	public void test(){
		ApplicationContext context =  new AnnotationConfigApplicationContext(SpringConfig.class);
		Object book = context.getBean("book");
		System.out.println(book);
	}
}
```

## AOP

### 什么是 AOP

- 面向切面编程（方面），利用 AOP 可以对业务逻辑的各个部分进行隔离，从而使得 业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。
- 通俗描述：不通过修改源代码方式，在主干功能里面添加新功能

术语

- 连接点：类里面哪些方法可以被增强，这些方法称为连接点。
- 切入点：实际被增强的方法。
- 通知（增强）：实际增强的逻辑部分。
- 切面：（动作）把通知应用到切入点的过程。



### 例子：

```xml
<context:component-scan base-package="test"></context:component-scan>
<aop:aspectj-autoproxy></aop:aspectj-autoproxy>
```





```java
@Component
public class User {
	public void add(){
		System.out.println("add......");
	}
}


@Component
@Aspect
public class UserProxy {
	//前置通知
	//@Before 注解表示作为前置通知
	@Before(value = "execution(* test.aop.User.add(..))")
	public void before() {
		System.out.println("before.........");
	}

	//后置通知（返回通知）
	@AfterReturning(value = "execution(* test.aop.User.add(..))")
	public void afterReturning() {
		System.out.println("afterReturning.........");
	}


	@After(value = "execution(* test.aop.User.add(..))")
	public void after() {
		System.out.println("after.........");
	}

	//异常通知
	@AfterThrowing(value = "execution(*	test.aop.User.add(..))")
	public void afterThrowing() {
		System.out.println("afterThrowing.........");
	}

	//环绕通知
	@Around(value = "execution(* test.aop.User.add(..))")
	public void around(ProceedingJoinPoint proceedingJoinPoint) throws
			Throwable {
		System.out.println("环绕之前.........");
		proceedingJoinPoint.proceed();
		System.out.println("环绕之后.........");
	}
}


public class MyTest {
	@Test
	public void test(){
		ApplicationContext context = new ClassPathXmlApplicationContext("bean1.xml");
		User user = context.getBean("user", User.class);
		user.add();
	}
}
```

结果：

![image-20201217123516285](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217123516285.png)

相同的切入点可以被提取

```java
@Pointcut(value = "execution(* com.atguigu.spring5.aopanno.User.add(..))")
public void pointdemo() {
}

@Before(value = "pointdemo()")
public void before() {
 	System.out.println("before.........");
}
```

完全基于注解

```java
@Configuration
@ComponentScan(basePackages = {"com.atguigu"})
@EnableAspectJAutoProxy(proxyTargetClass = true)
public class ConfigAop {
}
```



基于配置文件的配置

```xml
<!--    <context:component-scan base-package="test"></context:component-scan>-->
<bean id="user" class="test.aop.User"></bean>
<bean id="userProxy" class="test.aop.UserProxy"></bean>
<!--    <aop:aspectj-autoproxy></aop:aspectj-autoproxy>-->
<aop:config>
    <aop:pointcut id="p" expression="execution(* test.aop.User.add(..))"/>
    <aop:aspect ref="userProxy">
        <aop:before method="before" pointcut-ref="p"></aop:before>
    </aop:aspect>
</aop:config>
```



## JdbcTemplate

Spring 框架对 JDBC 进行封装，使用 JdbcTemplate 方便实现对数据库操作

JdbcTemplate 实现查询返回对象

![image-20201217144717885](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217144717885.png)

有三个参数 

 第一个参数：sql 语句 

 第二个参数：RowMapper 是接口，针对返回不同类型数据，使用这个接口里面实现类完成 数据封装 

 第三个参数：sql 语句值

## 事务一块说了

### 开启事务

```xml
<tx:annotation-driven transaction-manager="transactionManager"></tx:annotation-driven>
<bean id="transactionManager"
      class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
    <!--注入数据源-->
    <property name="dataSource" ref="dataSource"></property>
</bean>
```

### 声明式事务@Transactional

在 service 类上面（或者 service 类里面方法上面）添加事务注解

（1）@Transactional，这个注解添加到类上面，也可以添加方法上面 

（2）如果把这个注解添加类上面，这个类里面所有的方法都添加事务 

（3）如果把这个注解添加方法上面，为这个方法添加事务

#### 参数配置

![image-20201217151213549](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217151213549.png)



#### propagation：事务传播行为

![image-20201217151823707](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217151823707.png)

#### ioslation：事务隔离级别

![image-20201217151849904](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217151849904.png)

#### 其他

![image-20201217151934410](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201217151934410.png)

### XML配置

```xml
<tx:advice id="txadvice">
    <!--配置事务参数-->
    <tx:attributes>
        <!--指定哪种规则的方法上面添加事务-->
        <tx:method name="testTrans" propagation="REQUIRED"/>
    </tx:attributes>
</tx:advice>
<!--3 配置切入点和切面-->
<aop:config>
    <!--配置切入点-->
    <aop:pointcut id="pt" expression="execution(* test.MyTest.*(..))"/>
    <!--配置切面-->
    <aop:advisor advice-ref="txadvice" pointcut-ref="pt"/>
</aop:config>
```



## 事务 加 JdbcTemplate的例子

### XML文件配置

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-4.2.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context-4.2.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop-4.2.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx-4.2.xsd">
    <!--开启组件扫描
     1 如果扫描多个包，多个包使用逗号隔开
     2 扫描包上层目录
    -->
    <context:component-scan base-package="test"></context:component-scan>
    
    <context:property-placeholder location="classpath:jdbc.properties"></context:property-placeholder>
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"
          destroy-method="close">
        <property name="url" value="${mysql.url}" />
        <property name="username" value="root" />
        <property name="password" value="chentailiang.123" />
        <property name="driverClassName" value="com.mysql.jdbc.Driver" />
    </bean>

    <!-- JdbcTemplate 对象 -->
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <!--注入 dataSource-->
        <property name="dataSource" ref="dataSource"></property>
    </bean>

    <bean id="transactionManager"
          class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <!--注入数据源-->
        <property name="dataSource" ref="dataSource"></property>
    </bean>
    <tx:annotation-driven transaction-manager="transactionManager"></tx:annotation-driven>


    <tx:advice id="txadvice">
        <!--配置事务参数-->
        <tx:attributes>
            <!--指定哪种规则的方法上面添加事务-->
            <tx:method name="testTrans" propagation="REQUIRED"/>
        </tx:attributes>
    </tx:advice>
    <!--3 配置切入点和切面-->
    <aop:config>
        <!--配置切入点-->
        <aop:pointcut id="pt" expression="execution(* test.MyTest.*(..))"/>
        <!--配置切面-->
        <aop:advisor advice-ref="txadvice" pointcut-ref="pt"/>
    </aop:config>
</beans>
```

### bean对象

```java
@Data
public class User {
	Integer id;
	String username;
	String account;
}
```

### 测试类

```java
@Component("test")
public class MyTest {

	@Autowired
	JdbcTemplate jdbcTemplate;

	public void add(){
		String sql = "INSERT INTO `user`(username) VALUE(?)";
		Object[] args = {"哈哈哈"};
		int update = jdbcTemplate.update(sql, args);
	}

	public void query1(){
		String sql1 = "select count(*) from `user`";
		Integer count  = jdbcTemplate.queryForObject(sql1, Integer.class);
		System.out.println(count);
		String sql2 = "select * from `user` where id = 1";
		User user = jdbcTemplate.queryForObject(sql2, new
				BeanPropertyRowMapper<>(User.class) );
		System.out.println(user);
		String sql3 = "select * from `user`";
		List<User> users = jdbcTemplate.query(sql3, new
				BeanPropertyRowMapper<User>(User.class));
		System.out.println(users);
	}

	//批量操作
	public void batch(){
		String sql = "INSERT INTO `user`(username) VALUE(?)";
		int[] ints = jdbcTemplate.batchUpdate(sql,
				Arrays.asList(new Object[]{"小1"}, new Object[]{"小2"}));
	}

	//转账
	public void trans(int id, int money){
		String sql = "update `user` set account = account + ? where id = ?";
		jdbcTemplate.update(sql, new Object[]{money,id});
	}
    
	@Transactional
	public void testTrans() throws RuntimeException{
		//账户7向账户8转账100
		trans(7, -100);
		int i = 5 / 0;
		trans(8, 100);
	}


	@Test
	public void test(){
		ApplicationContext context = new ClassPathXmlApplicationContext("bean1.xml");
		MyTest test = context.getBean("test", MyTest.class);
//		add();
//		test.batch();
//		test.query1();
		try {
			test.testTrans();
		}catch (Exception e){}

	}

}
```

### 完全基于注解重写配置

```java
@Configuration //作为配置类，替代 xml 配置文件
@ComponentScan(basePackages = {"test"})
@EnableTransactionManagement
public class SpringConfig {
	//创建数据库链接池
	@Bean
	public DruidDataSource getDruidDataSource() {
		DruidDataSource dataSource = new DruidDataSource();
		dataSource.setDriverClassName("com.mysql.jdbc.Driver");
		dataSource.setUrl("jdbc:mysql://172.19.241.12:3306/user?useUnicode=true&characterEncoding=UTF-8&useSSL=false&serverTimezone=Asia/Shanghai");
		dataSource.setUsername("root");
		dataSource.setPassword("chentailiang.123");
		return dataSource;
	}

	//创建 JdbcTemplate 对象
	@Bean
	public JdbcTemplate getJdbcTemplate(DataSource dataSource) {
		//到 ioc 容器中根据类型找到 dataSource
		JdbcTemplate jdbcTemplate = new JdbcTemplate();
		//注入 dataSource
		jdbcTemplate.setDataSource(dataSource);
		return jdbcTemplate;
	}

	//创建事务管理器
	@Bean
	public DataSourceTransactionManager
	getDataSourceTransactionManager(DataSource dataSource) {
		DataSourceTransactionManager transactionManager = new
				DataSourceTransactionManager();
		transactionManager.setDataSource(dataSource);
		return transactionManager;
	}


	@Test
	public void test(){
		ApplicationContext context =  new AnnotationConfigApplicationContext(SpringConfig.class);
		MyTest test = context.getBean("test", MyTest.class);
		try {
			test.testTrans();
		}catch (Exception e){}

	}

}
```

## Spring 5新功能（新不了，下一个）

