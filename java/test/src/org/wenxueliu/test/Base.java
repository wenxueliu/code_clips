package org.wenxueliu.test;
import java.util.Map;
import java.util.HashMap;
import java.lang.management.ManagementFactory;
import java.lang.management.RuntimeMXBean;


public class Base {
    public long getSystemStartTime() {
            RuntimeMXBean rb = ManagementFactory.getRuntimeMXBean();
            return rb.getStartTime();
        }

    public Map<String, Long> getMemory() {
            Map<String, Long> m = new HashMap<String, Long>();
            Runtime runtime = Runtime.getRuntime();
            m.put("total", runtime.totalMemory());
            m.put("free", runtime.freeMemory());
            return m;
        }

    public Long getUptime() {
            RuntimeMXBean rb = ManagementFactory.getRuntimeMXBean();
            return rb.getUptime();
        }
}
