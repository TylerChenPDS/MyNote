## 语言细节

### 按值传递和按引用传递的区别

官方的说法是在**java中只有按值传递，并没有所谓的按引用传递**。java数据类型可以分为两大类：基本类型（primitive types）和引用类型（reference types）。**引用类型的按值传递，传递的是对象的地址**

![image-20210124151337933](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210124151337933.png)



### 面向对象的三大特性的理解

**封装**给对象提供了隐藏内部特性和行为的能力。 

**多态**性是允许你将父对象设置成为一个或更多的他的子对象相等的技术，赋值之后，父对象就可以根据当前赋值给它的子对象的特性以不同的方式运作。父对象就可以根据实际类型来表先不同的行为。

**继承**提供了代码的重用性，也可以在不修改类的情况下给现存的类添加新特性。 

### JAVA异常

在 Java 中，所有的异常都有一个共同的祖先 `java.lang` 包中的 `Throwable` 类。`Throwable` 类有两个重要的子类 `Exception`（异常）和 `Error`（错误）。`Exception` 能被程序本身处理(`try-catch`)， `Error` 是无法处理的(只能尽量避免)。

### Java 序列化中如果有些字段不想进行序列化，怎么办？

对于不想进行序列化的变量，使用 transient 关键字修饰。

transient 关键字的作用是：阻止实例中那些用此关键字修饰的的变量序列化；当对象被反序列化时，被 transient 修饰的变量值不会被持久化和恢复。transient 只能修饰变量，不能修饰类和方法。

### 深拷贝VS浅拷贝

1. **浅拷贝**：对基本数据类型进行值传递，对引用数据类型进行引用传递般的拷贝，此为浅拷贝。
2. **深拷贝**：对基本数据类型进行值传递，对引用数据类型，创建一个新的对象，并复制其内容，此为深拷贝。



### == 和 equals 的区别

==：对于基本类型，它比较的是值，对于引用类型，它比较的是内存地址。

equals()的作用也是比较两个对象是否相等，但它一般有2种使用情况：

- 类如果没有覆盖equals()方法，则通过equals() 比较的的时候等价于“==”
- 第二种情况是我们可以自定义equals()的内容，判断两个对象的内容是否相等，例如对于People对象来说，如果其身份证号码相等我们就认为它是相等的。



### 类的 hashcode 是什么

**1)hashCode()介绍:**

`hashCode()` 的作用是获取哈希码，也称为散列码；它实际上是返回一个 int 整数。这个哈希码的作用是确定该对象在哈希表中的索引位置。`hashCode()`定义在 JDK 的 `Object` 类中，这就意味着 Java 中的任何类都包含有 `hashCode()` 函数。另外需要注意的是： `Object` 的 hashcode 方法是本地方法，也就是用 c 语言或 c++ 实现的，该方法通常用来将对象的内存地址 转换为整数之后返回。

**2)为什么要有 hashCode？**

**因为在使用散列的时候，我们可以快速定位对象的位置**。当你把对象加入 `HashSet` 时，`HashSet` 会先计算对象的 hashcode 值来判断对象加入的位置，同时也会与其他已经加入的对象的 hashcode 值作比较，如果没有相符的 hashcode，`HashSet` 会假设对象没有重复出现。但是如果发现有相同 hashcode 值的对象，这时会调用 `equals()` 方法来检查 hashcode 相等的对象是否真的相同。如果两者相同，`HashSet` 就不会让其加入操作成功。如果不同的话，就会重新散列到其他位置。这样我们就大大减少了 equals 的次数，相应就大大提高了执行速度。



### 为什么重写 `equals` 时必须重写 `hashCode` 方法？

如果两个对象相等，则 hashcode 一定也是相同的。两个对象相等,对两个对象分别调用 equals 方法都返回 true。**但是，两个对象有相同的 hashcode 值，它们也不一定是相等的。因此，equals 方法被覆盖过，则 `hashCode` 方法也必须被覆盖。**否则，两个equals相等的对象的hashcode也会不一样（因为Object 里面的hashCode 使用的是native方法，是根据内存地址转换成正数得来的），这样在使用某些集合框架的时候可能会造成错误。



### 什么是泛型？

**Java泛型设计原则：只要在编译时期没有出现警告，那么运行时期就不会出现ClassCastException异常**.

泛型：**把类型明确的工作推迟到创建对象或调用方法的时候才去明确的特殊的类型**

- `ArrayList<E>`中的**E称为类型参数变量**
- `ArrayList<Integer>`中的**Integer称为实际类型参数**

没有泛型之前，集合框架是使用Object实现的，在集合种读取到的每一个对象都需要进行强制类型转化，如果不小心插入了类型错误的对象，只有在运行的时候才会发现错误。有了泛型之后，可以告诉编译器，每个集合种接收那些对象类型，并再编译时告知是否插入了错误的对象。



### c++ 和 java 的区别

- 都是面向对象的语言，都支持封装、继承和多态
- Java 不提供指针来直接访问内存，程序内存更加安全
- Java 的类是单继承的，C++ 支持多重继承；虽然 Java 的类不可以多继承，但是接口可以多继承。
- Java 有自动内存管理机制，不需要程序员手动释放无用内存
- **在 C 语言中，字符串或字符数组最后都会有一个额外的字符‘\0’来表示结束。但是，Java 语言中没有结束符这一概念。**因为java中String是个对象，有长度的概念，已经不需要'\0'表示结尾

### 重载和重写的区别

**重载：**

发生在同一个类中，方法名必须相同，参数类型不同、个数不同、顺序不同，方法返回值和访问修饰符可以不同。

**重写：**

重写发生在运行期，是子类对父类的允许访问的方法的实现过程进行重新编写。

1. 返回值类型、方法名、参数列表必须相同，抛出的异常范围小于等于父类，访问修饰符范围大于等于父类。
2. 如果父类方法访问修饰符为 `private/final/static` 则子类就不能重写该方法，但是被 static 修饰的方法能够被再次声明。
3. 构造方法无法被重写



### 为什么String是不可变的？

**可变性**

简单的来说：String 类中使用 final 关键字修饰字符数组来保存字符串，`private final char value[]`，所以 String 对象是不可变的。

> 补充（来自[issue 675](https://github.com/Snailclimb/JavaGuide/issues/675)）：在 Java 9 之后，String 类的实现改用 byte 数组存储字符串 `private final byte[] value`

### StringBuffer和StringBuilder的区别是什么？

StringBuilder 与 StringBuffer 都继承自 AbstractStringBuilder 类，在 AbstractStringBuilder 中也是使用字符数组保存字符串`char[]value` 但是没有用 final 关键字修饰，所以这两种对象都是可变的。

**线程安全性**

StringBuffer 对方法加了同步锁或者对调用的方法加了同步锁，所以是线程安全的。StringBuilder 并没有对方法进行加同步锁，所以是非线程安全的。

**性能**

相同情况下使用 StringBuilder 相比使用 StringBuffer 仅能获得 10%~15% 左右的性能提升，但却要冒多线程不安全的风险。



## 集合框架

### HashMap 和 Hashtable 有什么区别？

HashMap 和 Hashtable 都实现了 Map 接口，因此很多特性非常相似。

不同点：

 1，HashMap 允许键和值是 null，key为null`的键值对永远都放在以`table`[0]为头结点的链表中。`而 Hashtable 不允许键或者值是 null。

2,Hashtable 是线程安全的，而 HashMap 不是，多线程环境下可以采用concurrent并发包下的concurrentHashMap。因此，HashMap 更适合于单线程环境，而 Hashtable 适合于多线程环境。



### HashSet 和 TreeSet 有什么区别？

HashSet 是由一个 hash 表来实现的，因此，它的元素是无序的。add()，remove()，contains() 方法的时间复杂度是 O(1)。

TreeSet 是由一个树形的结构来实现的，它里面的元素是有序的。因此，add()， remove()，contains()方法的时间复杂度是 O(logn)。



## NIO/BIO/AIO

### BIO、NIO、AIO 概念和区别

**BIO**

BIO又被称为阻塞式I/O模型。多线程的BIO中 ，客户端有连接请求时服务器端就需要启动一个线程进行处理。但是随着客户端的增多，服务端的线程数膨胀，系统性能急剧下降，最终会导致系统不可用。这种模型无法满足高并发，高性能的场景。

```java
public class SocketServer {
    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(9000);
        while (true) {
            System.out.println("等待连接。。");
            //阻塞方法
            Socket clientSocket = serverSocket.accept();
            System.out.println("有客户端连接了。。");
            handler(clientSocket);

            /*new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        handler(clientSocket);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();*/
        }
    }

    private static void handler(Socket clientSocket) throws IOException {
        byte[] bytes = new byte[1024];
        System.out.println("准备read。。");
        //接收客户端的数据，阻塞方法，没有数据可读时就阻塞
        int read = clientSocket.getInputStream().read(bytes);
        System.out.println("read完毕。。");
        if (read != -1) {
            System.out.println("接收到客户端的数据：" + new String(bytes, 0, read));
        }
        clientSocket.getOutputStream().write("HelloClient".getBytes());
        clientSocket.getOutputStream().flush();
    }
}
```



**NIO (Non-blocking/New I/O)**

同步非阻塞，服务器实现模式为**一个线程可以处理多个请求(连接)**，客户端发送的连接请求都会注册到**多路复用器selector**上,  多路复用器轮询到连接有IO请求就进行处理。

**应用场景：**

NIO方式适用于连接数目多且连接比较短（轻操作） 的架构， 比如聊天服务器， 弹幕系统， 服务器间通讯，编程比较复杂

**NIO引入多路复用器代码示例：**

```java
public class NioSelectorServer {

    public static void main(String[] args) throws IOException, InterruptedException {

        // 创建NIO ServerSocketChannel
        ServerSocketChannel serverSocket = ServerSocketChannel.open();
        serverSocket.socket().bind(new InetSocketAddress(9000));
        // 设置ServerSocketChannel为非阻塞
        serverSocket.configureBlocking(false);
        // 打开Selector处理Channel，即创建epoll
        Selector selector = Selector.open();
        // 把ServerSocketChannel注册到selector上，并且selector对客户端accept连接操作感兴趣
        serverSocket.register(selector, SelectionKey.OP_ACCEPT);
        System.out.println("服务启动成功");

        while (true) {
            // 阻塞等待需要处理的事件发生
            selector.select();

            // 获取selector中注册的全部事件的 SelectionKey 实例
            Set<SelectionKey> selectionKeys = selector.selectedKeys();
            Iterator<SelectionKey> iterator = selectionKeys.iterator();

            // 遍历SelectionKey对事件进行处理
            while (iterator.hasNext()) {
                SelectionKey key = iterator.next();
                // 如果是OP_ACCEPT事件，则进行连接获取和事件注册
                if (key.isAcceptable()) {
                    ServerSocketChannel server = (ServerSocketChannel) key.channel();
                    SocketChannel socketChannel = server.accept();
                    socketChannel.configureBlocking(false);
                    // 这里只注册了读事件，如果需要给客户端发送数据可以注册写事件
                    socketChannel.register(selector, SelectionKey.OP_READ);
                    System.out.println("客户端连接成功");
                } else if (key.isReadable()) {  // 如果是OP_READ事件，则进行读取和打印
                    SocketChannel socketChannel = (SocketChannel) key.channel();
                    ByteBuffer byteBuffer = ByteBuffer.allocate(128);
                    int len = socketChannel.read(byteBuffer);
                    // 如果有数据，把数据打印出来
                    if (len > 0) {
                        System.out.println("接收到消息：" + new String(byteBuffer.array()));
                    } else if (len == -1) { // 如果客户端断开连接，关闭Socket
                        System.out.println("客户端断开连接");
                        socketChannel.close();
                    }
                }
                //从事件集合里删除本次处理的key，防止下次select重复处理
                iterator.remove();
            }
        }
    }
}
```

上面函数的调用过程：

**首先，会调用系统调用epoll_create创建epoll实例，然后把，ServerSocketChannel 加入到一个集合，再然后调用epoll_ctl() 为其绑定事件ACCEPT，然后调用selector.select() ，这个方法会调用epoll_wait(), 当Socket收到数据的时候，OS会通过中断机制，把对应的socket引用加入到epoll实例的就绪列表中。此时可以根据事件，做出不同的操作，如果是ACCEPT事件，则需要获得对应的ServerSocketChannel ，并调用其Socket方法，获得SocketChannel, 把新的到的SocketChannel 注册到多路复用器上，绑定事件READ（即服务器接收来自客户端的数据）。**

**此时如果客户端发送数据，那么服务器Socket收到数据后，OS启动中断机制，把对应的socket（就是SocketChannel）加入epoll实例的就绪列表中，然后遍历对其进行处理。**

| java                                                     | linux内核方法                            |
| -------------------------------------------------------- | ---------------------------------------- |
| Selector.open();                                         | 调用epoll_create                         |
| selector.select();                                       | epoll_wait()，阻塞等待需要处理的事件发生 |
| serverSocket.register(selector, SelectionKey.OP_ACCEPT); | 调用epoll_ctl() 为其绑定事件ACCEPT       |

NIO 有三大核心组件： **Channel(通道)， Buffer(缓冲区)，Selector(多路复用器)**

1、channel 类似于流，每个 channel 对应一个 buffer缓冲区，buffer 底层就是个数组

2、channel 会注册到 selector 上，由 selector 根据 channel 读写事件的发生将其交由某个空闲的线程处理

3、NIO 的 Buffer 和 channel 都是既可以读也可以写

**AIO**

AIO 也就是 NIO 2。Java 7 中引入了 NIO 的改进版 NIO 2,它是异步 IO 模型。

异步 IO 是基于事件和回调机制实现的，也就是应用操作之后会直接返回，不会堵塞在那里，当后台处理完成，操作系统会通知相应的线程进行后续的操作。

目前来说 AIO 的应用还不是很广泛。Netty 之前也尝试使用过 AIO，不过又放弃了。这是因为，Netty 使用了 AIO 之后，在 Linux 系统上的性能并没有多少提升。

AIO代码示例：

```java
public class AIOServer {

    public static void main(String[] args) throws Exception {
        final AsynchronousServerSocketChannel serverChannel =
                AsynchronousServerSocketChannel.open().bind(new InetSocketAddress(9000));

        serverChannel.accept(null, new CompletionHandler<AsynchronousSocketChannel, Object>() {
            @Override
            public void completed(AsynchronousSocketChannel socketChannel, Object attachment) {
                try {
                    System.out.println("2--"+Thread.currentThread().getName());
                    // 再此接收客户端连接，如果不写这行代码后面的客户端连接连不上服务端
                    serverChannel.accept(attachment, this);
                    System.out.println(socketChannel.getRemoteAddress());
                    ByteBuffer buffer = ByteBuffer.allocate(1024);
                    socketChannel.read(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {
                        @Override
                        public void completed(Integer result, ByteBuffer buffer) {
                            System.out.println("3--"+Thread.currentThread().getName());
                            buffer.flip();
                            System.out.println(new String(buffer.array(), 0, result));
                            socketChannel.write(ByteBuffer.wrap("HelloClient".getBytes()));
                        }

                        @Override
                        public void failed(Throwable exc, ByteBuffer buffer) {
                            exc.printStackTrace();
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void failed(Throwable exc, Object attachment) {
                exc.printStackTrace();
            }
        });

        System.out.println("1--"+Thread.currentThread().getName());
        Thread.sleep(Integer.MAX_VALUE);
    }
}
```



