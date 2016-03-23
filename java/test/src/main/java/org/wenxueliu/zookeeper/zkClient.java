/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.wenxueliu.zookeeper;

import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;
import org.apache.curator.RetryPolicy;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.api.CreateBuilder;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.retry.ExponentialBackoffRetry;
//import org.apache.zookeeper.data.Stat;

public class zkClient {

    final String clusterPath = "/a5/cluster/";
    final String nodePath  = "/a5/cluster/node/";

    CuratorFramework client = null;
    CreateBuilder create = null;

    public zkClient(CuratorFramework client) {
        client = client;
        create = client.create();
    }

    public void close() {
        client.close();
    }

    void createCluster(int clusterID) {
        try {
            client.create().creatingParentsIfNeeded().forPath(clusterPath + clusterID);
        } catch (Exception e) {
        }
    }

    void createNode(int clusterID) {
        try {
            client.create().creatingParentsIfNeeded().forPath(nodePath + clusterID);
        } catch (Exception e) {
        }
    }

    void main(String[] args) {
        CuratorFramework client = null;
        try {
            String connectionString = "192.168.0.120:2181";

            HashMap<String, String> topoMap = new HashMap<String, String>();

            String cluster1     = "/a5/cluster/1";
            topoMap.put(cluster1 + "/name", "c1");
            topoMap.put(cluster1 + "/type", "cluster");

            String c1a1         = cluster1 + "/a5/1";
            topoMap.put(c1a1 + "/name", "c1a1");
            topoMap.put(c1a1 + "/controller", "10.1.3.1:6633");
            topoMap.put(c1a1 + "/ovs", "10.1.3.1");

            String c1a2         = cluster1 + "/a5/2";
            topoMap.put(c1a2 + "/name", "c1a2");
            topoMap.put(c1a2 + "/controller", "10.1.2.2:6633");
            topoMap.put(c1a2 + "/ovs", "10.1.2.2");

            String c1p1         = cluster1 + "/pools" + "/1";
            topoMap.put(c1p1 + "/name", "c1p1");
            topoMap.put(c1p1 + "/mac", "00:00:00:00:00;01");
            topoMap.put(c1p1 + "/ip", "10.1.2.13");
            topoMap.put(c1p1 + "/port", "8000");
            topoMap.put(c1p1 + "/protocol", "tcp");
            topoMap.put(c1p1 + "/lbmethod", "rr");
            topoMap.put(c1p1 + "/type", "follower");
            String c1p1servers = c1p1 + "/servers";
            topoMap.put(c1p1servers + "/1", "1");
            topoMap.put(c1p1servers + "/2", "2");

            String p1b1         = cluster1 + "/servers" + "/1";
            topoMap.put(p1b1 + "/name", "1");
            topoMap.put(p1b1 + "/mac", "00:00:00:00:00:02");
            topoMap.put(p1b1 + "/ip", "10.1.2.3");
            topoMap.put(p1b1 + "/port", "80");
            topoMap.put(p1b1 + "/protocol", "tcp");
            topoMap.put(p1b1 + "/state", "1");
            topoMap.put(p1b1 + "/pool", "1");

            String p1b2         = cluster1 + "/servers" + "/2";
            topoMap.put(p1b2 + "/name", "2");
            topoMap.put(p1b2 + "/mac", "00:00:00:00:00:03");
            topoMap.put(p1b2 + "/ip", "10.1.2.4");
            topoMap.put(p1b2 + "/port", "80");
            topoMap.put(p1b2 + "/protocol", "tcp");
            topoMap.put(p1b2 + "/state", "1");
            topoMap.put(p1b2 + "/pool", "1");

            String cluster2     = "/a5/cluster/2";
            topoMap.put(cluster2 + "/name", "c2");
            topoMap.put(cluster2 + "/type", "ha");

            String p2b1         = cluster2 + "/servers" + "/1";
            topoMap.put(p2b1 + "/name", "1");
            topoMap.put(p2b1 + "/mac", "00:00:00:00:00:02");
            topoMap.put(p2b1 + "/ip", "10.1.2.3");
            topoMap.put(p2b1 + "/port", "80");
            topoMap.put(p2b1 + "/protocol", "tcp");
            topoMap.put(p2b1 + "/state", "1");
            topoMap.put(p2b1 + "/pool", "1");

            String c2a1         = cluster2 + "/a5/1";
            topoMap.put(c2a1 + "/name", "c2a1");
            topoMap.put(c2a1 + "/controller", "10.1.3.2:6633");
            topoMap.put(c2a1 + "/ovs", "10.1.3.2");

            client = CreateClientExamples.createWithOptions(connectionString, new ExponentialBackoffRetry(1000, 3), 30000, 30000);
            client.start();

            Iterator<Map.Entry<String, String>> iter = topoMap.entrySet().iterator();
            while(iter.hasNext()) {
                Map.Entry<String, String> entry = iter.next();
                client.create().creatingParentsIfNeeded().forPath(entry.getKey(), entry.getValue().getBytes());
            }

            iter = topoMap.entrySet().iterator();
            while(iter.hasNext()) {
                Map.Entry<String, String> entry = iter.next();
                byte[] data = client.getData().forPath(entry.getKey());
                System.out.println(entry.getKey() + "-->" + data);
            }

            //while(iter.hasNext()) {
            //    Map.Entry<String, String> entry = iter.next();
            //    client.delete().forPath(entry.getKey());
            //    Stat state = client.checkExists().forPath(entry.getKey());
            //    System.out.println(entry.getKey() + "state -->" + state);
            //}

        } catch (Exception e) {
            e.printStackTrace();
            if (client != null) {
                client.close();
            }
        } finally {
            if (client != null) {
                client.close();
            }
        }

    }
}
