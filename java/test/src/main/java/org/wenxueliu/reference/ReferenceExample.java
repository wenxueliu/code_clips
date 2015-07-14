package org.wenxueliu.reference;

import java.util.HashMap;

public class ReferenceExample {
    private int id;
    private String name;
    private Host   host;

    public static class Host {
        String ip;
        String mac;

        public Host(String ip, String mac){
            this.ip = ip;
            this.mac = mac;
        }

        public void setIp(String ip) {
            this.ip = ip;
        }

        public void setMac(String mac) {
            this.mac = mac;
        }

        @Override
        public String toString(){
            return "ip=" + ip + "mac=" + mac;
        }
    }

    public ReferenceExample(int id, String name, Host host) {
        this.id = id;
        this.name = name;
        this.host = host;
    }

    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }

    public Host getHost() {
        return host;
    }

    public void setHost(Host host) {
        this.host = host;
    }

    public int getId() {
        return id;
    }

    public void setId(int id){
        this.id = id;
    }

    @Override
    public String toString() {
        return "id=" + id + ",name=" + name + ",host=" + host.toString();
    }

    public static void main(String[] args) {
        Host hs = new ReferenceExample.Host("10.0.0.1", "00:00:00:00:00");
        Host hs1 = new ReferenceExample.Host("100.0.0.1", "00:00:00:00:00");
        int id = 1;
        String name = "aaa";
        ReferenceExample re = new ReferenceExample(id, name,hs);

        HashMap<String, ReferenceExample> hm = new HashMap<String, ReferenceExample>();
        hm.put(String.valueOf(re.getId()), re);
        ReferenceExample re1 = hm.get("1");
        System.out.println("before change:" + re1.toString());
        re1.setName("bbb");
        re1.setHost(hs1);
        System.out.println("after change:" + hm.get("1").toString());

    }

}
