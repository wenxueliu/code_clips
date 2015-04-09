package org.wenxueliu.test;

public class PerformanceTest {
	private static long i;
	private volatile static long vt;
	private static final int TEST_SIZE = 10000000;

	public static void testVar() {
		long time = System.nanoTime();
		for (int n = 0; n < TEST_SIZE; n++)
			vt = System.currentTimeMillis();
		System.out.println("SystemTime: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			i = System.currentTimeMillis();
		System.out.println("SystemTime: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			synchronized (PerformanceTest.class) {
			}
		System.out.println("synchronized block: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			vt++;
		System.out.println("volatile var inc: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			vt = i;
		System.out.println("volatile var set: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			i = vt;
		System.out.println("var set: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
			i++;
		System.out.println("var inc: " + (System.nanoTime()-time));
		for (int n = 0; n < TEST_SIZE; n++)
 			i = n;
		System.out.println("var set: " + (System.nanoTime()-time));
 	}
}
