package org.wenxueliu.concurrent;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.TimeUnit;

public class ThreadLock {
    public InsertData insertData = new InsertData();
    Lock lock = new ReentrantLock();    //注意这个地方
    private ReentrantReadWriteLock rwl = new ReentrantReadWriteLock();

    public void testGlobalLock() {
        new Thread(){
            @Override
            public void run() {
                insert2(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                insert2(Thread.currentThread());
            };
        }.start();
    }

    public void testLocalLock() {
        new Thread(){
            @Override
            public void run() {
                insert1(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                insert1(Thread.currentThread());
            };
        }.start();
    }

    public void testTryLock() {
        new Thread(){
            @Override
            public void run() {
                insert3(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                insert3(Thread.currentThread());
            };
        }.start();
    }

    public void testInterruptLock() {
        new Thread(){
            @Override
            public void run() {
                try {
                    insert4(Thread.currentThread());
                } catch (InterruptedException e) {
                    System.out.println(Thread.currentThread().getName()+"被中断");
                }
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                try {
                    insert4(Thread.currentThread());
                } catch (InterruptedException e) {
                    System.out.println(Thread.currentThread().getName()+"被中断");
                }
            };
        }.start();
    }

    public void testSynchronizedGet(){
        new Thread(){
            @Override
            public void run() {
                get1(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                get1(Thread.currentThread());
            };
        }.start();
    }

    public void testReentrantReadWriteLockGet(){
        new Thread(){
            @Override
            public void run() {
                get2(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                get2(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                insert5(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                insert5(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                get2(Thread.currentThread());
            };
        }.start();

        new Thread(){
            @Override
            public void run() {
                get2(Thread.currentThread());
            };
        }.start();
    }

    public void get2(Thread thread) {
        rwl.readLock().lock();
        try{
            System.out.println(thread.getName()+"得到了锁");
            insertData.get1(thread);
            System.out.println(thread.getName()+"释放了锁");
        } finally {
            rwl.readLock().unlock();
        }
    }


    public synchronized void get1(Thread thread) {
        System.out.println(thread.getName()+"得到了锁");
        insertData.get1(thread);
        System.out.println(thread.getName()+"释放了锁");
    }

    public void insert1(Thread thread) {
        Lock lock1 = new ReentrantLock();    //注意这个地方
        lock1.lock();
        try {
            System.out.println(thread.getName()+"得到了锁");
            insertData.insert1(thread);
        } catch (Exception e) {
            // TODO: handle exception
        }finally {
            System.out.println(thread.getName()+"释放了锁");
            lock1.unlock();
        }
    }

    public void insert2(Thread thread) {
        lock.lock();
        try {
            System.out.println(thread.getName()+"得到了锁");
            insertData.insert1(thread);
        } catch (Exception e) {
            // TODO: handle exception
        }finally {
            System.out.println(thread.getName()+"释放了锁");
            lock.unlock();
        }
    }

    public void insert3(Thread thread) {
        if (lock.tryLock()) {
            try {
                System.out.println(thread.getName()+"得到了锁");
                insertData.insert1(thread);
            } catch (Exception e) {
                // TODO: handle exception
            }finally {
                System.out.println(thread.getName()+"释放了锁");
                lock.unlock();
            }
        } else {
            System.out.println(thread.getName()+"获取锁失败");
        }
    }

    public void insert4(Thread thread) throws InterruptedException {
        lock.lockInterruptibly();
        try {
            System.out.println(thread.getName()+"得到了锁");
            insertData.insert1(thread);
            TimeUnit.SECONDS.sleep(3);
        } finally {
            System.out.println(thread.getName()+"释放了锁");
            lock.unlock();
        }
    }

    public void insert5(Thread thread) {
        rwl.writeLock().lock();
        try{
            System.out.println(thread.getName()+"得到了锁");
            insertData.insert1(thread);
            System.out.println(thread.getName()+"释放了锁");
        } finally {
            rwl.writeLock().unlock();
        }
    }

    public static void main(String[] args)  {
        final ThreadLock tl = new ThreadLock();
        tl.testLocalLock();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        tl.testGlobalLock();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        tl.testTryLock();
        tl.testInterruptLock();
        tl.testSynchronizedGet();
        tl.testReentrantReadWriteLockGet();
    }
}
