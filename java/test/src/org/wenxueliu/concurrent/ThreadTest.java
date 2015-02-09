package org.wenxueliu.concurrent;

import java.io.IOException;

public class ThreadTest {
    private int i = 10;
    private Object object = new Object();

    public Thread myThread1 = new Thread() {
        @Override
        public void run() {
            synchronized (object) {
                i++;
                System.out.println("i:"+i);
                try {
                    System.out.println("线程"+Thread.currentThread().getName()+"进入睡眠状态");
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    // TODO: handle exception
                }
                System.out.println("线程"+Thread.currentThread().getName()+"睡眠结束");
                i++;
                System.out.println("i:"+i);
            }
        }
    };

    public Thread myThread2 = new Thread() {
        @Override
        public void run() {
            synchronized (object) {
                i++;
                System.out.println("i:"+i);
                try {
                    System.out.println("线程"+Thread.currentThread().getName()+"进入睡眠状态");
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    // TODO: handle exception
                }
                System.out.println("线程"+Thread.currentThread().getName()+"睡眠结束");
                i++;
                System.out.println("i:"+i);
            }
        }
    };

    public void testSelf() {
        myThread1.start();
        myThread2.start();
    }

    public static void main(String[] args) throws IOException  {
        ThreadTest test = new ThreadTest();
        test.testSelf();
    }
}


