# 基础知识

## 什么是线程

进程是代码在数据集上的一次运行活动，是系统进行资源分配和调度的基本单位，线程是进程的一个执行路径。

线程是处理机调度的基本单位。

![image-20210223134928943](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223134928943.png)

一个进程中有多个线程，多个线程共享JVM进程的堆和方法去资源，每个线程里面有自己的程序计数器和虚拟机栈。程序计数器记录了当前线程让出CPU时（可能时时间片到了），线程的执行位置。

## 线程创建

调用Thread 的start()方法时，线程并没有马上执行，而是处于就绪态，就绪态就是该线程获取了除CPU资源以外的其他资源。

线程创建3种方式

1. 继承Thread类，重写run方法
2. 实现Runnable接口
3. 使用FutureTask的方式，带有返回值

第三种示例：

```java
public class Main {
	static class CallerTask implements Callable<String>{

		@Override
		public String call() throws Exception {
			return "hello";
		}
	}

	public static void main(String[] args) throws ExecutionException, InterruptedException {
		FutureTask<String> futureTask = new FutureTask<>(new CallerTask());
		new Thread(futureTask).start();
		String s = futureTask.get();
		System.out.println(s);
	}
}
```

## 线程通知与等待

1， Object 类里面 wait() 函数

​		调用这个函数必须事先获取该对象的监视器锁，否则会抛出异常。

​		当一个线程调用一个共享变量的wait()方法的时候，**该调用线程会挂起，并且释放锁**，直到发生：1）其他线程调用了该对象的notify()， 或者notifyAll()。2）其他线程调用了该线程的interrupt() 方法，该线程抛出InterruputtedExceptioin。

虚假唤醒问题：一个线程可以从挂起状态转换成运行态，即使该线程没有被其他线程调用notify()方法。为了防患于未然，可以在一个循环中调用wait()，如下：

```java
synchonized(obj){
	while(条件不满足){
		obj.wait();
	}
}
```

生产者消费者问题：

自己写的，消费者线程不会停。

```java
public class Main {
	//生产者消费者问题，生产者生产1-10的数，放到容量为2队列中，消费者消费
	static final int MAX_SIZE = 2;
	static final Queue<Integer> queue = new LinkedList<>();

	public static void main(String[] args) {
		//生产者线程
		new Thread(() -> {
			synchronized (queue) {
				for (int i = 1; i <= 10; i++) {
					while (queue.size() == MAX_SIZE) {
						try {
							//挂起当前线程，释放queue上的锁，让消费者可以获取该锁，
							queue.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}

					queue.add(i);
					System.out.println("生产者生产：" + i);
					queue.notify();
				}
			}
		}).start();

		//消费者线程
		new Thread(() -> {
			synchronized (queue) {
				while (true){
					while (queue.size() == 0) {
						try {
							//挂起当前线程，释放queue上的锁，让生产者可以生产
							queue.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					System.out.println("消费者消费：" + queue.poll());
					queue.notify();
				}
			}
		}).start();

	}

}
```

## 等待线程执行终止的join方法

```java
public class Main {
	public static void main(String[] args) throws InterruptedException {
		Thread t1 = new Thread(()->{
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("任务1结束");
		});
		Thread t2 = new Thread(()->{
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("任务2结束");
		});
		//等待t1,t2结束之后，主线程才会继续执行
		t1.join();
		t2.join();
		System.out.println("任务结束");
	}
}

```

## Sleep()方法

这个方法会让线程让出指定时间的执行权，在这一期间不参与CPU的调度，但是不释放该线程所拥有的监视器资源。指定时间过后，线程就又会处于就绪态。

## Yield()

当一个线程调用了该方法，当前线程会让出cpu的使用权，然后处于就绪态，然后线程调度器会从线程就绪队列里面获取一个线程优先级最高的线程。

## 线程中断

interrupt() 方法：该标志仅仅是设置标记，如果线程A因为调用了wait，join，或者sleep等方法被阻塞挂起的时候，这个时候线程b调用a的这个interrupt，会使a在调用这些方法的地方抛出异常。

interrupted() 方法（Thread的静态方法）：检测**当前线程**是否被中断，如果是，则返回true，并清除中断标记。

看一道题：

```java
public class Main {
	public static void main(String[] args) throws InterruptedException {
		Thread t1 = new Thread(()->{
			while (true){

			}
		});
		t1.start();
		t1.interrupt();
		t1.start();
		t1.interrupt();
		System.out.println(t1.isInterrupted()); //true
		System.out.println(t1.interrupted());//false
		System.out.println(Thread.interrupted());//false
		Thread.currentThread().interrupt();
		System.out.println(Thread.currentThread().isInterrupted());//true
		System.out.println(Thread.interrupted());//true
		System.out.println(Thread.interrupted());//false
		System.out.println(t1.isInterrupted());//true
	}
}
```

## 死锁产生的四个必要条件

1. 互斥条件。互斥的使用资源。
2. 请求并保持。申请不到一个新的资源，又持有原有的 资源不放手。
3. 不可剥夺。
4. 环路等待。

### 死锁的代码示例

```java
public class Main {
	public static void main(String[] args) throws InterruptedException {
		Object a = new Object(), b = new Object();
		new Thread(()->{
			synchronized (a){
				try {
					Thread.sleep(1000);
					synchronized (b) {
						System.out.println("**");
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}).start();

		new Thread(()->{
			synchronized (b){
				try {
					Thread.sleep(1000);
					synchronized (a) {
						System.out.println("**");
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}).start();
	}
}
```

## 避免死锁

使用资源遵循 有序性即可，破坏破坏请求和保持，和环路等待。只需把上面代码线程2改为：

```
new Thread(()->{
			synchronized (a){
				try {
					Thread.sleep(1000);
					synchronized (b) {
						System.out.println("**");
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}).start();
```

即可。

## 守护线程与用户线程

setDaemon(true)

守护线程的结束与否，不会影响JVM的正常退出。main线程结束后，JVM会启动一个叫做DestroyJavaVM的线程，该线程会等待所有用户线程结束之后终止JVM进程。

垃圾收集线程就是守护线程。

## ThreadLocal 原理

如果创建了ThreadLocal变量，那么访问这个变量的每一个线程都会有这个变量的本地副本。

![image-20210223163829100](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223163829100.png)

![image-20210223164049065](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223164049065.png)

**set方法**

```java
public void set(T value) {
    //(1)获取当前线程（调用者线程）
    Thread t = Thread.currentThread();
    //(2)以当前线程作为key值，去查找对应的线程变量，找到对应的map
    ThreadLocalMap map = getMap(t);
    //(3)如果map不为null，就直接添加本地变量，key为当前线程，值为添加的本地变量值
    if (map != null)
        map.set(this, value);
    //(4)如果map为null，说明首次添加，需要首先创建出对应的map
    else
        createMap(t, value);
}
```

getMap

```java
ThreadLocalMap getMap(Thread t) {
    return t.threadLocals; //获取线程自己的变量threadLocals，并绑定到当前调用线程的成员变量threadLocals上
}
```

createMap

```java
 void createMap(Thread t, T firstValue) {
     t.threadLocals = new ThreadLocalMap(this, firstValue);
 }
```

**get**

```java
public T get() {
    //(1)获取当前线程
    Thread t = Thread.currentThread();
    //(2)获取当前线程的threadLocals变量
    ThreadLocalMap map = getMap(t);
    //(3)如果threadLocals变量不为null，就可以在map中查找到本地变量的值
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null) {
            @SuppressWarnings("unchecked")
            T result = (T)e.value;
            return result;
        }
    }
    //(4)执行到此处，threadLocals为null，调用该更改初始化当前线程的threadLocals变量
    return setInitialValue();
}

private T setInitialValue() {
    //protected T initialValue() {return null;}
    T value = initialValue();
    //获取当前线程
    Thread t = Thread.currentThread();
    //以当前线程作为key值，去查找对应的线程变量，找到对应的map
    ThreadLocalMap map = getMap(t);
    //如果map不为null，就直接添加本地变量，key为当前线程，值为添加的本地变量值
    if (map != null)
        map.set(this, value);
    //如果map为null，说明首次添加，需要首先创建出对应的map
    else
        createMap(t, value);
    return value;
}
```

![image-20210223164453823](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223164453823.png)

如图：每个线程里面都有一个名为threaLocal 的变量，类型为map，key为我们定义的ThreadLocal 变量的this引用，value则是我们使用set方法设置的值。如果当前线程一直不消亡，那么这些本地变脸会一直存在，所以可能会造成内存溢出，因此要记得调用ThreadLocal的remove。

看一道题

```java
public class Main {
	static ThreadLocal<String> threadLocal = new ThreadLocal<>();
	public static void main(String[] args) throws InterruptedException {
		threadLocal.set("Hello");
		new Thread(()->{
			System.out.println(threadLocal.get());//null
		}).start();
		System.out.println(threadLocal.get());//hello
	}
}
```

### InheritableThreadLocals

为了解决上面的问题，inheritableThreadLocals搞出来了，它的一个特性就是让子线程可以访问父线程中设置的本地变量。

InheritableThreadLocals代码：

![image-20210223170100221](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223170100221.png)

![image-20210223170107787](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223170107787.png)

如何实现子线程访问父线程ThreadLocal的值呢？

看一下Thread的初始化方法：

![image-20210223170209659](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223170209659.png)

可以发现，创建子线程的时候，会把父线程的InheritableThreadLocals复制到子线程中去。（注意，是深拷贝，之后，父子线程里面的本地变量就不想关了）

```java
public class Main {
	static InheritableThreadLocal<String> inheritableThreadLocal = new InheritableThreadLocal<>();
	public static void main(String[] args) throws InterruptedException {
		inheritableThreadLocal.set("Hello");
		new Thread(()->{
			System.out.println(inheritableThreadLocal.get());//hello
			try {
				Thread.sleep(3000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println(inheritableThreadLocal.get());//hello
		}).start();
		System.out.println(inheritableThreadLocal.get());//hello
		Thread.sleep(1000);
		inheritableThreadLocal.set("Hello World!");
		System.out.println(inheritableThreadLocal.get());//Hello World!
	}
}
```

## java内存模型

![image-20210223173220952](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223173220952.png)

java内存模型规定：所有的变量都存放在主内存中，当线程使用变量的时候，会把主内存里面的变量复制到自己的工作空间。

真实的情况：（双核CPU，L1为cpu的1级缓存（寄存器），l2为高速缓冲存储器）

![image-20210223173356308](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223173356308.png)

A把共享变量X, L1,L2没有命中，于是把X=0加载到L2, L1。A然后修改X=1,并将其写到L1,L2,和主内存。

B获取X, L1没有命中，L2命中，于是把X=1 加载到B 的L1。然后B修改X=2,并将其写到L1,L2,和主内存。

这时，A还想修改，于是获取X, 此时L1 命中了，把X=1加载到L1, 这就有一个问题，明明我主内存中X=2。

## Syncronized内存语义

![image-20210223174354725](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223174354725.png)

但是使用锁机制会带来上下文切换的开销。

## Volatile

当一个变量被声名成volatile,线程在写入变量的时候不会把值存在寄存器中，而是直接刷新到主内存中。读取时，直接从主存中读取到变量的最新值。

volatile 不保证原子性。

## Unsafe类 实现CAS

```java
public class Main {
	// 这是实例化不了的
//	static  Unsafe unsafe = Unsafe.getUnsafe();
	static  Unsafe unsafe;
	//记录state在Main中的偏移量
	static  long stateOffset;
	public  volatile  long state = 0;
	static {
		try {
			//使用反射
			Field field = Unsafe.class.getDeclaredField("theUnsafe");
			field.setAccessible(true);
			unsafe = (Unsafe) field.get(null);

			//获取state在Main中的偏移量
			stateOffset = unsafe.objectFieldOffset(Main.class.getDeclaredField("state"));
		} catch (NoSuchFieldException | IllegalAccessException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		Main m = new Main();
		//设置state的值为1
		boolean b = unsafe.compareAndSwapInt(m, stateOffset, 0, 1);
		System.out.println(b);

	}
}
```

![image-20210223183136219](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223183136219.png)

![image-20210223183158355](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223183158355.png)

AtmaticLong 就是 使用上面compareAndSwapLong来实现的。

## 指令重排

![image-20210223183917298](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223183917298.png)

![image-20210223193209499](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223193209499.png)

可以使用volatile关键字，可以确保volatile写之前的操作不会被编译器重排到volatile之后。读volatile时，确保volatle读之后的操作不会被重排到volatile之前。

## 伪共享

![image-20210223202512037](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202512037.png)

![image-20210223202540616](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202540616.png)

![image-20210223202555420](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202555420.png)

![image-20210223202643458](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202643458.png)



![image-20210223202704860](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202704860.png)

看一个问题：

```java
//1
public static void main(String[] args) {
    int row = 1024, col = 1024;
    long[][] arr = new long[row][col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] = 1L;
        }
    }
}
//2
public static void main(String[] args) {
    int row = 1024, col = 1024;
    long[][] arr = new long[row][col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[j][i] = 1L;
        }
    }
}
```

为什么1比2执行的快？

数组内数组元素的地址是连续的，访问第一个元素的时候，会把第一个元素后的若干元素放入缓存行，这样顺序访问数组元素时可以加快访问速度。而2对数组的访问是跳跃式的，破坏了程序访问的局部性原则，缓存是有容控的，会导致缓存行中的数据没有被访问到就又被替换掉了。



### 如何避免伪共享？

1，字段填充

![image-20210223202946192](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202946192.png)

2，![image-20210223202959493](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223202959493.png)

![image-20210223203103382](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223203103382.png)

## 锁

### 乐观锁 versus 悲观锁

- **悲观锁：传统的锁机制**，例如 java 的 synchronized 关键字，他代表了 java 中悲观锁技术，保证了某一时刻仅有一个线程能访问同步代码/方法。synchronized 能够很好地工作，却有着 (相对) 比较大的性能开销。
- **乐观锁** (相对悲观锁) 对性能会有很大的帮助。他的核心思想是：寄希望于在没有冲突的情况下完成一次更新操作，使用乐观锁技术更新时会进行 “冲突检测” 来判断是否有其他的线程干扰，若是 (有其他线程干扰) 则视本次更新操作失败，一般会进行重试。在数据库中就是多版本控制，最后 更新的时候 会`update ... set version = ${version} + 1 ... where version = ${version}`, 如果version被别人修改了，则我这次修改失败。乐观锁不会使用数据库提供的锁机制，一般在表中添加version字段来实现。

### 公平锁与非公平锁

公平锁：最早请求的线程将最早获得锁。

非公平锁：随机。Reentrant

![image-20210223212110093](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223212110093.png)

### 独占锁 与 共享锁

根据能否被多个线程共同持有来决定。

### 可重入锁

当一个线程再次获取他自己已经获取的锁，如果不被阻塞，则称该锁为可重入锁。synchronized 就是可重入锁。

实现机制：在锁的内部维护一个线程标识，用来标识该锁被那个线程占用，然后关联一个计数器。一开始 计数器值为0表示锁没有被任何线程占用，当一个线程获取了该锁，计数器就会变成1，如果 这个线程又获取一次这个锁，就会把引用计数器+1， 释放则会减1。为0时，表示该锁被释放了。

### 自旋锁

自旋锁是当前线程在获取锁的时候发现被其他线程占有，不马上阻塞自己，不放弃CPU使用权的情况下，多次尝试获取（默认10次），很可能后面几次其他线程已经释放了锁。

其实就是用CPU时间换取线程阻塞与调度的开销



