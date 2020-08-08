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
   - Java虚拟机启动时被标明**启动**类的类。包含main()函数的类
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

#### 测试1：初始化一个类的**子类** ，**父类也要初始化**。被动使用。

```java
/**
        对于静态字段来说，只有直接定义了该字段的类才会被初始化
        当一个类在初始化时，要求父类全部都已经初始化完毕
        -XX:+TraceClassLoading，用于追踪类的加载信息并打印出来

        -XX:+<option>，表示开启option选项
        -XX:-<option>，表示关闭option选项
        -XX:<option>=value，表示将option的值设置为value
*/

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

#### 测试2。常量 final 会放到常量池，对常量的使用不是主动使用

常量在**编译**阶段会存入到**调用**这个常量的方法所在的**类的常量池**中。本质上，调用类并没有直接调用到定义常量的类，因此并不会触发定义常量的类的初始化

注意：这里指的是将常量存到**ClassLoader**的常量池中，之后**ClassLoader**和MyParent2就没有任何关系了。
甚至我们可以将MyParent2的class文件删除

```java
public class ClassLoader {
	public static void main(String[] args) {
		//输出 hello world
		System.out.println(MyParent2.str);
		// MyParent static block 输出 hello world
		System.out.println(MyParent2.str1);
	}
}
class MyParent2{
	public static final String str="hello world";
	public static String str1="hello world";
	static {
		System.out.println("MyParent static block");
	}
}
```

#### 测试3：常量的值并非编译期间可以确定的。

当一个**常量的值并非编译期间可以确定的**，那么其值就**不会**放到**调用类的常量池**中 ,这时在程序运行时，会导致**主动使用**这个常量所在的类，显然会导致这个类被初始化

```java
public class ClassLoader{
	public static void main(String[] args){
		//输出MyParent static block、kjqhdun-baoje21w-jxqioj1-2jwejc9029
		System.out.println(MyParent3.str);
	}
}
class MyParent3{
	public static final String str= UUID.randomUUID().toString();
	static {
		System.out.println("MyParent static block");
	}
}
```

#### 测试4：创建数组不属于主动使用

- 对于**数组实例**来说，其类型是由JVM在运行期动态生成的，表示为 MyParent4 这种形式。     
-   对于数组来说，JavaDoc经构成数据的元素成为Component，实际上是将数组降低一个维度后的类型。   
-  助记符：anewarray：表示创建一个引用类型（如类、接口）的数组，并将其引用值压入栈顶
-  助记符：newarray：表示创建一个指定原始类型（int boolean float double）的数组，并将其引用值压入栈顶                   

```java
public class ClassLoader{
	public static void main(String[] args){
		//创建类的实例，属于主动使用，会导致类的初始化
//		MyParent4 myParent4=new MyParent4();
		//不是主动使用
		MyParent4[] myParent4s=new MyParent4[1];
		//输出 class [L org.example.MyParent4;
		System.out.println(myParent4s.getClass());
		//输出Object
		System.out.println(myParent4s.getClass().getSuperclass());

		int[] i=new int[1];
		System.out.println(i.getClass());          //输出 [ I
		System.out.println(i.getClass().getSuperclass());    //输出Object
	}
}
class MyParent4{
	static {
		System.out.println("MyParent static block");
	}
}
```

#### 测试5：接口初始化

当一个接口在初始化时，并不要求其父接口都完成了初始化   ，只有在真正使用到父接口的时候（如引用接口中定义的常量），才会初始化

#### 测试6：准备阶段和初始化的顺序问题（初始化时按照静态定义的顺序，从上到下赋值）

```java
public class ClassLoader{
   public static void main(String[] args){
         Singleton Singleton= org.example.Singleton.getInstance();
         System.out.println( org.example.Singleton.counter1);     
         System.out.println( org.example.Singleton.counter2);
   }
}
class Singleton{
// static {
//    counter2 = 1;   // 1, 2
// }
   public static int counter1;
   public static int counter2=0;
   private static Singleton singleton=new Singleton();

   private Singleton(){
      counter1++;
      counter2++;
   }

   // public static int counter2=0;       //   若改变此赋值语句的位置，输出  1，0
   public static Singleton getInstance(){
      return singleton;
   }
}
```