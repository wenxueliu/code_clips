/**
*DistributedReentrantLock.java
*本地线程之间锁争用，先使用虚拟机内部锁机制，减少结点间通信开销
*/
//public class DistributedReentrantLock {
//	private static final Logger logger = Logger.getLogger(DistributedReentrantLock.class);
// private ReentrantLock reentrantLock = new ReentrantLock();
//
// private WriteLock writeLock;
// private long timeout = 3*60*1000;
// 
// private final Object mutex = new Object();
// private String dir;
// private String schema;
// 
// private final ExitListener exitListener = new ExitListener(){
//		@Override
//		public void execute() {
//			initWriteLock();
//		}
//	};
//	
//	private synchronized void initWriteLock(){
//		logger.debug("初始化writeLock");
//		writeLock = new WriteLock(dir,new LockListener(){
//
//			@Override
//			public void lockAcquired() {
//				synchronized(mutex){
//					mutex.notify();
//				}
//			}
//			@Override
//			public void lockReleased() {
//			}
// 		
// 	},schema);
//		
//		if(writeLock != null && writeLock.zk != null){
//			writeLock.zk.addExitListener(exitListener);
//		}
//		
//		synchronized(mutex){
//			mutex.notify();
//		}
//	}
//	
// public DistributedReentrantLock(String dir,String schema) {	
// 	this.dir = dir;
// 	this.schema = schema;
// 	initWriteLock();
// }
//
// public void lock(long timeout) throws InterruptedException, KeeperException {
// reentrantLock.lock();//多线程竞争时，先拿到第一层锁
// try{
// 	boolean res = writeLock.trylock();
// 	if(!res){
//	 	synchronized(mutex){
//					mutex.wait(timeout);
//				}
//	 	if(writeLock == null || !writeLock.isOwner()){
//	 		throw new InterruptedException("锁超时");
//	 	}
// 	}
// }catch(InterruptedException e){
// 	reentrantLock.unlock();
// 	throw e;
// }catch(KeeperException e){
// 	reentrantLock.unlock();
// 	throw e;
// }
// }
// 
// public void lock() throws InterruptedException, KeeperException {
// 	lock(timeout);
// }
//
// public void destroy() throws KeeperException {
// 	writeLock.unlock();
// }
// 
//
// public boolean unlock(){
// 	if(!isOwner()) return false;
// try{
// 	writeLock.unlock();
// 	reentrantLock.unlock();//多线程竞争时，释放最外层锁
// }catch(RuntimeException e){
// 	reentrantLock.unlock();//多线程竞争时，释放最外层锁
// 	throw e;
// }
// 
// return reentrantLock.hasQueuedThreads();
// }
//
//
//
// public boolean isOwner() {
// return reentrantLock.isHeldByCurrentThread() && writeLock.isOwner();
// }
//
//	public void clear() {
//		writeLock.clear();
//	}
//
//}
