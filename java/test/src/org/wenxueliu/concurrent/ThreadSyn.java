package org.wenxueliu.concurrent;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;

public class ThreadSyn {

    public void test() {
        final InsertData insertData = new InsertData();

        new Thread() {
            public void run() {
                insertData.insert1(Thread.currentThread());
            };
        }.start();

        new Thread() {
            public void run() {
                insertData.insert1(Thread.currentThread());
            };
        }.start();
    }

    public void testSyn() {
        final InsertData insertData = new InsertData();

        new Thread() {
            public void run() {
                insertData.insert2(Thread.currentThread());
            };
        }.start();

        new Thread() {
            public void run() {
                insertData.insert2(Thread.currentThread());
            };
        }.start();
    }

    public void testStaticSyn() {
        final InsertData insertData = new InsertData();

        new Thread() {
            public void run() {
                insertData.insert2(Thread.currentThread());
            };
        }.start();

        new Thread() {
            public void run() {
                InsertData.insert3();
            };
        }.start();
    }

	public void CountDownLatchTest() {
         final CountDownLatch latch = new CountDownLatch(2);

         new Thread(){
             public void run() {
                 try {
                     System.out.println("子线程"+Thread.currentThread().getName()+"正在执行");
                    Thread.sleep(3000);
                    System.out.println("子线程"+Thread.currentThread().getName()+"执行完毕");
                    latch.countDown();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
             };
         }.start();

         new Thread(){
             public void run() {
                 try {
                     System.out.println("子线程"+Thread.currentThread().getName()+"正在执行");
                     Thread.sleep(3000);
                     System.out.println("子线程"+Thread.currentThread().getName()+"执行完毕");
                     latch.countDown();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
             };
         }.start();

         try {
            System.out.println("执行主线程执行");
            System.out.println("等待2个子线程执行完毕...");
            latch.await();
            System.out.println("2个子线程已经执行完毕");
            System.out.println("继续执行主线程");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
     }

	public void CyclicBarrierTest() {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N);
        for(int i=0;i<N;i++)
            new Writer(barrier).start();

		try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }

        System.out.println("CyclicBarrier Reuse");
        for(int i=0;i<N;i++)
            new Writer(barrier).start();

		try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }

        CyclicBarrier barrier1  = new CyclicBarrier(N,new Runnable() {
            @Override
            public void run() {
                System.out.println("当前线程"+Thread.currentThread().getName());
            }
        });
        for(int i=0;i<N;i++)
            new Writer(barrier1).start();

	}

	class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }

        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(1000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println("所有线程写入完毕，继续处理其他任务...");
        }
    }

    public static void main(String[] args)  {
        ThreadSyn ts = new ThreadSyn();
        System.out.println("------------ normal Test ------------");
        ts.test();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("------------ synchronized Test ------------");
        ts.testSyn();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("------------ synchronized static Test ------------");
        ts.testStaticSyn();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
		System.out.println("------------  CountDownLatchTest ------------");
		ts.CountDownLatchTest();
		try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
		System.out.println("------------  CyclicBarrierTest ------------");
		ts.CyclicBarrierTest();
	}

}



