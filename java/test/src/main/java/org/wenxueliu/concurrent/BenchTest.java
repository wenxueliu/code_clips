package org.wenxueliu.concurrent;

import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import com.udpwork.ssdb.SSDB;

@SuppressWarnings("all") public class BenchTest {

    final String poolId = "10.1.2.100:80";
    final String bkId   = "10.1.2.11:80";
    final byte protocol = 6;
    final int  poolIP   = IPv4Address.strToInt("10.1.2.100");
    final int  poolPort = 8000;
    private List<String> clientKeyList = new ArrayList<String>();
    private Map<String, BkInfo> clientServerMap = new ConcurrentHashMap<String, BkInfo>();
    private BkInfo bkInfo = new BkInfo(poolId, bkId);


    public void genKeys(String beginIP, String endIP, int beginPort, int endPort) {
        try {
            int bIP = IPv4Address.strToInt(beginIP);
            int eIP = IPv4Address.strToInt(endIP);
            for (int ip = bIP; ip <= eIP; ip++) {
                for (int port = beginPort; port <= endPort; port++) {
                    clientKeyList.add(new StringBuilder().append(ip).append("-")
                            .append(port).append("-")
                            .append(protocol).append("-")
                            .append(poolIP).append("-")
                            .append(poolPort).append("-")
                            .toString());
                }
            }
        } catch (IllegalArgumentException e) {
            System.out.println("add host to monitor error :" + e.getMessage());
        }
    }

    public int getKeySize() {
        return clientKeyList.size();
    }

    public void testWithMap() {
        long start_time = System.currentTimeMillis();
        for (String clientKey : clientKeyList) {
            clientServerMap.put(clientKey, bkInfo);
        }
        long end_time = System.currentTimeMillis();
        long take_time = end_time - start_time;
        System.out.println("testWithMap take time " + take_time);
    }

    public void testWithSSDB() {
        long start_time = 0;
        long end_time = 0;
        SSDB ssdb = null;
        try {
            ssdb = new SSDB("127.0.0.1", 8888,5000);
            start_time = System.currentTimeMillis();
            for (String clientKey : clientKeyList) {
                ssdb.set(clientKey, poolId+"-" + bkId);  //这里的k确保每次都不同，v 平均 1M大小
            }
            end_time = System.currentTimeMillis();
            ssdb.close();
        } catch (Exception e) {
            if(ssdb != null) {
                ssdb.close();
            }
	    } finally {
            if(ssdb != null) {
                ssdb.close();
            }
        }
        if (ssdb != null) {
            ssdb.close();
        }
        long take_time = end_time - start_time;
        System.out.println("testWithSSDB take time " + take_time);
    }

    public void testWithRedis() {
    }

    public static int main(String[] args) {
        BenchTest bench = new BenchTest();
        bench.genKeys("10.1.2.1", "10.1.2.200", 1, 60000);
        bench.testWithMap();
        bench.testWithSSDB();
        bench.testWithRedis();
        return 0;
    }
}
