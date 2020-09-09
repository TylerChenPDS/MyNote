Java虚拟机不和包括java在内的任何语言绑定，它只与“Class”特定的二进制文件格式关联，Class文件中包含Java虚拟机指令集和符号表以及若干其他辅助信息。

------

测试

```java
public class MyTest1 {
	int a = 1;

	public int getA() {
		return a;
	}

	public void setA(int a) {
		this.a = a;
	}
}
```

**执行命令  javap -c MyTest**

```java
Compiled from "MyTest1.java"
public class org.ex2.MyTest1 {
  int a;

  public org.ex2.MyTest1();
    Code:
       0: aload_0
       1: invokespecial #1                  // Method java/lang/Object."<init>":()V
       4: aload_0
       5: iconst_1
       6: putfield      #2                  // Field a:I
       9: return

  public int getA();
    Code:
       0: aload_0
       1: getfield      #2                  // Field a:I
       4: ireturn

  public void setA(int);
    Code:
       0: aload_0
       1: iload_1
       2: putfield      #2                  // Field a:I
       5: return
}

```

**使用反编译命令：javap -verbose MyTest，生成以下数据**

```java
Classfile /D:/日常文件/桌面文件/危险废物管理系统/lyc/study/target/classes/org/ex2/MyTest1.class
  Last modified 2020-9-9; size 461 bytes
  MD5 checksum 5d71b88b5608fce54d3459704080680f
  Compiled from "MyTest1.java"
public class org.ex2.MyTest1
  minor version: 0
  major version: 52
  flags: ACC_PUBLIC, ACC_SUPER
Constant pool: 
//
   #1 = Methodref          #4.#20         // java/lang/Object."<init>":()V
   #2 = Fieldref           #3.#21         // org/ex2/MyTest1.a:I
   #3 = Class              #22            // org/ex2/MyTest1
   #4 = Class              #23            // java/lang/Object
   #5 = Utf8               a
   #6 = Utf8               I
   #7 = Utf8               <init>
   #8 = Utf8               ()V
   #9 = Utf8               Code
  #10 = Utf8               LineNumberTable
  #11 = Utf8               LocalVariableTable
  #12 = Utf8               this
  #13 = Utf8               Lorg/ex2/MyTest1;
  #14 = Utf8               getA
  #15 = Utf8               ()I
  #16 = Utf8               setA
  #17 = Utf8               (I)V
  #18 = Utf8               SourceFile
  #19 = Utf8               MyTest1.java
  #20 = NameAndType        #7:#8          // "<init>":()V
  #21 = NameAndType        #5:#6          // a:I
  #22 = Utf8               org/ex2/MyTest1
  #23 = Utf8               java/lang/Object
{
  int a;
    descriptor: I
    flags:

  public org.ex2.MyTest1();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=2, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: aload_0
         5: iconst_1
         6: putfield      #2                  // Field a:I
         9: return
      LineNumberTable:
        line 7: 0
        line 8: 4
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      10     0  this   Lorg/ex2/MyTest1;

  public int getA();
    descriptor: ()I
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: getfield      #2                  // Field a:I
         4: ireturn
      LineNumberTable:
        line 11: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lorg/ex2/MyTest1;

  public void setA(int);
    descriptor: (I)V
    flags: ACC_PUBLIC
    Code:
      stack=2, locals=2, args_size=2
         0: aload_0
         1: iload_1
         2: putfield      #2                  // Field a:I
         5: return
      LineNumberTable:
        line 15: 0
        line 16: 5
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       6     0  this   Lorg/ex2/MyTest1;
            0       6     1     a   I
}
SourceFile: "MyTest1.java"
```

![](./img/2.png)

1. 使用javap -verbose MyTest 命令分析一个字节码文件时，将会分析该字节码文件的魔数，版本号，常量池，类信息，类的构造方法，类中的方法信息，类变量与成员变量的信息。
2. 魔数：所有的.class文件的前四个字节都是魔数，魔数值为固定值：0xCAFEBABE（咖啡宝贝）
3. 版本号：魔数后面4个字节是版本信息，前两个字节表示minor version（次版本号），后两个字节表示major version（主版本号），十六进制34=十进制52。所以该文件的版本号为1.8.0。低版本的编译器编译的字节码可以在高版本的JVM下运行，反过来则不行。
4. **常量池**（constant pool）：版本号之后的就是常量池入口，一个java类定义的很多信息都是由常量池来维护和描述的，可以将常量池看作是class文件的资源仓库，包括java类定义的方法和变量信息，常量池中主要存储两类常量：字面量和符号引用。字面量如文本字符串、java中生命的final常量值等，符号引用如类和接口的全局限定名，字段的名称和描述符，方法的名称和描述符等。
5. 常量池的整体结构：Java类对应的常量池主要由**常量池数量**和**常量池数组**两部分共同构成，常量池数量紧跟在主版本号后面，占据两个字节，而常量池数组在常量池数量之后。常量池数组与一般数组不同的是，常量池数组中元素的类型、结构都是不同的，长度当然也就不同，但是每一种元素的第一个数据都是一个u1类型标志位，占据一个字节，JVM在解析常量池时，就会根据这个u1类型的来获取对应的元素的具体类型。 **值得注意的是，常量池数组中元素的个数=常量池数-1**,（其中0暂时不使用）。目的是满足某些常量池索引值的数据在特定的情况下需要表达不引用任何常量池的含义。根本原因在于索引为0也是一个常量，它是JVM的保留常量，它不位于常量表中。这个常量就对应null，所以常量池的索引从1而非0开始。<img src="./img/3.png" style="zoom:80%;" />
6. 在JVM规范中，每个变量/字段都有描述信息，主要的作用是描述字段的数据类型，方法的参数列表（包括数量、类型和顺序）与返回值。根据描述符规则，基本数据类型和代表无返回值的void类型都用一个大写字符来表示，而对象类型使用字符L+对象的全限定名称来表示。为了压缩字节码文件的体积，对于基本数据类型，JVM都只使用一个大写字母来表示。如下所示:B-byte，C-char，D-double，F-float，I-int，J-long，S-short，Z-boolean，V-void，L-对象类型，如Ljava/lang/String;  对于数组类型来说，每一个维度使用一个前置的[ 来表示，如int[]表示为[I ，String[]:[] (二维数组)被记录为[[Ljava/lang/String;
7. 用描述符描述方法的时候，用先参数列表后返回值的方式来描述。参数列表按照参数的严格顺序放在一组（）之内，如方法String getNameByID(int id ,String name)
   (I,Ljava/lang/String;)Ljava/lang/String;

### 字节码结构

![](./img/4.png)

图二：

![](./img/6.png)

- Access Falgs：

- 访问标志信息包括了该class文件是类还是接口，是否被定义成public，是否是abstract，如果是类，是否被定义成final。

  ![](./img/5.png)0x0021是0x0020和0x0001的并集，表示ACC_PUBLIC和ACC_SUPER
  0x0002:private

- 字段表（Fields）：
  字段表用于描述类和接口中声明的变量。这里的字段包含了类级别变量和实例变量，但是不包括方法内部声明的局部变量。

  ![](./img/7.png)

- 方法表
  方法的属性结构：
  方法中的每个属性都是一个attribute_info结构:
  （1）JVM预定义了部分attribute，但是编译器自己也可以实现自己的attribute写入class文件里，供运行时使用；
  （2）不同的attribute通过attribute_name_index来区分。

- attribute_info格式:
  attribute_info{
  u2 attribute_name_index;
  u4 attribute_length;
  u1 info[attribute_length]
  }

- attribute_name_index值为code，则为Code结构
  Code的作用是保存该方法的结构，所对应的的字节码

  ![](./img/8.png)

