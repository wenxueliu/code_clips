package org.wenxueliu.classloader;
import java.util.ServiceLoader;
public class MyClassLoader {

    public static void defaultLoader() {
        System.out.println("sun.boot.class.path:" + System.getProperty("sun.boot.class.path"));
        System.out.println("java.ext.dirs:" + System.getProperty("java.ext.dirs"));
        System.out.println("java.class.path:" + System.getProperty("java.class.path"));
        ClassLoader cl = Thread.currentThread().getContextClassLoader();//ClassLoader.getSystemClassLoader()
        System.out.println("getContextClassLoader:" + cl.toString());
        System.out.println("getContextClassLoader.parent:" +cl.getParent().toString());
        System.out.println("getContextClassLoader.parent2:" +cl.getParent().getParent());
    }

    public static void serviceLoader() {
        ServiceLoader<IService> serviceLoader  = ServiceLoader.load(IService.class);
        for (IService service : serviceLoader) {
            System.out.println(service.getScheme()+"="+service.sayHello());
        }
    }
}

