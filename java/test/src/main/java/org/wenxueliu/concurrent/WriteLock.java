/**
*WriteLock.java
*基于zk的锁实现
*一个最简单的场景如下：
*1.结点A请求加锁，在特定路径下注册自己（会话自增结点)，得到一个ID号1
*2.结点B请求加锁，在特定路径下注册自己（会话自增结点)，得到一个ID号2
*3.结点A获取所有结点ID，判断出来自己是最小结点号，于是获得锁
*4.结点B获取所有结点ID，判断出来自己不是最小结点，于是监听小于自己的最大结点（结点A）变更事件
*5.结点A拿到锁，处理业务，处理完，释放锁（删除自己）
*6.结点B收到结点A变更事件，判断出来自己已经是最小结点号，于是获得锁。
*/
//public class WriteLock extends ZkPrimative {
// private static final Logger LOG = Logger.getLogger(WriteLock.class);
//
// private final String dir;
// private String id;
// private LockNode idName;
// private String ownerId;
// private String lastChildId;
// private byte[] data = {0x12, 0x34};
// private LockListener callback;
// 
// public WriteLock(String dir,String schema) {
// super(schema,true);
// this.dir = dir;
// }
// 
// public WriteLock(String dir,LockListener callback,String schema) {
// 	this(dir,schema);
// <a href="http://www.nbso.ca/">nbso online casino reviews</a>  this.callback = callback;
// }
//
// public LockListener getLockListener() {
// return this.callback;
// }
// 
// public void setLockListener(LockListener callback) {
// this.callback = callback;
// }
//
// public synchronized void unlock() throws RuntimeException {
// 	if(zk == null || zk.isClosed()){
// 		return;
// 	}
// if (id != null) {
// try {
// 	 zk.delete(id, -1); 
// } catch (InterruptedException e) {
// LOG.warn("Caught: " e, e);
// //set that we have been interrupted.
// Thread.currentThread().interrupt();
// } catch (KeeperException.NoNodeException e) {
// // do nothing
// } catch (KeeperException e) {
// LOG.warn("Caught: " e, e);
// throw (RuntimeException) new RuntimeException(e.getMessage()).
// initCause(e);
// }finally {
// if (callback != null) {
// callback.lockReleased();
// }
// id = null;
// }
// }
// }
// 
// private class LockWatcher implements Watcher {
// public void process(WatchedEvent event) {
// LOG.debug("Watcher fired on path: " event.getPath() " state: " 
// event.getState() " type " event.getType());
// try {
// trylock();
// } catch (Exception e) {
// LOG.warn("Failed to acquire lock: " e, e);
// }
// }
// }
// 
// private void findPrefixInChildren(String prefix, ZooKeeper zookeeper, String dir) 
// throws KeeperException, InterruptedException {
// List<String> names = zookeeper.getChildren(dir, false);
// for (String name : names) {
// if (name.startsWith(prefix)) {
// id = dir "/" name;
// if (LOG.isDebugEnabled()) {
// LOG.debug("Found id created last time: " id);
// }
// break;
// }
// }
// if (id == null) {
// id = zookeeper.create(dir "/" prefix, data, 
// acl, EPHEMERAL_SEQUENTIAL);
//
// if (LOG.isDebugEnabled()) {
// LOG.debug("Created id: " id);
// }
// }
//
// }
//
//	public void clear() {
//		if(zk == null || zk.isClosed()){
// 		return;
// 	}
//		try {
//			zk.delete(dir, -1);
//		} catch (Exception e) {
//			 LOG.error("clear error: " e,e);
//		} 
//	}
//	
// public synchronized boolean trylock() throws KeeperException, InterruptedException {
// 	if(zk == null){
// 		LOG.info("zk 是空");
// 		return false;
// 	}
// if (zk.isClosed()) {
// 	LOG.info("zk 已经关闭");
// return false;
// }
// ensurePathExists(dir);
// 
// LOG.debug("id:" id);
// do {
// if (id == null) {
// long sessionId = zk.getSessionId();
// String prefix = "x-" sessionId "-";
// idName = new LockNode(id);
// LOG.debug("idName:" idName);
// }
// if (id != null) {
// List<String> names = zk.getChildren(dir, false);
// if (names.isEmpty()) {
// LOG.warn("No children in: " dir " when we've just " 
// "created one! Lets recreate it...");
// id = null;
// } else {
// SortedSet<LockNode> sortedNames = new TreeSet<LockNode>();
// for (String name : names) {
// sortedNames.add(new LockNode(dir "/" name));
// }
// ownerId = sortedNames.first().getName();
// LOG.debug("all:" sortedNames);
// SortedSet<LockNode> lessThanMe = sortedNames.headSet(idName);
// LOG.debug("less than me:" lessThanMe);
// if (!lessThanMe.isEmpty()) {
// 	LockNode lastChildName = lessThanMe.last();
// lastChildId = lastChildName.getName();
// if (LOG.isDebugEnabled()) {
// LOG.debug("watching less than me node: " lastChildId);
// }
// Stat stat = zk.exists(lastChildId, new LockWatcher());
// if (stat != null) {
// return Boolean.FALSE;
// } else {
// LOG.warn("Could not find the" 
// 		" stats for less than me: " lastChildName.getName());
// }
// } else {
// if (isOwner()) {
// if (callback != null) {
// callback.lockAcquired();
// }
// return Boolean.TRUE;
// }
// }
// }
// }
// }
// while (id == null);
// return Boolean.FALSE;
// }
//
// public String getDir() {
// return dir;
// }
//
// public boolean isOwner() {
// return id != null && ownerId != null && id.equals(ownerId);
// }
//
// public String getId() {
// return this.id;
// }
//}
