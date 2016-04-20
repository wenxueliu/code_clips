/**
 *http://rdc.gleasy.com/%E4%B8%A4%E7%A7%8D%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%AE%9E%E7%8E%B0%E6%96%B9%E6%A1%881.html
* DistributedLockUtil.java
* 分布式锁工厂类，所有分布式请求都由该工厂类负责
**/
//public class DistributedLockUtil {
//	private static Object schemeLock = new Object();
//	private static Object mutexLock = new Object();
//	private static Map<String,Object> mutexLockMap = new ConcurrentHashMap();
//	private String schema;
//	private Map<String,DistributedReentrantLock> cache = new ConcurrentHashMap<String,DistributedReentrantLock>();
//
//	private static Map<String,DistributedLockUtil> instances = new ConcurrentHashMap();
//	public static DistributedLockUtil getInstance(String schema){
//		DistributedLockUtil u = instances.get(schema);
//		if(u==null){
//			synchronized(schemeLock){
//				u = instances.get(schema);
//				if(u == null){
//					u = new DistributedLockUtil(schema);
//					instances.put(schema, u);
//				}
//			}
//		}
//		return u;
//	}
//
//	private DistributedLockUtil(String schema){
//		this.schema = schema;
//	}
//
//	private Object getMutex(String key){
//		Object mx = mutexLockMap.get(key);
//		if(mx == null){
//			synchronized(mutexLock){
//				mx = mutexLockMap.get(key);
//				if(mx==null){
//					mx = new Object();
//					mutexLockMap.put(key,mx);
//				}
//			}
//		}
//		return mx;
//	}
//
//	private DistributedReentrantLock getLock(String key){
//		DistributedReentrantLock lock = cache.get(key);
//		if(lock == null){
//			synchronized(getMutex(key)){
//				lock = cache.get(key);
//				if(lock == null){
//					lock = new DistributedReentrantLock(key,schema);
//					cache.put(key, lock);
//				}
//			}
//		}
//		return lock;
//	}
//	
//	public void reset(){
//		for(String s : cache.keySet()){
//			getLock(s).unlock();
//		}
//	}
//	
//	/**
//	 * 尝试加锁
//	 * 如果当前线程已经拥有该锁的话,直接返回false,表示不用再次加锁,此时不应该再调用unlock进行解锁
//	 * 
//	 * @param key
//	 * @return
//	 * @throws InterruptedException
//	 * @throws KeeperException
//	 */
//	public LockStat lock(String key) throws InterruptedException, KeeperException{
//		if(getLock(key).isOwner()){
//			return LockStat.NONEED;
//		}
//		getLock(key).lock();
//		return LockStat.SUCCESS;
//	}
//	
//	public void clearLock(String key) throws InterruptedException, KeeperException{
//		synchronized(getMutex(key)){
//			DistributedReentrantLock l = cache.get(key);
//			l.clear();
//			cache.remove(key);
//		}
//	}	
//	
//	public void unlock(String key,LockStat stat) throws InterruptedException, KeeperException{
//		unlock(key,stat,false);
//	}
//
//	public void unlock(String key,LockStat stat,boolean keepalive) throws InterruptedException, KeeperException{
//		if(stat == null) return;
//		if(LockStat.SUCCESS.equals(stat)){
//			DistributedReentrantLock lock = getLock(key);
//			boolean hasWaiter = lock.unlock();
//			if(!hasWaiter && !keepalive){
//				synchronized(getMutex(key)){
//					lock.clear();
//					cache.remove(key);
//				}
//			}
//		}
//	}
//	
//	public static enum LockStat{
//		NONEED,
//		SUCCESS
//	}
//}
