package org.wenxueliu.sizeof;

import java.lang.instrument.Instrumentation;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.IdentityHashMap;
import java.util.Map;
import java.util.Stack;

public class ObjectSize {
    private static Instrumentation inst;

    public static void premain(String agentArgs, Instrumentation instP) {
        inst = instP;
    }

    public static long sizeOf(Object obj) {
        return inst.getObjectSize(obj);
    }

    public static long deepSizeOf(Object obj) {
        Map<Object, Object> visited = new IdentityHashMap<Object, Object>();
        Stack<Object> stack = new Stack<Object>();
        long result = internalSizeOf(obj, stack, visited);
        while (!stack.isEmpty()) {
            result += internalSizeOf(stack.pop(), stack, visited);
        }
        visited.clear();
        return result;
    }

    private static boolean needSkipObject(Object obj, Map<Object, Object> visited) {
        if (obj instanceof String) {
            if (obj == ((String) obj).intern()) {
                return true;
            }
        }
        return (obj == null) || visited.containsKey(obj);
    }

    private static long internalSizeOf(Object obj, Stack<Object> stack, Map<Object, Object> visited) {
        if (needSkipObject(obj, visited)) {
            return 0;
        }
        visited.put(obj, null);
        long result = 0;
        result += sizeOf(obj);
        Class <?>clazz = obj.getClass();
        if (clazz.isArray()) {
            //如果 primitive type array, Class 的 name 为 2 位
            if (clazz.getName().length() != 2) {
                int length = Array.getLength(obj);
                for (int i = 0; i < length; i++) {
                    stack.add(Array.get(obj, i));
                }
            }
            return result;
        }
        return getNodeSize(clazz, result, obj, stack);
    }

    private static long getNodeSize(Class <?>clazz, long result, Object obj, Stack<Object> stack) {
        while (clazz != null) {
            Field[] fields = clazz.getDeclaredFields();
            for (Field field : fields) {
                //排除静态属性
                if (!Modifier.isStatic(field.getModifiers())) {
                    //由于基本关键字在调用 java 默认提供的方法就已经计算过了, 故这里不再计算
                    if (field.getType().isPrimitive()) {
                        continue;
                    } else {
                        field.setAccessible(true);
                        try {
                            Object objectToAdd = field.get(obj);
                            if (objectToAdd != null) {
                                stack.add(objectToAdd);
                            }
                        } catch (IllegalAccessException ex) {
                            assert false;
                        }
                    }
                }
            }
            clazz = clazz.getSuperclass();
        }
        return result;
    }

    static public void testSize() {
        System.out.println("Normal Integer(1) size : " + ObjectSize.sizeOf(new Integer(1)));
        System.out.println("Normal String() size : " + ObjectSize.sizeOf(new String()));
        System.out.println("Normal char[1] size : " + ObjectSize.sizeOf(new char[1]));

        System.out.println("Deep Integer(1) size : " + ObjectSize.deepSizeOf(new Integer(1)));
        System.out.println("Deep String() size : " + ObjectSize.deepSizeOf(new String()));
        System.out.println("Deep char[1] size : " + ObjectSize.deepSizeOf(new char[1]));
    }
}
