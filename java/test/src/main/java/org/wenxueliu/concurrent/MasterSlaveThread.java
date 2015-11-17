import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

/*
 * 多个线程并发操作修改 map, 其中一个主线程偶尔做修改
 * 摘自 http://codefine.co/%E4%B8%80%E7%A7%8D%E5%B8%B8%E8%A7%81%E7%9A%84%E5%B9%B6%E5%8F%91%E7%BC%96%E7%A8%8B%E5%9C%BA%E6%99%AF%E7%9A%84%E5%A4%84%E7%90%86/?utm_source=rss&utm_medium=rss&utm_campaign=%25e4%25b8%2580%25e7%25a7%258d%25e5%25b8%25b8%25e8%25a7%25
 */

public class MasterSlaveThread {
    /*
    * 用于标志长时间独占表的线程是否正在工作。
    */
    private AtomicBoolean closed = new AtomicBoolean(false);

    /**
     * 用于标志当前的数据表是否正在插入数据。
     */
    private AtomicInteger busy = new AtomicInteger();

    /**
     * 用于存储数据的表格。
     */
    private ConcurrentHashMap<String, String> map = new ConcurrentHashMap<String, String>();

    /**
     * 多个线程会并发调用该接口向
     * map表中插入数据。
     * @param key
     * @param value
     */
    public void addOneRecord(String key, String value) {
        /**
         * 大多数时候，该循环都不会占用性能资源。
         */
        while (closed.get()) {
            waitAMoment();
        }

        if (map != null) {
            /**
             * 用一个同步变量在各个线程中计数。
             */
            busy.getAndIncrement();
            map.put(key, value);
            busy.getAndDecrement();
        }
    }

    /**
     * 后台线程会定期处理map表中的数据。
     */
    public void getAndClearAllRecords() {
        synchronized (this) {
            /**
             * 先将closed标志设置，以便独占该map表；
             * 避免操作过程中被修改。
             */
            closed.set(true);
            while (busy.get() != 0) {
                waitAMoment();
            }
            doSomeThingWithMap(map);
            closed.set(false);
        }
    }

    private void doSomeThingWithMap(ConcurrentHashMap<String, String> map) {
        // TODO Auto-generated method stub
    }

    private void waitAMoment() {
        // TODO Auto-generated method stub
    }
}
