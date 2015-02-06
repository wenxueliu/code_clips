package org.wenxueliu.concurrent;

import java.util.ArrayList;
import java.util.Iterator;

public class InsertData {
    private ArrayList<Integer> arrayList = new ArrayList<Integer>();

    public void insert1(Thread thread){
        for(int i=0;i<5;i++){
            System.out.println(thread.getName()+"在插入数据"+i);
            arrayList.add(i);
        }
    }

    public synchronized void insert2(Thread thread){
        for(int i=0;i<5;i++){
            System.out.println(thread.getName()+"在插入数据"+i);
            arrayList.add(i);
        }
    }

    public synchronized static void insert3(){
        for(int i=0;i<5;i++) {
            System.out.println("insert3"+"在插入数据"+i);
        }
    }

    public void get1(Thread thread) {
        //for(Integer i: arrayList){
        //    System.out.println(thread.getName()+"在获取数据"+i);
        //}
        Iterator<Integer> iter = arrayList.iterator();
        while(iter.hasNext()) {
            System.out.println(thread.getName()+"在获取数据"+ iter.next());
        }
    }

    public synchronized void get2(Thread thread) {
        for(Integer i: arrayList){
            System.out.println(thread.getName()+"在获取数据"+i);
        }
    }

    public synchronized static void get3() {
        for (int i = 0; i < 5; i++){
            System.out.println("get3"+"在获取数据"+i);
        }
    }
}
