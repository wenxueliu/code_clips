package org.wenxueliu.concurrent;

@SuppressWarnings("all") public class BkInfo {
    private String poolId;
    private String bkId;
    private long lastSeeTime;
    //may overflow ?
    private long hitTimes;

    public BkInfo(String poolId, String bkId) {
        this.poolId = poolId;
        this.bkId = bkId;
        this.lastSeeTime = System.currentTimeMillis();
        this.hitTimes = 0;
    }

    public String getPoolId() {
        return this.poolId;
    }

    public void setPoolId(String poolId) {
        this.poolId = poolId;
    }

    public String getBkId() {
        return this.bkId;
    }

    public void setBkId(String bkId) {
        this.bkId = bkId;
    }

    public long getLastSeeTime() {
        return this.lastSeeTime;
    }

    public void setLastSeeTime(long nowTime) {
        this.lastSeeTime = nowTime;
    }

    public long getHitTimes() {
        return this.hitTimes;
    }

    public void hitTimesInc() {
        this.hitTimes = this.hitTimes + 1;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        BkInfo other = (BkInfo) obj;
        if (poolId != other.poolId) {
            return false;
        }
        if (bkId != other.bkId) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + poolId.hashCode();
        result = prime * result + bkId.hashCode();
        return result;
    }

    @Override
    public String toString() {
        StringBuilder str = new StringBuilder().append("BkInfo [bkId=").append(bkId)
            .append(", poolId=").append(poolId)
            .append(", lastSeeTime=").append(lastSeeTime)
            .append(", hitTimes=").append(hitTimes)
            .append("]");
        return str.toString();
    }
}
