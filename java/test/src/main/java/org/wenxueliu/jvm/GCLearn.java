package org.wenxueliu.jvm;

import java.util.List;
import java.util.ArrayList;

public class GCLearn {
    static class OOMObject {
        public byte[] placeholder = new byte[64*1024];
    }

    public static void fillHeap(int num) throws InterruptedException {
        List<OOMObject> list = new ArrayList<OOMObject>();
        for (int i = 0; i < num; i++) {
            Thread.sleep(50);
            list.add(new OOMObject());
        }
        System.gc();
    }

    public static void main(String[] args) throws Exception {
        try {
            fillHeap(1000);
        } catch (InterruptedException e) {
            System.out.println("Ctrl-C is pressed");
        } catch (Exception e) {
            System.out.println("unknow Exception");
        } finally {
            System.out.println("process exit");
        }
    }
}
