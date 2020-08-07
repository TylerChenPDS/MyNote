## JVM-类加载机制

### 类加载 class loading 概念

- 在java代码中，**类型**的加载、连接和初始化过程都是在程序运行期间完成的。（类从磁盘加载到内存中经历的三个阶段）
- 提供了更大的灵活性，增加了更多的可能性。

### **类的加载、连接与初始化**

1. 加载：查找并加载类的二进制数据到java虚拟机中
2. 连接：
   - 验证 : 确保被加载的类的正确性
   - 准备：为类的**静态变量**分配内存，并将其初始化为**默认值**，但是到达初始化之前类变量都没有初始化为真正的初始值
   - 解析：**把类中的符号引用转换为直接引用**，就是在类型的常量池中寻找类、接口、字段和方法的符号引用，把这些符号引用替换成直接引用的过程
3. 初始化：为类的静态变量赋予**正确（真实）**的初始值

#### java对类的使用

1. 主动使用

   所有的Java虚拟机实现必须在每个类或接口被Java程序“**首次主动使用**”时才能**初始化**他们

   主动使用（七种情况）

   - 创建类的实例 new Object()
   - 访问某个类或接口的静态变量。 getstatic（助记符），或者对该静态变量赋值 putstatic
   - 调用类的静态方法 invokestatic
   - 反射（Class.forName(“com.test.Test”)）
   - 初始化一个类的**子类** ，**父类也要初始化**
   - Java虚拟机启动时被标明启动类的类。包含main()函数的类
   - JDK1.7开始提供的动态语言支持（了解）

2. 被动使用

   ​	除了上面七种情况外，其他使用java类的方式都被看做是对类的**被动使用**，都不会导致类的**初始化**。但是有可能加载

#### 类的加载

​	类的加载指的是将类的.class文件中的二进制数据读入到内存中，将其放在运行时数据区的方法区内，然后在内存中创建一个java.lang.Class对象（规范并未说明Class对象位于哪里，HotSpot虚拟机将其放在**方法区**中）用来封装内在方法区内的数据结构。

##### 加载.calss文件的方式

- 从本地系统中直接加载
- 通过网络下载.class文件
- 从zip，jar等归档文件中加载.class文件
- 从专用数据库中提取.class文件
- **将java源文件动态编译为.class文件**，(动态代理 例如 jsp)

#### 测试1：

初始化一个类的**子类** ，**父类也要初始化**。

被动使用。

```java
public class ClassLoader {
	public static void main(String[] args) {
		//输出：MyParent static block 、 hello world   （因为对MyChild不是主动使用）
		System.out.println(MyChild.str);
		//输出：MyParent static block  、MyChild static block、welcome
		// 调用了 MyChild 的getstatic，子类初始化，所以父类也要初始化
		System.out.println(MyChild.str2);
	}
}
class MyParent{
	public static String str="hello world";
	static {
		System.out.println("MyParent static block");
	}
}
class MyChild extends MyParent{
	public static String str2="welcome";
	static {
		System.out.println("MyChild static block");
	}
}
```

