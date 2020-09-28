# 1，线程的创建与启动

## 1.1 继承Thread类创建线程类

- 1.定义Thread类的子类，并重写该类的run()方法，该run()方法的方法体就代表了线程需要完成的任务，  因此经常把run（）方法称为线程执行体。

- 2.创建Thread子类的实例，即创建了线程对象

- 3.调用线程对象的start()方法就可以启动该线程

## 1.2 实现Runnable接口创建线程类

- 定义Runnable接口的实现类，并重写该接口的run（）方法，该run（）方法的方法体也称之为线程方法执行体。
- 创建Runnable实现类的实例，并将此实例作为形参传入new Thread（）的构造函数中，就可创建Thread线程对象

## 1.3 使用Callable和Future创建线程

- 创建Callable接口的实现类，重写call()方法，该方法就是线程方法执行体，call()方法有返回值，再创建Callable实现类的实例。

- 使用FutureTask类的实例，来包装Callable对象，即把callable的实例以形参的方式传入new FutureTask()的构造函数中

- 使用FutureTask对象作为Thread对象的target创建启动线程。

- 通过FutureTask实例对象调用get()方法得到子线程的返回值。

```java
public class Demo {
    public static void main(String[] args) {
        //创建Callable对象
        ThirdThreadCallable callable = new  ThirdThreadCallable();
        //创建FutureTask对象，并把callable以形参的方式传入FutureTask的构造方法内
        FutureTask<Integer> futureTask = new FutureTask<>(callable);
        for (int i = 0; i < 30; i++) {
            // 通过Thread类的currentThread方法可以得到当前的线程名
            System.out.println(Thread.currentThread().getName()+" "+i);
            if (i==20) {
                //创建线程并启动
                new Thread(futureTask, "有返回值的线程").start();
            }
        }
        //获取子线程的返回值
        try {
            System.out.println("子线程的返回值： "+futureTask.get());
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
    }
}

class ThirdThreadCallable implements Callable<Integer> {
    int i ;
    //call（）方法称之为线程方法执行体，且该方法有返回值，可通过FutureTask实例对象调用get（）方法得到子线程的返回值
    @Override
    public Integer call() throws Exception {
        for (; i <30; i++) {
            System.out.println(Thread.currentThread().getName()+" "+i);
        }
        return i;
    }

}
```

## 1.4 创建线程的三种方式对比

**使用实现Runnable 、Callable接口的方式创建多线程**

优点

- 线程类只是实现了Runnable接口或Callable接口，同时还可以继承其他类。

- 多个线程可以共享一个target对象，非常适合多个相同线程来处理同一份资源的情况，从而可以将CPU、代码、数据分开，形成清晰的模型，较好的体现了面向对象的思想

缺点

- 编程稍微复杂，如果需要访问当前线程，必须使用Thread.currentThread()方法。



**使用继承Thread类的方式创建多线程**

优点：

- 编写简单，如果要访问当前线程，无需使用Thread.currentThread（）方法，可以直接使用this的方式获取当前线程

缺点：

- 因为线程类已经继承了Thread类，所以不能再继承其他的父类。

# 2 线程的生命周期图

![](./img/1.png)





**新建状态（New）**

当程序使用new关键字创建了一个线程之后，该线程就处于新建状态。例：

Thread t = new MyThread();

**就绪状态（Runnable）**

调用start()方法后，线程就会处于就绪状态；此时只能说明此线程已经做好了执行的就绪准备，表示可以运行了，但还不是正在运行的线程。

**运行状态（Running）**

当线程获得到JVM中线程调度器的调度时，线程就会处于运行状态，开始执行run()方法的线程执行体。

**阻塞状态（Blocked）**

这是线程仍处于活动状态但当前没有资格运行（因为一些原因，暂时放弃对CPU的使用权，停止执行）时的状态。

**线程进入阻塞的三种状态：**

**1、等待阻塞：**

1）线程调用wait()方法，使本线程进入到等待状态；

2）调用join()方法，使本线程进入到另一种等待状态：等待别线程终止或者超时后才执行自己这个线程。

**2、同步阻塞**

线程在获取synchronized同步锁失败(因为锁被其它线程所占用)，它会进入同步阻塞状态；

**3、其他阻塞**

通过调用线程的sleep()或join()或发出了I/O请求时，线程会进入到阻塞状态。当sleep()状态超时、join()等待线程终止或者超时、或者I/O处理完毕时，线程重新转入就绪状态。

**死亡状态（Dead）**

当退出run（）方法时，线程就会自然死亡，处于终止或死亡状态，也就结束了生命周期。

# 3, 控制线程

## 3.1 join

- 在A线程中调用了B线程的join()方法时，表示只有当B线程执行完毕时，A线程才能继续执行。
- join方法可以传递参数，join(10)表示A线程会等待B线程10毫秒，10毫秒过去后。
- 需要注意的是，jdk规定，join(0)的意思不是A线程等待B线程0秒，而是A线程等待B线程无限时间，直到B线程执行完毕，即join(0)等价于join()。
- join方法必须在线程start方法调用之后调用才有意义。这个也很容易理解：如果一个线程都没有start，那它也就无法同步了。

```java
public class JoinTest {
	public static void main(String[] args) throws InterruptedException {
		ThreadJoinTest t1 = new ThreadJoinTest("小明");
		ThreadJoinTest t2 = new ThreadJoinTest("小东");
		t1.start();
		/**join的意思是使得放弃当前线程的执行，并返回对应的线程，例如下面代码的意思就是：
		 程序在main线程中调用t1线程的join方法，则main线程放弃cpu控制权，并返回t1线程继续执行直到线程t1执行完毕
		 所以结果是t1线程执行完后，才到主线程执行，相当于在main线程中同步t1线程，t1执行完了，main线程才有执行的机会
		 join方法可以传递参数，join(10)表示main线程会等待t1线程10毫秒，10毫秒过去后，
         * main线程和t1线程之间执行顺序由串行执行变为普通的并行执行
		 */
		t1.join();
        //t1.join(10)
		t2.start();
	}
}
class ThreadJoinTest extends Thread {
	public ThreadJoinTest(String name) {
		super(name);
	}

	@Override
	public void run() {
		for (int i = 0; i < 10; i++) {
			System.out.println(this.getName() + ":" + i);
		}
	}
}

```

## 3.2 守护线程setDaemon(true)

​		后台线程，它是在后台运行的，它的任务是为其他线程提供服务，这种线程被称为“后台线程（Daemon Thread）”,又称为“守护线程”或“精灵线程”。JVM的垃圾回收线程就是典型的后台线程。

**后台线程的特征：**如果**所有的前台线程都死亡了，后台线程也会自动死亡。**

调用Thread对象的**setDaemon(true)**可将制定线程设置为后台线程。所有的前台线程都死亡时，后天线程也死亡，程序就退出了。

注意：**设置为后台线程setDaemon(true)，注意必须在start前掉用，否则会报错！**

## 3.3 控制线程睡眠。sleep

注意：**sleep方法只能让当前线程睡眠**。调用某一个线程类的对象t.sleep()，睡眠的不是t，而是当前线程。

1, Thread.sleep();

sleep(long millis) 线程睡眠 millis 毫秒

sleep(long millis, int nanos) 线程睡眠 millis 毫秒 + nanos 纳秒

2, TimeUnit.单位.sleep()

**TimeUnit.SECONDS.sleep(1);**

**TimeUnit.MINUTES.sleep(**1);

**TimeUnit.HOURS.sleep(1);**

**TimeUnit.DAYS.sleep(1);**

## 3.4 改变线程优先级 setPriority()

- 每个线程在执行时都具有一定的优先级，优先级高的线程获得较多的执行机会，而优先级低的线程则获得较少的执行机会。
- 每个线程默认的优先级都与创建它的父线程的优先级相同，在默认情况下，main线程具有普通优先级，由main线程创建的子线程也具有普通优先级。
- **java 中的线程优先级的范围是1～10，1的优先级最低，10的优先级最高。**通过**setPriority()**方法，可以改变线程的优先级。

## 3.5 线程让步 yield()

Thread中有一个线程让步方法（静态方法）yield()，作用就是让步。它能让当前线程由“运行状态”进入到“就绪状态”，从而让其它具有相同优先级的等待线程获取执行权；但是，并不能保证在当前线程调用yield()之后，其它具有相同优先级的线程就一定能获得执行权；也有可能是当前线程又进入到“运行状态”继续运行！

**在多CPU并行的环境下，yield方法的功能有时候不明显，可能看不到效果。**

# 4， 线程同步

**一、什么情况下会产生线程安全问题？**

同时满足以下两个条件时：

1，多个线程在操作共享的数据。

2，操作共享数据的线程代码有多条。

当一个线程在执行操作共享数据的多条代码过程中，其他线程参与了运算，就会导致线程安全问题的产生。

**线程不安全的例子**

```java
public class TicketDemo implements Runnable {
	private  int tickets = 100; //定义在这个地方的变量是所有线程共享的变量
	@Override
	public void run() {
		while (tickets > 0) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
			}
			System.out.println(Thread.currentThread().getName() + "....卖出的票号:...." + tickets);
			tickets--;
		}
	}
	public static void main(String[] args) {
		TicketDemo ticketDemo = new TicketDemo();
		Thread t1 = new Thread(ticketDemo);
		Thread t2 = new Thread(ticketDemo);
		Thread t3 = new Thread(ticketDemo);
		Thread t4 = new Thread(ticketDemo);
		t1.start();
		t2.start();
		t3.start();
		t4.start();
	}
}

```

**我们发现可能会有多个线程卖同一张票的情况发生，这就是线程安全问题。**

## 4.1 线程同步概念

**什么是线程同步：**

- 同步就是协同步调，按预定的先后次序进行运行。如：你说完，我再说。这里的同步千万不要理解成那个同时进行，应是指协同、协助、互相配合。

- 线程同步是指多线程通过特定的设置来控制线程之间的执行顺序（即所谓的同步）也可以说是在线程之间通过同步建立起执行顺序的关系，如果没有同步，那线程之间是各自运行各自的！