# 1，JVM 内存区域的划分

​		Java虚拟机在执行Java程序的过程中会把它所管理的内存划分为若干个不同的数据区域。这些区域有各自的用途和各自的生命周期，有的区域随着虚拟机进程的启动而一直存在，有些区域则是依赖用户线程的启动和结束而建立和销毁。

![](./img/6.png)

## 1.1 **程序计数器**

程序计数器(Program Counter Register)是一块较小的内存空间，它可以看作是当前线程所执行的字节码的行号指示器。字节码解释器工作时就是通过改变这个计数器的值来选取下一条需要执行的字节码指令。它是程序控制流的指示器，分支、循环、跳转、异常处理、**线程恢复**等基础功能都需要依赖这个计数器来完成。**程序计数器在JVM中占用的空间很少，但作用很大。也是JVM规范里唯一规定没有任何OutOfMemoryError情况的区域。**

## 1.2 Java虚拟机栈

​    与程序计数器一样，Java虚拟机栈(Java Virtual Machine Stack)也是线程隔离的，它的生命周期与线程相同。虚拟机栈描述的是**Java方法执行的线程内存模型**。

### 1.2.1 栈帧

​    **每个方法被执行的时候，Java虚拟机都会同步创建一个栈帧(Stack Frame)。栈帧**是一个用于存储**局部变量表、操作数栈、动态连接、方法出口**等信息是一个数据结构。**每一个方法被调用直至执行完毕的过程，就对应着一个栈帧在虚拟机栈中从入栈到出栈的过程**。

一般情况下，我们笼统地认为我们定义的局部变量，如String s = new String("内容在堆里，s变量在栈里");实质上，更具体讲，**s是存放在执行s所在的方法的某条具体的线程对应的JVM栈里的某个栈帧里的局部变量表里！**

### 1.2.2 局部变量

**局部变量表存放了：**

- 编译期可知的各种Java虚拟机基本数据类型(boolean、byte、 char、 short、 int、 float、 long、 double) 。

- 对象引用：它并不等同于对象本身，可能是一个指向对象起始地址的引用指针，也可能是指向一个代表对象的句柄或者其他与此对象相关的位置。也可以理解为是一个地址。

- returnAddress类型 (指向了一条字节码指令的地址)。

**局部变量表空间是用一个叫做局部变量槽（slot）来表示的。**

​		其中64位长度的long和double类型的数据会占用两个变量槽，其余的数据类型只占用一个。**局部变量表所需的内存空间在编译期间完成分配，当进入一个方法时，这个方法需要在栈帧中分配多大的局部变量空间是完全确定的，在方法运行期间不会改变局部变量表的大小。**

**【注意】**，这里说的”大小” 是指变量槽的数量，虚拟机真正使用多大的内存空间，譬如按照1个变量槽占用32个比特、64个比特，或者更多，是完全由具体的虚拟机实现自行决定的事情。

**在《Java虚拟机规范》中，对这个内存区域规定了两类异常状况:**

1、如果线程请求的栈深度大于虚拟机所允许的深度，将抛出StackOverflowError异常;

2、如果Java虚拟机栈容量可以动态扩展,当栈扩展时无法申请到足够的内存会抛出OutOfMemoryError异常。

**【注意】**HotSpot虚拟机的栈容量是不可以动态扩展的，以前的Classic虚拟机倒是可以。所以在HotSpot虚拟机上是不会由于虚拟机栈无法扩展而导致OutOfMemoryError异常，只要线程申请栈空间成功了就不会有OOM,但是如果申请时就失败，才会出现OOM异常。

## 1.3 本地方法栈

​		本地方法栈(Native Method Stacks)与虚拟机栈所发挥的作用是非常相似的，其区别只是虚拟机栈为虚拟机执行Java方法(也就是字节码)服务，而本地方法栈则是为虚拟机使用到的本地(Native)方法服务。

​		**【注意】**有的Java虚拟机(譬如HotSpot虚拟机) 直接就把本地方法栈和虚拟机栈合二为一。**HotSpot虚拟机是没本地方法栈，**对应的以后学习配置的时候：虽然-Xoss参数是设置本地方法栈大小存在，但是是没用的，栈的容量大小只有-Xss 参数设置。与虚拟机栈一样，本地方法栈也会在栈深度溢出或者栈扩展失败时分别抛出StackOverflowError和OutOfMemoryError异常。

## 1.4 Java堆

​		对于Java应用程序来说，Java堆(Java Heap)是虛拟机所管理的内存中最大的一块。Java堆是被所有线程共享的一块内存区域，在虚拟机启动时创建。此内存区域的唯一目的就是存放**对象实例**， Java世界里**“几乎”** 所有的对象实例都在这里分配内存。

​		在《Java虚拟机规范》中对Java堆的描述是:”所有的对象实例以及数组都应当在堆上分配”。**Java堆是垃圾收集器管理的内存区域，因此一些资料中它也被称作“GC堆”**。从回收内存的角度看，由于现代垃圾收集器大部分都是基于分代收集理论设计的

​		**【注意】：”新生代””老年代””永久代”“Eden空间””From Survivor空间”"To Survivor空间”**，这些堆区域的进一步划分的叫法，仅仅是**一部分垃圾收集器**的共同特性或者说设计风格而已，而非某个Java虚拟机具体实现的**固有内存布局**，更不是《Java虚拟机规范》里对Java堆的进一步细致划分。

​		Java堆既可以被实现成固定大小的，也可以是可扩展的，不过当前主流的Java虚拟机都是**按照可扩展来实现**的(通过参数-Xmx和-Xms设定其空间的大小)。如果在Java堆中没有内存去给对象实例分配，并且堆也无法再扩展时，Java虚拟机将会抛出OutOfMemoryError！

## 1.5 方法区(Method Area)

​		方法区(Method Area)与Java堆一样，是各个线程共享的内存区域，虽然《Java虚拟机规范》中把方法区描述为堆的一个逻辑部分，但是它却有一个别名叫作**“非堆”(Non-Heap)**，目的是与Java堆区分开来。它用于存储已**被虚拟机加载的类型信息、常量、静态变量、即时编译器编译后的代码缓存**等数据。

### 1.5.1 **永久代**

​		在JDK 8以前，许多Java程序员都习惯在HotSpot虚拟机上开发、部署程序，很多人都更愿意把**方法区称呼为”永久代”(PermanentGeneration)**，或将两者混为一谈。原因是当时的HotSpot虚拟机设计团队选择把**垃圾收集器的分代设计扩展至方法区**，或者说**使用永久代来实现方法区**，这样使得HotSpot的垃圾收集器能够像管理Java堆一样管理这部分内存，省去专门为方法区编写内存管理代码的工作。**但是对于其他虚拟机实现，譬如JRockit、J9来说，是不存在永久代的概念的。**

​		JDK1.7中，符号引用(Symbols)转移到了native heap；字符串常量池(interned strings)转移到了java heap；类的静态变量(class statics)转移到了java heap。

​		但现在回头来看，当年使用永久代来实现方法区的决定并不是一个好主意,
​		1、这种设计导致了Java应用更容易遇到内存溢出的问题和性能问题。
​		2、类及方法的信息等比较难预估其大小，永久代要求配置参数-XX: MaxPermSize，即使不设置也有默认大小。因此对于永久代的大小指定比较困难，太小容易出现永久代溢出，太大则容易导致老年代溢出。
​		3、永久代会为 GC 带来不必要的复杂度，并且回收效率偏低。	

​		到了JDK8,完全废弃了永久代的概念，改用与JRockit、J9一样在**本地内存**中实现的元空间(Meta-space) 来代替了。这样就没有参数-XX: MaxPermSize的大小限制，减少了OOM的机会。当然也不是无限大哈，受到本机总内存(包括物理内存、SWAP分区或者分页文件)大小以及处理器寻址空间的限制，**当方法区满足不了程序的空间需求，OOM仍然会出现的。**

## 1.6 **运行时常量池**

### 1.6.1 静态常量池

​		静态常量池：即*.class文件中的常量池，class文件中的常量池不仅仅包含字符串/数字这些**字面量**，还包含类、方法的信息，占用class文件绝大部分空间。这种常量池主要用于存放两大类常量：字面量和符号引用量。

- 字面量相当于Java语言层面常量的概念，如文本字符串，声明为final的常量值等
- 符号引用则属于编译原理方面的概念，包括了如下三种类型的常量：类和接口的全限定名、字段名称描述符、方法名称描述符

### 1.6.2 运行时常量池

​		运行时常量池：**虚拟机会将在类加载后把各个class文件中的常量池载入到运行时常量池中，**前面的静态常量池只是一个**静态文件结构**，运行时常量池是**方法区的一部分**，是**一块内存区域**。运行时常量池可以在运行期间将符号引用解析为直接引用，即把那些描述符（名字）替换为能直接定位到字段、方法的引用或句柄（地址）。

### 1.6.3 字符串常量池

​		字符串常量池 ：字符串常量池可以理解为运行时常量池分出来的部分。加载时，对于Class的静态常量池，如果字符串会被装到字符串常量池中。字符串池是 JVM 层面的技术。

​		在 JDK 1.6 以及以前的版本中，字符串池是放在 Perm 区(Permanent Generation，永久代)。

​		在 JDK 1.7 的版本中，**字符串常量池**移到Java Heap。

​		在 JDK 1.8 的版本中在永久代移除后，**字符串常量池和**运行时常量池也没有放到新的方法区---元空间里，而是留在了Java堆里。元空间里只存储**类和类加载器的元数据信息**了！

​	运行期间也可以将新的常量放入池中，这种特性被开发人员利用得比较多的便是**String类的intern()方法**。
这个方法将对应的String引用的字符串放入字符串常量池，并返回常量池中对应的地址。

​	测试代码：

```java
String str1 = new String("1")+ new String("1");      
System.out.println(str1.intern() == str1);   
System.out.println(str1 ==  "11");  
//再jdk1.7及以后输出结果为 true, true
//1.6输出结果为false,fasle
```

```java
String str2 = "11";//新加的一行代码，其余不变  
String str1 = new String("1")+ new String("1");      
System.out.println(str1.intern() == str1);   
System.out.println(str1 ==  "11");  
//运行结果为false, false
```

第一种情况： String str1 = **new** String("1")+ **new** String("1"); 
	这行代码在字符串常量池中生成“1”，并在堆空间中生成str1 引用指向的对象（内容为"11"）。注意此时常量池中是没有 “11”对象的。
	str1.intern()。这一行代码，是将 str1中的“11”字符串放入 String 常量池中，此时常量池中不存在“11”字符串。JDK8的常量池中不会再重复新创建一份对象了（因为jdk8中的字符串常量池也在堆中），**直接存储堆中的引用就是str1，常量池中放的是引用。**
	System.out.println(str1.intern() == str1);  返回true。 
	str1 == "11" 这一行代码会直接去常量池中创建 "11" ，但是发现已经有这个对象了，还是维持原来的引用。     	System.out.println(str1 == "11"); 返回true。 

**第二种情况：** str2先在常量池中创建了“11”，**常量池中放的是“11”这个字符串对象本身。**

​	那么str1.intern()，想将 str1中的“11”字符串放入 String 常量池中，但是发现已经有这个对象了，常量池中不需要再重复创建一份对象了，当然就直接指向了str2。
​	System.out.println(str1.intern() == str1);  返回false。
​	str1 == "11" 
​    这一行代码会直接去常量池中创建 "11" ，但是发现已经有这个对象了，还是维持原来的引用str2。

## 1.7 直接内存

​		直接内存(Direct Memory)并不是虚拟机运行时数据区的一部分，也不是《Java虚拟机规范》中定义的内存区域。但是这部分内存也被频繁地使用，而且也可能导致OutOfMemoryError异常出现。在JDK 1.4中新加入了NIO类，引入了一种基于通道(Channel) 与缓冲区(Buffer) 的I/0方式，它可以使用Native函数库直**接分配堆外内存**。

​		本机**直接内存**的分配不会受到Java堆大小的限制。

# 2 创建对象

## 2.1 分配内存空间

- 当Java虚拟机遇到一条字节码new指令时，首先将去检查这个指令的参数是否能在常量池中定位到一个类的符号引用，并且检查这个符号引用代表的类是否已被加载、解析和初始化过。如果没有，那必须先执行相应的类**加载过程**
- 在类加载检查通过后，接下来虚拟机将为新生对象分配内存。**对象所需内存的大小在类加载完成后便可完全确定**，为对象分配空间的任务实际上便等同于把一块确定大小的内存块从Java堆中划分出来。
  - **指针碰撞**：假设Java堆中内存是绝对规整的,所有被使用过的内存都被放在一边，空闲的内存被放在另一边，中间放着一个指针作为分界点的指示器，那所分配内存就仅仅是把那个指针向空闲空间方向挪动一段与对象大小相等的距离，这种分配方式称为**”指针碰撞”**(Bump The Pointer) 。
  - **空闲列表：**但如果Java堆中的内存并不是规整的，已被使用的内存和空闲的内存相互交错在一起，那就没有办法简单地进行指针碰撞了，虚拟机就必须维护一个列表，记录上哪些内存块是可用的，在分配的时候从列表中找到一块足够大的空间划分给对象实例，并更新列表上的记录，这种分配方式称为**"空闲列表”**(Free List)。
  - **Java堆是否规整又由所采用的垃圾收集器是否带有空间压缩整理(Compact)的能力决定。**因此，当使用Serial、 ParNew等带压缩整理过程的收集器时，系统采用的分配算法是指针碰撞，既简单又高效; 而当使用CMS这种基于清除(Sweep) 算法的收集器时，**理论上**就只能采用较为复杂的空闲列表来分配内存。
- **分可用空间的时候，要考虑线程安全问题:**对象创建在虚拟机中是非常频繁的行为，即使仅仅修改一个指针所指向的位置，**在并发情况下也并不是线程安全的**，可能出现正在给对象A分配内存，指针还没来得及修改，对象B又同时使用了原来的指针来分配内存的情况。
  - 一种是对分配内存空间的动作进行同步处理一实际上虚拟机是采用CAS配上失败重试的方式保证更新操作的原子性;
  - 另外一种是把内存分配的动作按照线程划分在不同的空间之中进行，即每个线程在Java堆中预先分配一小块内存，称为**本地线程分配缓冲**(Thread Local Allocation Buffer,TLAB)，哪个线程要分配内存，就在哪个线程的本地缓冲区中分配，只有本地缓冲区用完了，分配新的缓存区时才需要同步锁定。虚拟机是否使用TLAB，**可以通过-XX: +/-UseTLAB参数来设定。**

## 2.2 对象初始化

- 内存分配完成之后，虚拟机必须将分配到的内存空间(但不包括对象头)都初始化为零值，如果使用了TLAB的话，这一项工作也可以提前至TLAB分配时顺便进行。这步操作保证了对象的实例字段在Java代码中可以不赋初始值就直接使用，**使程序能访问到这些字段的数据类型所对应的零值。**

- 接下来，初始化对象的对象头(Object Header)把对象是哪个类的实例、如何才能找到类的元数据信息、对象的哈希码、对象的GC分代年龄等信息放到对象头里。根据虚拟机当前运行状态的不同，对象头的设置方式也会不同。

- 在上面工作都完成之后，从虚拟机的视角来看，一个新的对象已经产生了。但是从Java程序的视角看来，对象的始构造函数，即Class文件中的()方法还没有执行，

- **执行构造函数。**一般来说，new指令之后会接着执行()方法，按照程序员的意愿对对象进行初始化，

  这样一个真正可用的对象才算完全被构造出来。

## 2.3 **对象的内存布局**

在HotSpot虚拟机里，对象在堆内存中的存储可以划分为三个部分:

- 对象头(Header) 
- 实例数据(Instance Data)
- 对齐填充(Padding)

**HotSpot虚拟机对象的对象头部分包括两部分信息。**

- **是用于存储对象自身的运行时数据：**如哈希码(HashCode) 、GC分代年龄、锁状态标志、线程持有的锁、偏向线程ID、偏向时间戳等,这部分数据的长度在32位和64位的虚拟机中分别为32个比特和64个比特，官方称它为"Mark Word"。Mark Word被设计成一个有着动态定义的数据结构，以便在极小的空间内存储尽量多的数据，
- **类型指针**即对象指向它的类型元数据的指针，Java虚拟机通过这个指针来确定该对象是哪个类的实例。并不是所有的虚拟机实现都必须在对象数据上保留类型指针。此外，如果对象是一个Java数组，那在对象头中还必须有一块用于记录数组长度的数据，因为虚拟机可以通过普通Java对象的元数据信息确定Java对象的大小，但是如果数组的长度是不确定的，将无法通过元数据中的信息推断出数组的大小。

**HotSpot虚拟机对象的实例数据部分**

​		接下来实例数据部分是对象真正存储的有效信息，即我们在程序代码里面所定义的各种类型的字段内容，无论是从父类继承下来的，还是在子类中定义的字段都必须记录起来。

​			这部分的存储顺序会受到虚拟机**分配策略参数(-XX: FieldsAllocationStyle)** 和**字段在Java源码中定义顺序**的影响。

**HotSpot虚拟机默认的分配顺序为:**

longs/doubles、 ints、 shorts/chars. bytes/booleans、oops (OrdinaryObject Pointers, OOPs) ，

​		从以上默认的分配策略中可以看到，相同宽度的字段总是被分配到一起存放，在满足这个前提条件的情况下，在父类中定义的变量会出现在子类之前。如果HotSpot虛拟机的+XX: CompactFields参数值为true (默认就为true)，那子类之中较窄的变量也允许插入父类变量的空隙之中，以节省出一点点空间。

**HotSpot虚拟机对象的对齐填充部分**

​		对象的第三部分是对齐填充，**这并不是必然存在的，也没有特别的含义**，它仅仅起着占位符的作用。由于HotSpot虛拟机的自动内存管理系统要求对象**起始地址必须是8字节的整数倍**，换句话说就是任何对象的大小都必须是8字节的整数倍。对象头部分已经被精心设计成正好是8字节的倍数(1倍或者2倍)，因此，如果对象实例数据部分没有对齐的话，就需要通过对齐填充来补全。

## 2.4 **对象的访问定位**

​		对象访问方式也是由虚拟机实现而定的，主流的访问方式主要有**使用句柄**和**直接指针**两种:

1. 、如果使用句柄访问的话，Java堆中将可能会**划分出一块内存来作为句柄池**，reference中存储的就是对象的句柄地址， 而包柄中包含了对象实例数据与类型数据各自具体的地址信息。

   ![](./img/7.png)

2. 如果使用直接指针访问的话，Java堆中对象的内存布局就必须考虑如何放置访问类型数据的相关信息，reference中存储的直接就是对象地址，如果只是访问对象本身的话，就不需要多一次间接访问的开销，

   ![](./img/8.png)

**总结:**

- 使用直接指针来访问最大的好处就是速度更快，它节省了一次指针定位的时间开销，

- 由于对象访问在Java中非常频繁，因此这类开销积少成多也是一项极为可观的执行成本，

- **主流虚拟机HotSpot主要使用第二种方式进行对象访问(有例外情况，后面章节有介绍到)，**

- 但从整个软件开发的范围来看，在各种语言、框架中使用句柄来访问的情况也十分常见。

# 3 OOM实战

## 3.1  堆溢出

**配置参数**：

-Xms参数 ：设置堆的最小值

-Xmx参数：设置对的最大值

-XX: +HeapDumpOnOutOfMemoryError：可以让虚拟机在出现内存溢出异常的时候Dump出当前的内存堆转储快照以便进行事后分析

-XX:HeapDumpPath=${目录}参数:表示生成DUMP文件的路径，也可以指定文件名称，
例如：-XX:HeapDumpPath=${目录}/java_heapdump.hprof。

```java
//-Xms20m -Xmx20m -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=D:\heap_dump.hprof
public class EX1 {
	static class Test{
	}
	public static void main(String[] args) {
		//集合里面的对象不会被认为是垃圾，
		Set<Test> set = new HashSet<>();
		while (true){
			set.add(new Test());
		}
	}
}
```

![](./img/9.png)

官方对此的定义：超过98%的时间用来做GC并且回收了不到2%的堆内存时会抛出此异常。
JVM加一个参数：-XX:-UseGCOverheadLimit 禁用这个检查，进一步就是 java.lang.OutOfMemoryError: Java heap space。

## 3.2 **栈溢出**

​		由于HotSpot虚拟机中并不区分虚拟机栈和本地方法栈，因此对于HotSpot来说，-Xoss参数(设置本地方法栈大小)虽然存在，但实际上是没有任何效果的，栈容量只能由**-Xss**参数来设定。

关于虚拟机栈和本地方法栈，在《Java虚拟机规范》中描述了两种异常: 
   1) 如果线程请求的栈深度大于虚拟机所允许的最大深度,将抛出StackOverflowError异常。
   2) 如果虚拟机的栈内存允许动态扩展，当扩展栈容量无法申请到足够的内存时，将抛出OutOfMemoryError异常。

​		**而HotSpot虚 拟机的选择是不支持扩展，**所以除非在**创建线程申请内存时**就因无法获得足够内存而出现OutOfMemoryError异常，否则在线程运行时是不会因为扩展而导致内存溢出的，只会因为**栈容量无法容纳新的栈帧**而导致StackOverflowError异常。

```java
//-Xss128k
public class JVMDemo2 {
    private int stackLength = 1;

    public void stackLeak() {
        stackLength++;
        stackLeak();
    }
    public static void main(String[] args) {
        JVMDemo2 oom = new JVMDemo2();
        try {
            oom.stackLeak();
        } catch (Throwable e) {
            System.out.println("stack length:" + oom.stackLength);
            throw e;
        }
    }
}
```

## 3.3**方法区和运行时常量池溢出**

​	在JDK 6或更早之前的HotSpot虚拟机中，String常量池都是分配在永久代中，我们可以通过**-XX:PermSize和-XX: MaxPermSize**限制永久代的大小，即可间接限制其中String常量池的容量。

```java
//-XX:PermSize=6M -XX:MaxPermSize=6M
public class Demo {
    public static void main(String[] args) {
        // 使用Set保持着常量池引用，避免Full GC回收常量池行为
        Set<String> set = new HashSet<String>();
        // 在short范围内足以让3MB的PermSize产生OOM了
        int i = 0;
        while (true) {
            System.out.println(i);
            set.add(String.valueOf(i++).intern());
        }
    }
}
```

![](./img/10.png)

而使用JDK 7或更高版本的JDK来运行这段程序并不会得到相同的结果。

​		无论是在JDK 7中继续使用-XX: MaxPermSize参数，或者在JDK 8及以上版本使用**-XX:MetaspaceSize=N -XX:MaxMetaspaceSize=N**参数把方法区容量限制在3MB，也都不会重现JDK 6中的溢出异常，一般情况下，循环将一直进行下去，不停歇。出现这种变化，是因为自JDK 7起，原本存放在永久代的字符串常量池被移至Java堆之中,所以在JDK 7及以上版本，这时候使用**-Xms -Xmx参数**限制最大堆到3MB就能够看到以下两种运行结果之一，具体取决于哪里的对象分配时产生了溢出。

​		方法区的主要职责是用于存放类型的相关信息，如类名、访问修饰符、常量池、字段描述、方法描述等。这里我们借助个一个第三方框架工具CGLib直接操作字节码运行时生成大量的动态类。

cglib介绍：

- CGLIB（Code Generator Library）是一个强大的、高性能的代码生成库。其被广泛应用于AOP框架（如Spring）中，用以提供方法拦截操作。Hibernate作为一个比较受欢迎的ORM框架，同样使用CGLIB来代理多对一和一对一关联的一这端。

- **实质上CGLIB底层使用了ASM**（一个短小精悍的字节码操作框架）来操作字节码生成新的类。除了CGLIB库外，脚本语言（如Groovy何BeanShell）也使用ASM生成字节码。直接用ASM也可以，但不鼓励直接使用ASM，因为它需要对Java字节码的格式足够的了解。

- ![](./img/11.png)

- **Enhancer**

  Enhancer可能是CGLIB中最常用的一个类，

  Enhancer创建一个被代理对象的子类并且拦截所有的方法调用（包括从Object中继承的toString和hashCode方法）。

  但是，Enhancer不能够拦截final修饰的方法，这是由于Java final方法语义决定的。

  基于同样的道理，Enhancer也不能对fianl类进行代理操作。 

```xml
<!-- https://mvnrepository.com/artifact/cglib/cglib-nodep -->
        <dependency>
            <groupId>cglib</groupId>
            <artifactId>cglib-nodep</artifactId>
            <version>3.3.0</version>
            <scope>test</scope>
        </dependency>
```

```java
public class SampleClass {
    public String test(String input) {
        return "hello world";
    }
}

public class Demo {
    public static void main(String[] args) {
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(SampleClass.class);
        //不改变SampleClass源码的基础上，改变里面的test方法的返回值
        enhancer.setCallback(new FixedValue() {
            @Override
            public Object loadObject() throws Exception {
                return "hell cglib";
            }
        });
        //动态创建代理类，可以看做是sampleClass的子类
        SampleClass sampleClass = (SampleClass) enhancer.create();
        System.out.println(sampleClass.test(null));
    }
}
```

```java
//也可以用MethodInterceptor来实现对方法的拦截
public class SampleClass {
    public void test() {
        System.out.println("hello world");
    }
}

public class Demo {
    public static void main(final String[] args) {
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(SampleClass.class);
        //不改变SampleClass源码的基础上，在test方法前后添加一些代码
        enhancer.setCallback(new MethodInterceptor() {
            @Override
            public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
                System.out.println("test方法执行前。。。。。");
                Object rs = methodProxy.invokeSuper(o,args); //执行test方法
                System.out.println("test方法执行后.....");
                return rs;
            };
        });
        //动态创建代理类，可以看做是sampleClass的子类
        SampleClass sampleClass = (SampleClass) enhancer.create();
        sampleClass.test();
    }
}
```

**方法区内存溢出异常测试元空间**

```java
// -XX:MetaspaceSize=10m -XX:MaxMetaspaceSize=10m
public class Demo {
    public static void main(String[] args) {
        while (true) {
            Enhancer enhancer = new Enhancer();
            enhancer.setSuperclass(OOMObject.class);
            enhancer.setUseCache(false);
            enhancer.setCallback(new MethodInterceptor() {
                public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {
                    return proxy.invokeSuper(obj, args);
                }
            });
            enhancer.create();
        }
    }
    static class OOMObject {
    }
}
```

- -XX: MaxMetaspaceSize: 设置元空间最大值，默认是-1,即不限制，或者说只受限于本地内存大小。
- -XX: MetaspaceSize: 指定元空间的初始空间大小，以字节为单位，达到该值就会触发垃圾收集进行类型卸载，同时收集器会对该值进行调整:如果释放了大量的空间，就适当降低该值;如果释放了很少的空间，那么在不超过-XX: MaxMetaspaceSize (如果设置了的话)的情况下，适当提高该值。
- -XX: MinMetaspaceFreeRatio: 作用是在垃圾收集之后控制最小的元空间剩余容量的百分比，可减少因为元空间不足导致的垃圾收集的频率。

## 3.4 直接内存溢出

​		直接内存(Direct Memory)的容量大小可通过-XX:MaxDirectMemorySize参数来指定，如果不去指定，则默认与Java堆最大值(由-Xmx指定)一致，越过了DirectByteBuffer类直接通过**反射**获取Unsafe实例进行内存分配。

​		Java和C++语言的一个重要区别就是Java中我们无法直接操作一块内存区域，不能像C++中那样可以自己申请内存和释放内存。Java中的Unsafe类为我们提供了类似C++手动管理内存的能力。Unsafe类，全限定名是sun.misc.Unsafe，从名字中我们可以看出来这个类对普通程序员来说是“危险”的，一般应用开发者不会用到这个类。

​		因为虽然使用DirectByteBuffer分配内存也会抛出内存溢出异常，但它抛出异常时并没有真正向操作系统申请分配内存，而是通过计算得知内存无法分配就会在代码里手动抛出溢出异常，真正申请分配内存的方法是Unsafe:allocateMemory()。

```java
//-Xmx20M -XX:MaxDirectMemorySize=10M
public class Demo {
    private static final int _1MB = 1024 * 1024;
    public static void main(String[] args) throws Exception {
        //获得某个类的所有声明的字段，即包括public、private和proteced，但是不包括父类的申明字段。
        Field unsafeField = Unsafe.class.getDeclaredFields()[0];
        unsafeField.setAccessible(true);//设置可以获取此类的私有成员变量的value
        Unsafe unsafe = (Unsafe) unsafeField.get(null);
        while (true) {
            unsafe.allocateMemory(_1MB);
        }
    }
}
```

