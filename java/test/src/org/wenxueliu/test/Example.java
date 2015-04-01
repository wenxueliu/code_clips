package org.wenxueliu.test;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Arrays;
import java.util.HashMap; import java.util.HashSet;
import java.util.UUID;
import java.util.Map;
import java.lang.IndexOutOfBoundsException;
import java.lang.Exception;
import java.lang.ref.WeakReference;
import java.util.concurrent.TimeUnit;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import org.wenxueliu.annotations.AnnotationParsing;
import org.wenxueliu.fileiterators.TextFile;
import org.wenxueliu.interclass.EnumTest;
import org.wenxueliu.enums.StatusExample1;
import org.wenxueliu.enums.StatusExample2;
import org.wenxueliu.reference.ReferenceExample;
import org.wenxueliu.reference.ReferenceExample.Host;
import org.wenxueliu.collection.HashMapTest;
import org.wenxueliu.collection.ListTest;
import org.wenxueliu.concurrent.ThreadSyn;
import org.wenxueliu.concurrent.InsertData;
import org.wenxueliu.concurrent.ThreadLock;
import org.wenxueliu.concurrent.LinkedBlockingQueueExample;
import org.wenxueliu.concurrent.ThreadTest;
import org.wenxueliu.concurrent.MyThreadLocal;
import org.wenxueliu.demotask.DemoExecutor;
/**
 *
 */

/**
 * @author wenxueliu
 *
 */
public class Example {

    void ThreadLocalTest() {
        MyThreadLocal.LocalTest();
    }
    void ThreadBasicTest() {
        ThreadTest test = new ThreadTest();
        test.testSelf();
    }
    void LinkedBlockingQueueExampleTest() {
        LinkedBlockingQueueExample.example();
    }

    void DemoExecutorTest() {
        DemoExecutor de = new DemoExecutor();
        de.example();
    }
    void ThreadLockTest() {
        final ThreadLock tl = new ThreadLock();
        System.out.println("------ ThreadLockTest --------");
        tl.testLocalLock();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("-- testGlobalLock --");
        tl.testGlobalLock();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }

        System.out.println("-- testTryLock --");
        tl.testTryLock();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("-- testInterruptLock --");
        tl.testInterruptLock();
        try {
            TimeUnit.SECONDS.sleep(5);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("-- testSynchronizedGet --");
        tl.testSynchronizedGet();
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
        System.out.println("-- testReentrantReadWriteLockGet --");
        tl.testReentrantReadWriteLockGet();
    }

    void ThreadSynTest() {
        ThreadSyn ts = new ThreadSyn();
        System.out.println("------ ThreadSynTest --------");
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
		System.out.println("------------ CountDownLatchTest Test ------------");
		ts.CountDownLatchTest();
		try {
            TimeUnit.SECONDS.sleep(3);
        } catch(InterruptedException e) {
            //No-op
        }
		System.out.println("------------  CyclicBarrierTest ------------");
		ts.CyclicBarrierTest();
    }

    void listLoopTest() {
        ListTest listTest = new ListTest();
        System.out.println("------ listLoopTest --------");
        listTest.TestList(100);
        listTest.TestList(1000);
        listTest.TestList(10000);
        listTest.TestList(100000);
        listTest.TestList(1000000);
    }

    void ListHashMap() {
        HashMapTest hash = new HashMapTest();
        System.out.println("------ HashMapTravelTest --------");
        hash.loopMapCompare(hash.getHashMaps(100));
        hash.loopMapCompare(hash.getHashMaps(1000));
        hash.loopMapCompare(hash.getHashMaps(10000));
        hash.loopMapCompare(hash.getHashMaps(100000));
        hash.loopMapCompare(hash.getHashMaps(1000000));
    }
    void StringCompare() {
        System.out.println("------ StringCompare --------");
        String str = new String("192.168.1.24:8080");
        String str2 = new String("192.168.1.24:8080");
        System.out.println("str:" + str);
        System.out.println("str2:" + str2);
        System.out.println("str.equals(str2) : " + str.equals(str2));
        System.out.println("str==str2 : " + (str == str2));
    }
    void ArrayListToString() {
        System.out.println("------ ArrayListToString --------");
        ArrayList<String> str = new ArrayList<String>();
        str.add("add");
        str.add("add");
        str.add("add");
        System.out.println(str.toString());
    }
    void ReferenceTest() {
        System.out.println("------ ReferenceTest --------");
        Host hs = new ReferenceExample.Host("10.0.0.1", "00:00:00:00:00");
        Host hs1 = new ReferenceExample.Host("100.0.0.1", "00:00:00:00:00");
        int id = 1;
        String name = "aaa";
        ReferenceExample re = new ReferenceExample(id, name,hs);

        HashMap<String, ReferenceExample> hm = new HashMap<String, ReferenceExample>();
        hm.put(String.valueOf(re.getId()), re);
        //ReferenceExample re1 = hm.get("1");
        //re1.setName("bbb");
        //re1.setHost(hs1);
        WeakReference<ReferenceExample> re1 = new WeakReference<ReferenceExample>(hm.get("1"));
        System.out.println("before change:" + re1.get().toString());
        System.out.println("before change:" + hm.get("1").toString());
        re1.get().setName("bbb");
        re1.get().setHost(hs1);
        hs1.setIp("100.0.0.2");
        System.out.println("after change:" + hm.get("1").toString());
        hm.remove("1");
        System.gc();
        System.out.println("after remove:" + re1.get().toString());

    }

    void HashSetVsArrayList() {
        System.out.println("------ HashSetVsArrayList --------");
        HashSet<String> hashSet = new HashSet<String>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < 900000; i++) {
            hashSet.add(String.valueOf(i));
        }

        System.out.println("Insert HashSet Time: " + (System.currentTimeMillis() - start));


        ArrayList<String> arrayList = new ArrayList<String>();

        start = System.currentTimeMillis();

        for (int i = 0; i < 900000; i++) {
            arrayList.add(String.valueOf(i));
        }
        System.out.println("Insert ArrayList Time: " + (System.currentTimeMillis() - start));
    }



    public void HashMapTest() {
        System.out.println("------ HashMapTest --------");
        HashMap<String, String> strMap = new HashMap<String, String>();
        strMap.put("a", "1");
        System.out.println(strMap.put("a", "2"));
        System.out.println(strMap.put("b", "2"));
        System.out.println(strMap.put(null, "2"));
        System.out.println(strMap.put(null, null));

        String v = strMap.get("a");
        v = "3";
        System.out.println("modify test: " + strMap.get("a"));


        System.out.println(strMap.remove("a"));
        System.out.println(strMap.remove("c"));
        System.out.println(strMap.remove(null));
    }
    //public void TestException() throws IndexOutOfBoundsException
    public void TestException(List<String> list)
    {
        System.out.println("travel list");
        for (String s: list){
            System.out.println(s);
        }
        //list.add("aaaaaaaaaa");
        //list.remove(list.size() - 1);
    }

    public void TestThrow(String str){
        if (str == "") {
            throw new java.lang.IllegalArgumentException("can not be empty");
        }
    }

    public void ThrowTest() {
        System.out.println("------ testThrow2 --------");
        try {
            TestThrow2();
        } catch (IllegalArgumentException ex) {
            System.out.println(" error : " + ex.getMessage());
        }

        System.out.println("------ testThrow3 --------");
        try {
            TestThrow3();
        } catch (IllegalArgumentException ex) {
            System.out.println(" error : " + ex.getMessage());
        }
    }

    public void TestThrow2() {
        TestThrow("");
    }

    public void TestThrow3() throws IndexOutOfBoundsException {
        TestThrow("");
    }

    public void AnnotationTest(){
        System.out.println("------ AnnotationTest --------");
        AnnotationParsing.parseMethodInfo();
    }

    public void FileIteratorTest(String filename){
        System.out.println("------ FileIteratorTest --------");
        //使用增强for循环进行文件的读取
        for(String line:new TextFile(filename)){
            System.err.println(line);
        }
    }

    public void interclassTest(){
        System.out.println("------ interclassTest --------");
        EnumTest enumTest = new EnumTest(EnumTest.WeekDay.valueOf(0));
        System.out.println("today : " + enumTest.toString());
        try {
            enumTest.setDay(EnumTest.WeekDay.valueOf(2));
        } catch (IllegalArgumentException ex){
            System.out.println("IllegalArgumentException:" + ex.getMessage());
        }
        System.out.println("today : " + enumTest.getDay());
        //System.out.println("today name: " + enumTest.name());
        System.out.println("today toStrin: " + enumTest.toString());
        EnumTest.info();
    }

    void ExceptionTest() {
        System.out.println("------ Exception Test --------");

        List<String> list = new ArrayList<String>();
        //List<String> list = null;
        try {
            TestException(list);
        } catch (Exception ex) {
            System.out.println("IndexOutOfBoundsException exception");
            ex.printStackTrace();
        }
    }

    void RefCopyTest() {
		Map <String, ArrayList<String>> map = new HashMap<String, ArrayList<String>>();
		ArrayList<String> array = new ArrayList<String>();
		array.add("abc");
		map.put("2", array);
		map.put("1", new ArrayList<String>(Arrays.asList("cde")));
		System.out.println(map.get("1").toString());
		map.get("1").add("bcd");
		System.out.println(map.get("1").toString());
    }

    //public void StatusTest(){
    //    System.out.println("------ StatusTest  --------");
    //    StatusExample1 st = new StatusExample1(StatusExample1.Process.STATUS_OPEN);

    //    StatusExample1.Status s1 = StatusExample1.Status(1);
    //    StatusExample1.Status s2 = StatusExample1.Status(6);
    //    System.out.println(s1.status());
    //    System.out.println(s2.status());

    //    StatusExample1.Status s3 = StatusExample1.Status(1, "hello");
    //    StatusExample1.Status s4 = StatusExample1.Status(6, "enum");
    //    System.out.println(s3.status());
    //    System.out.println(s4.status());
    //}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
        Example e = new Example();
        //e.RefCopyTest();
        //e.AnnotationTest();
        //e.FileIteratorTest("/home/wenxueliu/dic.txt");
        //e.interclassTest();
        //e.ThrowTest();
        //e.HashMapTest();
        //e.HashSetVsArrayList();
        //e.ReferenceTest();
        //e.ArrayListToString() ;
        //e.StringCompare();
        //e.ListHashMap();
        //e.listLoopTest();
        //e.ThreadSynTest();
        //e.ThreadLockTest();
        //e.DemoExecutorTest();
        //e.LinkedBlockingQueueExampleTest();
        //e.ThreadBasicTest();
        //e.testNode();
        e.ThreadLocalTest();
	}

    public void testNode() {
        ArrayList<Integer> ips = new ArrayList<Integer>();
        try {
            System.out.println(ConvertIPStrToInt("127.0.0.1") + "==" + 2130706433);
            System.out.println(ConvertIPIntToStr(2130706433) + "==" + "127.0.0.1");
            System.out.println((long)(ConvertIPStrToInt("255.255.255.255")&0xffffffff) );//+ "==" + 4294967295);

            ips.add(ConvertIPStrToInt("192.168.1.1"));
            ips.add(ConvertIPStrToInt("192.168.1.2"));
            ips.add(ConvertIPStrToInt("192.168.1.3"));
            ips.add(ConvertIPStrToInt("192.168.1.4"));
            ips.add(ConvertIPStrToInt("192.168.1.5"));
            ips.add(ConvertIPStrToInt("192.168.1.6"));
            ips.add(ConvertIPStrToInt("192.168.1.7"));
            ips.add(ConvertIPStrToInt("192.168.1.8"));
            ips.add(ConvertIPStrToInt("192.168.1.9"));
            ips.add(ConvertIPStrToInt("192.168.1.10"));
            ips.add(ConvertIPStrToInt("192.168.1.11"));
            ips.add(ConvertIPStrToInt("192.168.1.12"));
            ips.add(ConvertIPStrToInt("192.168.1.13"));
            ips.add(ConvertIPStrToInt("192.168.1.14"));
            ips.add(ConvertIPStrToInt("192.168.1.15"));
            ips.add(ConvertIPStrToInt("192.168.1.16"));
            //ips.add(ConvertIPStrToInt("192.168.1.8"));
        } catch (UnknownHostException e){
            e.printStackTrace();
        }

        //ips.add(19216812);
        //ips.add(19216813);
        Node root = new Node(2);
        Node1 root1 = new Node1(2, 32);
        for (Integer ip: ips) {
            String str = Integer.toBinaryString(ip);
            String zeroStr = "00000000000000000000000000000000";
            str = zeroStr.substring(0, (32-str.length())) + str;
            System.out.println(str + " from " + ip);
            root = insert(root, str, 0);
            root1 = insert(root1, str, 0, 32);
        }

        //HashMap<Integer, ArrayList<Integer>> tree = new HashMap<Integer, ArrayList<Integer>>();
        //dumpNodes(null, 0, tree);
        HashMap<Integer, ArrayList<String>> tree1 = new HashMap<Integer, ArrayList<String>>();
        dumpNodes(root1, 0, tree1);

        //char[] path = new char[32];
        //preOrderTraverse(root1.leftChild, path, '0', 0);
        //preOrderTraverse(root1.rightChild, path, '1', 0);

        String[] path = new String[32];
        preOrderTraverse(root1.leftChild, path, "0", 0);
        preOrderTraverse(root1.rightChild, path, "1", 0);

    }


    public class Node1 {
        private Node1 leftChild;
        private Node1 rightChild;
        private int value;
        private int index;

        Node1 (int data, int index) {
            this.leftChild = null;
            this.rightChild = null;
            this.value = data;
            this.index = index;
        }
    }

    public Node1 insert(Node1 node, String str, int level, int index) {
        if (node == null) {
            node = new Node1(0, index);
        }
        if (level == 32) {
            node.value = 1;
            return node;
        }

        //System.out.println("node.value" + node.value + "node.index:" + node.index);
        if (str.charAt(level) == '0') {
            node.leftChild = insert(node.leftChild, str, level + 1, index - 1);
        } else {
            node.rightChild= insert(node.rightChild, str, level + 1, index + 1);
        }

        if (node.leftChild != null && node.leftChild.value == 1 && node.rightChild != null && node.rightChild.value == 1) {
            node.value = 1;
        }
        return node;
    }

    public void traveNodes(Node1 node, int level, HashMap<Integer, ArrayList<String>> tree) {
        if (node == null) {
            return;
        } else {
            if (tree.get(level) == null) {
                tree.put(level, new ArrayList<String>());
                tree.get(level).add(node.value + ":" + node.index);
            } else {
                tree.get(level).add(node.value + ":" + node.index);
            }
            traveNodes(node.leftChild, level+1, tree);
            traveNodes(node.rightChild, level+1, tree);
        }
    }

    public void dumpNodes(Node1 node, int level, HashMap<Integer, ArrayList<String>> tree) {
        traveNodes(node, level, tree);
        String padding = String.format("%0" + 64 + "d", 0).replace("0"," ");
        for (int dep = 0; dep < 33; dep++) {
            StringBuilder line = new StringBuilder();
            line.append(padding);
            //System.out.println("b" + line.toString() + "end");
            for (String str : tree.get(dep)) {
                String value = str.split(":")[0];
                String index = str.split(":")[1];
                //System.out.println("value:" + value + "index" + index);
                line.setCharAt(Integer.valueOf(index),value.charAt(0));
            }
            System.out.println(line);
        }
    }

    private String binToIP(String binIP) {
        //StringBuilder strIP = new StringBuilder();
        //strIP.append(Integer.valueOf(binIP.substring(0, 8), 2) & 0xff).append(".")
        //     .append(Integer.valueOf(binIP.substring(8, 16), 2) & 0xff).append(".")
        //     .append(Integer.valueOf(binIP.substring(16, 24), 2) & 0xff).append(".")
        //     .append(Integer.valueOf(binIP.substring(24, 32), 2) & 0xff);
        //return strIP.toString();
        String strIP = new String();
        //System.out.println(binIP + Integer.valueOf("000001001", 2));
        strIP = String.valueOf(Integer.valueOf(binIP.substring(0, 8), 2) & 0xff) + ".";
        strIP += String.valueOf(Integer.valueOf(binIP.substring(8, 16), 2) & 0xff) + ".";
        strIP += String.valueOf(Integer.valueOf(binIP.substring(16, 24), 2) & 0xff) + ".";
        strIP += String.valueOf(Integer.valueOf(binIP.substring(24, 32), 2) & 0xff);
        return strIP;
    }

    public void preOrderTraverse(Node1 node,char[] path, char value, int level) {
        if (node == null || level == 32) {
            return;
        }
        path[level] = value;
        if (node.value == 1) {
            //String padding = String.format("%0" + 32 + "d", 0).replace("0","0");
            String zeroStr="00000000000000000000000000000000";
            //System.out.println(String.valueOf(path) + level);
            //System.out.println(zeroStr.substring(0, 32 - level));
            String ip = binToIP(String.valueOf(Arrays.copyOf(path, level)) + zeroStr.substring(0, 31 - level));
            System.out.println(ip);
            return;
        }
        preOrderTraverse(node.leftChild, path, '0', level + 1);
        preOrderTraverse(node.rightChild, path, '1', level + 1);
    }

    public void preOrderTraverse(Node1 node,String[] path, String value, int level) {
        if (node == null || level == 32) {
            return;
        }
        path[level] = value;
        //System.out.println("path[level]" + value + "level " + level);
        if (node.value == 1) {
            //String padding = String.format("%0" + 32 + "d", 0).replace("0","0");
            String t = "";
            for (int i = 0; i <= level; i++) {
                t += path[i];
            }
            String zeroStr="00000000000000000000000000000000";
            //System.out.println(String.valueOf(path) + level);
            System.out.println("level " + level + " " + t);
            //System.out.println(zeroStr.substring(0, 32 - level));
            String ip = binToIP(t + zeroStr.substring(0, 31 - level)) + "/" + (level+1);
            System.out.println(ip);
            return;
        }
        preOrderTraverse(node.leftChild, path, "0", level + 1);
        preOrderTraverse(node.rightChild, path, "1", level + 1);
    }




    public class Node {
        Node leftChild;
        Node rightChild;
        int value;

        Node (int data) {
            leftChild = null;
            rightChild = null;
            value = data;
        }
    }

    public Node insert(Node node, String str, int level) {
        if (node == null) {
            node = new Node(0);
        }
        if (level == 32) {
            node.value = 1;
            return node;
        }

        if (str.charAt(level) == '0') {
            node.leftChild = insert(node.leftChild, str, level+1);
        } else {
            node.rightChild= insert(node.rightChild, str, level+1);
        }

        if (node.leftChild != null && node.leftChild.value == 1 && node.rightChild != null && node.rightChild.value == 1) {
            node.value = 1;
        }
        return node;
    }

    public void traveNodes(Node node, int level, HashMap<Integer, ArrayList<Integer>> tree) {
        if (node == null) {
            if (tree.get(level) == null) {
                tree.put(level, new ArrayList<Integer>());
                tree.get(level).add(2);
            } else {
                tree.get(level).add(2);
            }
            return;
        } else {
            if (tree.get(level) == null) {
                tree.put(level, new ArrayList<Integer>());
                tree.get(level).add(node.value);
            } else {
                tree.get(level).add(node.value);
            }
            traveNodes(node.leftChild, level+1, tree);
            traveNodes(node.rightChild, level+1, tree);
        }
    }

    public void dumpNodes(Node node, int level, HashMap<Integer, ArrayList<Integer>> tree) {
        traveNodesV1(node, level, tree);
        int depth = 7;
        int len = 1 << depth;
        String padding = String.format("%0" + len + "d", 0).replace("0"," ");
        System.out.println("str:" + padding.length() + "end");
        for (int dep = 0; dep < depth - 1; dep++) {
            StringBuilder line = new StringBuilder();
            line.append(padding.substring(0, (1 << (depth - dep - 2)) - 1));
            for (Integer i:tree.get(dep)) {
                line.append(i);
                line.append(padding.substring(0, (1 << (depth - dep - 1)) - 1));
            }
            System.out.println(line);
        }
    }

    public void traveNodesV1(Node node, int level, HashMap<Integer, ArrayList<Integer>> tree) {
        if (node == null) {
            if (tree.get(level) == null) {
                tree.put(level, new ArrayList<Integer>());
                tree.get(level).add(2);
            } else {
                tree.get(level).add(2);
            }
            //return;
            if (level == 5) {
                return;
            }
            traveNodes(null, level+1, tree);
            traveNodes(null, level+1, tree);
        } else {
            if (tree.get(level) == null) {
                tree.put(level, new ArrayList<Integer>());
                tree.get(level).add(node.value);
            } else {
                tree.get(level).add(node.value);
            }
            if (level == 5) {
                return;
            }
            traveNodes(node.leftChild, level+1, tree);
            traveNodes(node.rightChild, level+1, tree);
        }
    }

    private int pack(byte[] bytes) {
      int val = 0;
      for (int i = 0; i < bytes.length; i++) {
          val <<= 8;
          val |= bytes[i] & 0xff;
        }
      return val;
    }

    // dottedStrin as 127.0.0.1
    public int ConvertIPStrToInt(String dottedString) throws UnknownHostException {
        //return pack(InetAddress.getByName(dottedString).getAddress());
        return ByteBuffer.wrap(InetAddress.getByName(dottedString).getAddress()).getInt();
    }

    private byte[] unpack(int bytes) {
      return new byte[] {
          (byte)((bytes >>> 24) & 0xff),
          (byte)((bytes >>> 16) & 0xff),
          (byte)((bytes >>>  8) & 0xff),
          (byte)((bytes       ) & 0xff)
        };
    }

    public String ConvertIPIntToStr(int packedBytes) throws UnknownHostException {
        //return InetAddress.getByAddress(unpack(packedBytes)).getHostAddress();
        return InetAddress.getByName(String.valueOf(packedBytes)).getHostAddress();
    }
}
