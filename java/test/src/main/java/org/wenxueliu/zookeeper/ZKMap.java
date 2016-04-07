
package org.wenxueliu.zookeeper;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.List;
import java.util.Collections;

import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.framework.api.CreateBuilder;
import org.apache.curator.retry.ExponentialBackoffRetry;

import com.fasterxml.jackson.core.type.TypeReference;

//test
import java.util.Date;
import org.projectfloodlight.openflow.types.DatapathId;
import org.projectfloodlight.openflow.types.OFPort;
import org.projectfloodlight.openflow.types.U64;
import java.io.UnsupportedEncodingException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ZKMap<K, V> {

    private static Logger logger = LoggerFactory.getLogger(ZKMap.class);

    CuratorFramework client;
    CreateBuilder create;
    ISerializer<K> keySerializer;
    ISerializer<V> valueSerializer;
    String name;
    String basePath;

    //TODO check null
    public ZKMap(CuratorFramework client, String name, Class<K> keyClass, Class<V> valueClass) {
        this.client = client;
        this.name = name;
        this.keySerializer = new Serializer<K>(keyClass);
        this.valueSerializer = new Serializer<V>(valueClass);
        this.basePath = "/" + name;
    }

    //TODO check null
    public ZKMap(CuratorFramework client, String name, TypeReference<K> keyType, TypeReference<V> valueType) {
        this.client = client;
        this.name = name;
        this.keySerializer = new Serializer<K>(keyType);
        this.valueSerializer = new Serializer<V>(valueType);
        this.basePath = "/" + name;
    }

    public boolean containsKey(K key) {
        try {
            if (key == null || get(key) == null) {
                return false;
            } else {
                return true;
            }
        } catch (Exception e) {
            System.out.println("containsKey error: " + e.getMessage());
            return false;
        }
    }

    private String computeKey(K key) {
        StringBuilder str = new StringBuilder(this.getBasePath()).append("/");
        str.append(BytesUtils.bytesToString(keySerializer.encode(key)));
        return str.toString();
    }

    public void testKeyConvert(K key) {
        //boolean equals = keySerializer.decode(BytesUtils.StringToBytes(BytesUtils.bytesToString(keySerializer.encode(key)))).equals(key);
        byte[] bytes = keySerializer.encode(key);
        byte[] dstBytes = BytesUtils.StringToBytes(BytesUtils.bytesToString(bytes));
        boolean equals = Arrays.equals(bytes, dstBytes);
        System.out.println("key convert : " + equals);

        equals = key.equals(keySerializer.decode(bytes));
        System.out.println("srcBytes serializer convert : " + equals);

        equals = key.equals(keySerializer.decode(dstBytes));
        System.out.println("dstBytes serializer convert : " + equals);
    }

    public void testValueConvert(V value) {
        //boolean equals = keySerializer.decode(BytesUtils.StringToBytes(BytesUtils.bytesToString(keySerializer.encode(key)))).equals(key);
        byte[] bytes = valueSerializer.encode(value);
        byte[] dstBytes = BytesUtils.StringToBytes(BytesUtils.bytesToString(bytes));
        boolean equals = Arrays.equals(bytes, dstBytes);
        System.out.println("value convert : " + equals);

        equals = value.equals(valueSerializer.decode(bytes));
        System.out.println("srcBytes serializer convert : " + equals);

        equals = value.equals(valueSerializer.decode(dstBytes));
        System.out.println("dstBytes serializer convert : " + equals);
    }

    public Set<K> keySet() {
        HashSet<K> keySet = new HashSet<K>();
        for (String keyStr : this.keyList()) {
            keySet.add(keySerializer.decode(BytesUtils.StringToBytes(keyStr)));
        }
        return keySet;
    }

    private List<String> keyList() {
        try {
            return this.client.getChildren().forPath(this.getBasePath());
        } catch (Exception e) {
            logger.error("keyList error:{}", e.getMessage());
            return Collections.<String>emptyList();
        }
    }

    private String getBasePath() {
        return this.basePath;
    }

    public void put(K key, V value) {
        String keyStr = computeKey(key);
        byte[] valueBytes = valueSerializer.encode(value);
        logger.info(" put key={} value={}", keyStr, valueBytes);
        try {
            if (containsKey(key)) {
                this.client.setData().forPath(keyStr, valueBytes);
            } else {
                this.client.create().creatingParentsIfNeeded().forPath(keyStr, valueBytes);
            }
        } catch (Exception e) {
            logger.error("put error : " + e.getMessage());
        }
    }

    public V get(K key) {
        try {
            logger.trace("get ", computeKey(key));
            byte[] valueBytes = this.client.getData().forPath(computeKey(key));
            if (valueBytes.length == 0) {
                return null;
            } else {
                return valueSerializer.decode(valueBytes);
            }
        } catch (Exception e) {
            logger.error("get error: " + e.getMessage());
            return null;
        }
    }

    public static void test() {
        String connectionString = "127.0.0.1:2181";
        CuratorFramework client = CuratorFrameworkFactory.builder()
            .connectString(connectionString)
            .retryPolicy(new ExponentialBackoffRetry(1000, 3))
            .connectionTimeoutMs(3000)
            .sessionTimeoutMs(3000)
            .build();
        client.start();

        ZKMap<Link, LinkInfo> zkmap = new ZKMap<Link, LinkInfo>(client, "link_linkinfo", Link.class, LinkInfo.class);

        Link link = new Link(DatapathId.of(1L), OFPort.of(2), DatapathId.of(2L), OFPort.of(1), U64.of(100));
        LinkInfo info = new LinkInfo(new Date(), new Date(), null);

        zkmap.testKeyConvert(link);
        zkmap.testValueConvert(info);
        //zkmap.put(link, info);
        //LinkInfo newInfo = zkmap.get(link);
        //System.out.println("origin " + info);
        //System.out.println("new --- " + newInfo);
        //System.out.println("origin equals to new " + newInfo.equals(info));
        //System.out.println("origin link " + link);
        //for (Link l : zkmap.keySet()) {
        //    System.out.println("new link " + l);
        //    System.out.println("new link equals origin" + l.equals(link));
        //}
    }
}
