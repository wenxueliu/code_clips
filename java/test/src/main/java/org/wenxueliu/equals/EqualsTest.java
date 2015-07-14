package org.wenxueliu.equals;

import java.util.*;
import java.lang.Comparable;

/**
 * @desc equals()的测试程序。
 *
 * @author skywang
 * @emai kuiwu-wang@163.com
 */
public class EqualsTest {

    public static void main(String[] args) {

        // 新建2个相同内容的Person对象，
        // 再用equals比较它们是否相等
        Person1 p11 = new Person1("eee", 100);
        Person1 p12 = new Person1("eee", 100);
        System.out.printf("p11.equals(p12) : %s\n", p11.equals(p12)); // expect false

        // 新建2个相同内容的Person对象，
        // 再用equals比较它们是否相等
        Person2 p21 = new Person2("eee", 100);
        Person2 p22 = new Person2("eee", 100);
        Person2 p23 = new Person2("eee", 100);
        // equals 与 == 的区别
        System.out.printf("p21.equals(p22) : %s\n", p21.equals(p22));
        System.out.printf("p21==p22 : %s\n", p21==p22); 
        
        //equals 与 hashCode
        System.out.printf("p21.equals(p22) : %s; p21(%d) p22(%d)\n", p21.equals(p22), p21.hashCode(), p22.hashCode());
        System.out.printf("p21.equals(p23) : %s; p21(%d) p23(%d)\n", p21.equals(p23), p21.hashCode(), p23.hashCode());

       // 新建2个相同内容的Person对象，
        // 再用equals比较它们是否相等
        Person3 p31 = new Person3("eee", 100);
        Person3 p32 = new Person3("eee", 100);
        Person3 p33 = new Person3("eee", 100);
        // equals 与 == 的区别
        System.out.printf("p31.equals(p32) : %s\n", p31.equals(p32));
        System.out.printf("p31==p32 : %s\n", p31==p32); 
        
        //equals 与 hashCode
        System.out.printf("p31.equals(p32) : %s; p31(%d) p32(%d)\n", p31.equals(p32), p31.hashCode(), p32.hashCode());
        System.out.printf("p31.equals(p33) : %s; p31(%d) p33(%d)\n", p31.equals(p33), p31.hashCode(), p33.hashCode());




        // 新建HashSet对象 
        HashSet set1 = new HashSet();
        set1.add(p11);
        set1.add(p12);


        // 比较p1 和 p2， 并打印它们的hashCode()
        System.out.printf("p11.equals(p12) : %s; p11(%d) p12(%d)\n", p11.equals(p12), p11.hashCode(), p12.hashCode());
        // 打印set
        System.out.printf("set:%s\n", set1);  //输出 2 个, 期望 1 个
     

        // 新建HashSet对象 
        HashSet set2 = new HashSet();
        set2.add(p21);
        set2.add(p22);
        set2.add(p23);

        // 比较p1 和 p2， 并打印它们的hashCode()
        System.out.printf("p21.equals(p22) : %s; p21(%d) p22(%d)\n", p21.equals(p22), p21.hashCode(), p22.hashCode());
        // 打印set
        System.out.printf("set:%s\n", set2); //输出 3 个, 期望 3 个

        // 新建HashSet对象 
        HashSet set3 = new HashSet();
        set3.add(p31);
        set3.add(p32);
        set3.add(p33);

        // 比较p1 和 p2， 并打印它们的hashCode()
        System.out.printf("p31.equals(p32) : %s; p31(%d) p32(%d)\n", p31.equals(p32), p31.hashCode(), p32.hashCode());
        // 打印set
        System.out.printf("set:%s\n", set3); //输出 1 个, 期望 1 个
        //HashMap 对象
    }

    /**
     * @desc Person类。
     */
    static public class Person1 {
        int age;
        String name;

        public Person1(String name, int age) {
            this.name = name;
            this.age = age;
        }

        public String toString() {
            return name + " - " +age;
        }
    }

    /**
     * @desc Person类。
     */
    private static class Person2 {
        int age;
        String name;

        public Person2(String name, int age) {
            this.name = name;
            this.age = age;
        }

        public String toString() {
            return name + " - " +age;
        }

        /** 
         * @desc 覆盖equals方法 
         */  
        @Override
        public boolean equals(Object obj){  
            if(obj == null){  
                return false;  
            }  
              
            //如果是同一个对象返回true，反之返回false  
            if(this == obj){  
                return true;  
            }  
              
            //判断是否类型相同  
            if(this.getClass() != obj.getClass()){  
                return false;  
            }  
              
            Person2 person = (Person2)obj;  
            return name.equals(person.name) && age==person.age;  
        } 
    }

    /**
     * @desc Person类。
     */
    private static class Person3 {
        int age;
        String name;

        public Person3(String name, int age) {
            this.name = name;
            this.age = age;
        }

        public String toString() {
            return name + " - " +age;
        }

        /** 
         * @desc重写hashCode 
         */  
        @Override
        public int hashCode(){  
            int nameHash =  name.toUpperCase().hashCode();
            return nameHash ^ age;
        }

        /** 
         * @desc 覆盖equals方法 
         */  
        @Override
        public boolean equals(Object obj){  
            if(obj == null){  
                return false;  
            }  
              
            //如果是同一个对象返回true，反之返回false  
            if(this == obj){  
                return true;  
            }  
              
            //判断是否类型相同  
            if(this.getClass() != obj.getClass()){  
                return false;  
            }  
              
            Person3 person = (Person3)obj;  
            return name.equals(person.name) && age==person.age;  
        } 
    }
}
