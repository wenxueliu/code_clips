package org.wenxueliu.concurrent;

import java.util.Random;
import java.util.concurrent.CountDownLatch;

/**
 * @author qifuguang
 * @date 15/8/24 23:35
 */
@SuppressWarnings("all") public class MyCountDownLatch {
    private static final int RUNNER_NUMBER = 5; // 运动员个数
    private static final Random RANDOM = new Random();

    public static void main(String[] args) {
        // 用于判断发令之前运动员是否已经完全进入准备状态，需要等待5个运动员，所以参数为5
        CountDownLatch readyLatch = new CountDownLatch(RUNNER_NUMBER);
        // 用于判断裁判是否已经发令，只需要等待一个裁判，所以参数为1
        CountDownLatch startLatch = new CountDownLatch(1);
        for (int i = 0; i < RUNNER_NUMBER; i++) {
            Thread t = new Thread(new Runner((i + 1) + "号运动员", readyLatch, startLatch));
            t.start();
        }
        try {
            readyLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        startLatch.countDown();
        System.out.println("裁判：所有运动员准备完毕，开始...");
    }

    static class Runner implements Runnable {
        private CountDownLatch readyLatch;
        private CountDownLatch startLatch;
        private String name;

        public Runner(String name, CountDownLatch readyLatch, CountDownLatch startLatch) {
            this.name = name;
            this.readyLatch = readyLatch;
            this.startLatch = startLatch;
        }

        public void run() {
            int readyTime = RANDOM.nextInt(1000);
            System.out.println(name + "：我需要" + readyTime + "秒时间准备.");
            try {
                Thread.sleep(readyTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(name + "：我已经准备完毕.");
            readyLatch.countDown();
            try {
                startLatch.await();  // 等待裁判发开始命令
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(name + "：开跑...");
        }
    }
}
