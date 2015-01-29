package org.wenxueliu.collection;

import java.text.DecimalFormat;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

/**
 * JavaLoopTest
 * 
 * @author www.trinea.cn 2013-10-28
 */
public class HashMapTest {

    public static void main(String[] args) {
        System.out.print("compare loop performance of HashMap");
        HashMapTest hash = new HashMapTest();
        hash.loopMapCompare(hash.getHashMaps(10000));
    }

    public Map<String, String> getHashMaps(int size) {
        System.out.println("size of HashMap : " + size);
        Map<String, String> map = new HashMap<String, String>();
        for (int j = 0; j < size; j++) {
            String s = String.valueOf(j);
            String key = s + s + s + s;
            map.put(key, s);
        }
        return map;
    }

    public void loopMapCompare(Map<String, String> hashMap) {
        long startTime, endTime;

        Map<String, String> map = hashMap;

        startTime = Calendar.getInstance().getTimeInMillis();
        for (Entry<String, String> entry : map.entrySet()) {
            entry.getKey();
            entry.getValue();
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("by EntrySet : " + (endTime - startTime) + "ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        Iterator<Map.Entry<String, String>> iterator = map.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<String, String> entry = iterator.next();
            entry.getKey();
            entry.getValue();
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("by EntrySet Iterator : " + (endTime - startTime) + "ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        for (String key : map.keySet()) {
            map.get(key);
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("by KeySet: " + (endTime - startTime) + "ms");

        startTime = Calendar.getInstance().getTimeInMillis();
        Set<Entry<String, String>> entrySet = map.entrySet();
        for (Entry<String, String> entry : entrySet) {
            entry.getKey();
            entry.getValue();
        }
        endTime = Calendar.getInstance().getTimeInMillis();
        System.out.println("by EntrySet v2 : " + (endTime - startTime) + "ms");
    }

}
