package org.wenxueliu.concurrent;

import java.util.Random;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

/**
 * @author qifuguang
 * @date 15/8/25 00:34
 * @url http://qifuguang.me/2015/08/25/%5BJava%E5%B9%B6%E5%8F%91%E5%8C%85%E5%AD%A6%E4%B9%A0%E4%BA%94%5DCountDownLatch%E5%92%8CCyclicBarrier%E4%BB%8B%E7%BB%8D/?comefrom=http://blogread.cn/news/
 * 总结
 *  CountDownLatch和CyclicBarrier都能够实现线程之间的等待，只不过它们侧重点不同：
 *  1. CountDownLatch一般用于某个线程A等待若干个其他线程执行完任务之后，它才执行；
 *  2. CyclicBarrier一般用于一组线程互相等待至某个状态，然后这一组线程再同时执行；
 *  3. CountDownLatch是不能够重用的，而CyclicBarrier是可以重用的。
 */
@SuppressWarnings("all") public class MyCyclicBarrier {
    private static final int THREAD_NUMBER = 5;
    private static final Random RANDOM = new Random();


    static class Worker implements Runnable {
        private CyclicBarrier barrier;

        public Worker(CyclicBarrier barrier) {
            this.barrier = barrier;
        }

        public void run() {
            int time = RANDOM.nextInt(1000);
            System.out.println(Thread.currentThread().getId() + "：我需要" + time + "毫秒时间写入数据.");
            try {
                Thread.sleep(time);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getId() + "：写入数据完毕，等待其他小伙伴...");
            try {
                barrier.await(2000, TimeUnit.MILLISECONDS); // 等待所有线程都调用过此函数才能进行后续动作
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            } catch (TimeoutException e) {
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getId() + "：所有线程都写入数据完毕，继续干活...");
        }
    }

    public void noloop() {
        CyclicBarrier barrier = new CyclicBarrier(THREAD_NUMBER, new Runnable() {
            public void run() {
                System.out.println(Thread.currentThread().getId() + "：我宣布，所有小伙伴写入数据完毕");
            }
        });
        for (int i = 0; i < THREAD_NUMBER; i++) {
            Thread t = new Thread(new Worker(barrier));
            t.start();
        }
    }

    public void loop() {
        CyclicBarrier barrier = new CyclicBarrier(THREAD_NUMBER, new Runnable() {
            public void run() {
                System.out.println(Thread.currentThread().getId() + "：我宣布，所有小伙伴写入数据完毕");
            }
        });
        for (int i = 0; i < THREAD_NUMBER; i++) {
            Thread t = new Thread(new Worker(barrier));
            t.start();
        }
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("================barrier重用==========================");
        for (int i = 0; i < THREAD_NUMBER; i++) {
            Thread t = new Thread(new Worker(barrier));
            t.start();
        }
    }

    public void timeout() {
        CyclicBarrier barrier = new CyclicBarrier(THREAD_NUMBER, new Runnable() {
            public void run() {
                System.out.println(Thread.currentThread().getId() + "：我宣布，所有小伙伴写入数据完毕");
            }
        });
        for (int i = 0; i < THREAD_NUMBER; i++) {
            if (i < THREAD_NUMBER - 1) {
                Thread t = new Thread(new Worker(barrier));
                t.start();
            } else {  //最后一个线程故意延迟3s创建。
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Thread t = new Thread(new Worker(barrier));
                t.start();
            }
        }
    }

    public static void main(String[] args) throws Exception {
        MyCyclicBarrier mycb = new MyCyclicBarrier();
        mycb.noloop();
        mycb.loop();
        mycb.timeout();
    }
}
