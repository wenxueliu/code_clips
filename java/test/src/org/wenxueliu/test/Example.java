package org.wenxueliu.test;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.lang.IndexOutOfBoundsException;
import java.lang.Exception;
import org.wenxueliu.annotations.AnnotationParsing;
import org.wenxueliu.fileiterators.TextFile;
/**
 *
 */

/**
 * @author wenxueliu
 *
 */
public class Example {

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

    public static void AnnotationTest(){
        AnnotationParsing.parseMethodInfo();
    }

    public static void FileIteratorTest(String filename){

        //使用增强for循环进行文件的读取
        for(String line:new TextFile(filename)){
            System.err.println(line);
        }
    }

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        for(int i=0; i<args.length; i++) {
            System.out.println(args[i]);
        }
		Map <String, ArrayList<String>> map = new HashMap<String, ArrayList<String>>();
		ArrayList<String> array = new ArrayList<String>();
		array.add("abc");
		map.put("2", array);
		map.put("1", new ArrayList<String>(Arrays.asList("cde")));
		System.out.println(map.get("1").toString());
		map.get("1").add("bcd");
		System.out.println(map.get("1").toString());

        System.out.println("------ Exception Test --------");
        Example e = new Example();

        List<String> list = new ArrayList<String>();
        //List<String> list = null;
        try {
            e.TestException(list);
        } catch (Exception ex) {
            System.out.println("IndexOutOfBoundsException exception");
            ex.printStackTrace();
        }

        System.out.println("------ AnnotationTest --------");
        AnnotationTest();

        System.out.println("------ FileIteratorTest --------");

        FileIteratorTest("/home/wenxueliu/dic.txt");
	}

}
