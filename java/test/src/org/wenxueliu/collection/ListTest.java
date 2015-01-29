package org.wenxueliu.collection;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

/**
* JavaLoopTest
*
* @author www.trinea.cn 2013-10-28
*/
public class ListTest {

    public static void main(String[] args) {
        ListTest listTest = new ListTest();
        listTest.TestList(100);
        listTest.TestList(1000);
        listTest.TestList(10000);
        listTest.TestList(100000);
        listTest.TestList(1000000);
    }

    public void TestList(int size) {
        System.out.print("-------List Size " + size + " -------------\n");
        List<String> arrayList = new ArrayList<String>();
        List<String> linkedList = new LinkedList<String>();
        Random random = new Random();
        for ( int i = 0; i < size; i++) {
            String str = String.valueOf(random.nextInt());
            arrayList.add(str);
            linkedList.add(str);
        }
        System.out.print("------- arrayList -------------\n");
        loopListCompare(arrayList);
        System.out.print("------- LinedList -------------\n");
        loopListCompare(linkedList);
    }

    public void loopListCompare(List<String> list) {
        long startTime, endTime;

        startTime = Calendar.getInstance().getTimeInMillis();
        for (String j : list) {
            // use j
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("for each : " + (endTime - startTime) + " ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        Iterator<String> iterator = list.iterator();
        while(iterator.hasNext()) {
            iterator.next();
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("Iterator(while) : " + (endTime - startTime) + " ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        for (Iterator<String> iter= list.iterator(); iterator.hasNext();) {
            iter.next();
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("Iterator(for) : " + (endTime - startTime) + " ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        for (int j = 0; j < list.size(); j++) {
            list.get(j);
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("for(size()) : " + (endTime - startTime) + " ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        int size = list.size();
        for (int j = 0; j < size; j++) {
            list.get(j);
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("for(size) : " + (endTime - startTime) + " ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        for (int j = list.size() - 1; j >= 0; j--) {
            list.get(j);
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("for reverse(size) : " + (endTime - startTime) + " ms");
    }
}
