package org.wenxueliu.concurrent;

import java.util.ArrayList;

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

    public static void main(String[] args)  {
        ThreadSyn ts = new ThreadSyn();
        System.out.println("------------ normal Test ------------");
        ts.test();
        System.out.println("------------ synchronized Test ------------");
        ts.testSyn();
        System.out.println("------------ synchronized static Test ------------");
        ts.testStaticSyn();
    }

}



