//package net.floodlightcontroller.monitor;
//
//
//import java.util.HashMap;
//import java.util.EnumSet;
//import java.net.SocketAddress;
//import java.net.InetSocketAddress;
//import java.net.SocketException;
//
//import org.jboss.netty.bootstrap.ClientBootstrap;
//import org.jboss.netty.channel.Channel;
//import org.jboss.netty.channel.ChannelFuture;
//import org.jboss.netty.channel.ChannelFutureListener;
//import org.jboss.netty.channel.ChannelPipelineFactory;
//import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;
//import org.jboss.netty.channel.group.ChannelGroup;
//import org.jboss.netty.channel.group.DefaultChannelGroup;
//
//import java.util.concurrent.ConcurrentHashMap;
//import java.util.concurrent.atomic.AtomicInteger;
//import java.util.concurrent.locks.Condition;
//import java.util.concurrent.locks.Lock;
//import java.util.concurrent.locks.ReentrantLock;
//import java.util.concurrent.Executors;
//import java.util.concurrent.ExecutorService;
//import java.util.concurrent.ThreadFactory;
//import java.util.concurrent.TimeUnit;
//import java.util.concurrent.ScheduledExecutorService;
//
//import net.floodlightcontroller.core.util.SingletonTask;
//import net.floodlightcontroller.threadpool.IThreadPoolService;
//import net.floodlightcontroller.core.annotations.LogMessageCategory;
//import net.floodlightcontroller.core.annotations.LogMessageDoc;
//import net.floodlightcontroller.core.annotations.LogMessageDocs;
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
//
////TODO HAListener to Slave call Client.shutdown()
//public class ClientManager {
//    protected static final Logger logger =
//            LoggerFactory.getLogger(ClientManager.class);
//
//    protected IThreadPoolService threadPool;
//
//    /**
//     * Buffer size for sockets
//     */
//    public static final int SEND_BUFFER_SIZE = 4 * 1024 * 1024;
//
//    /**
//     * Connect timeout for client connections
//     */
//    public static final int CONNECT_TIMEOUT = 500;
//
//    /**
//     * Channel group that will hold all our channels
//     */
//    final ChannelGroup cg = new DefaultChannelGroup("heartbeat dected");
//    ClientPipelineFactory pipelineFactory;
//
//    protected ExecutorService bossExecutor;
//    protected ExecutorService workerExecutor;
//    protected ClientBootstrap clientBootstrap;
//
//    /**
//     * Task to periodically ensure that connections are active
//     */
//    protected SingletonTask reconnectTask;
//
//    /**
//     * Active connection to server
//     */
//    protected volatile Channel channel;
//    private volatile int connectionGeneration = 0;
//    protected Object readyNotify = new Object();
//    protected volatile boolean ready = false;
//    protected volatile boolean shutDown = false;
//
//    /**
//     * Node connections
//     */
//    protected HashMap<String, NodeConnection> connections =
//            new HashMap<String, NodeConnection>();
//
//    private ConcurrentHashMap<Integer, ClientFuture> futureMap = 
//            new ConcurrentHashMap<Integer, ClientFuture>();
//    private Object futureNotify = new Object();
//    private static int MAX_PENDING_REQUESTS = 1000;
//
//    protected IMonitorService monitor;
//    protected ConcurrentHashMap<Short, MessageWindow> messageWindows;
//    protected static final EnumSet<MessageType> windowedTypes = 
//            EnumSet.of(MessageType.SYNC_VALUE,
//                       MessageType.SYNC_OFFER);
//
//    public enum MessageType {
//        SYNC_VALUE,
//        SYNC_OFFER
//    }
//
//    protected static final int MAX_PENDING_MESSAGES = 500;
//
//    public ClientManager(IMonitorService monitor) {
//        this.monitor = monitor;
//        messageWindows = new ConcurrentHashMap<Short, MessageWindow>();
//    }
//
//    /**
//     * Various states for connections
//     * @author readams
//     */
//    protected enum NodeConnectionState {
//        NONE,
//        PENDING,
//        CONNECTED
//    }
//
//    /**
//     * Connection state wrapper for node connections
//     * @author readams
//     */
//    protected static class NodeConnection {
//        volatile NodeConnectionState state = NodeConnectionState.NONE;        
//        protected ChannelFuture pendingFuture;
//        protected Channel nodeChannel;
//        
//        protected void nuke() {
//            state = NodeConnectionState.NONE;
//            if (pendingFuture != null) pendingFuture.cancel();
//            if (nodeChannel != null) nodeChannel.close();
//            pendingFuture = null;
//            nodeChannel = null;
//        }
//    }
//
//    /**
//     * Maintain state for the pending message window for a given message type
//     * @author readams
//     */
//    protected static class MessageWindow {
//        AtomicInteger pending = new AtomicInteger();
//        volatile boolean disconnected = false;
//        Lock lock = new ReentrantLock();
//        Condition full = lock.newCondition();
//    }
//
//    /**
//     * Wait for the client connection
//     * @author readams
//     */
//    protected class ConnectCFListener implements ChannelFutureListener {
//        protected DestHost node;
//
//        public ConnectCFListener(DestHost node) {
//            super();
//            this.node = node;
//        }
//
//        @Override
//        public void operationComplete(ChannelFuture cf) throws Exception {
//            if (!cf.isSuccess()) {
//                synchronized (connections) {
//                    NodeConnection c = connections.remove(node.getId());
//                    if (c != null) c.nuke();
//                    cf.getChannel().close();
//                }
//                
//                String message = "[unknown error]";
//                if (cf.isCancelled()) message = "Timed out on connect";
//                if (cf.getCause() != null) message = cf.getCause().getMessage();
//                logger.debug("[->] Could not connect to " +
//                             "node: ", 
//                             new Object[]{ message });
//            } else {
//                logger.trace("[->] Channel future successful");
//            }
//        }
//    }
//
//    /**
//     * Ensure that all client connections are active
//     */
//    protected void startClientConnections() {
//    //    for (DestHost n : getDestHosts()) {
//    //        doNodeConnect(n);
//    //    }
//    }
//
//    /**
//     * Connect to a remote node if appropriate
//     * @param bootstrap the client bootstrap object
//     * @param n the node to connect to
//     */
//    protected void doNodeConnect(DestHost n) {
//    //    if (!shutDown && n.getNodeId() < syncManager.getLocalNodeId()) {
//    //        Short nodeId = n.getNodeId();
//
//    //        synchronized (connections) {
//    //            NodeConnection c = connections.get(n.getNodeId());
//    //            if (c == null) {
//    //                connections.put(nodeId, c = new NodeConnection());
//    //            }
//
//    //            if (logger.isTraceEnabled()) {
//    //                logger.trace("[{}->{}] Connection state: {}", 
//    //                             new Object[]{syncManager.getLocalNodeId(),
//    //                                          nodeId, c.state});
//    //            }
//    //            if (c.state.equals(NodeConnectionState.NONE)) {
//    //                if (logger.isDebugEnabled()) {
//    //                    logger.debug("[{}->{}] Attempting connection {} {}", 
//    //                                 new Object[]{syncManager.getLocalNodeId(),
//    //                                              nodeId, 
//    //                                              n.getHostname(), 
//    //                                              n.getPort()});
//    //                }
//    //                SocketAddress sa =
//    //                        new InetSocketAddress(n.getHostname(), n.getPort());
//    //                c.pendingFuture = clientBootstrap.connect(sa);
//    //                c.pendingFuture.addListener(new ConnectCFListener(n));
//    //                c.state = NodeConnectionState.PENDING;
//    //            }
//    //        }
//    //    }
//    }
//
//    /**
//     * Periodically ensure that all the node connections are alive
//     * @author readams
//     */
//    protected class ConnectTask implements Runnable {
//        @Override
//        public void run() {
//            try {
//                if (!shutDown)
//                    startClientConnections();
//            } catch (Exception e) {
//                logger.error("Error in reconnect task", e);
//            }
//            if (!shutDown) {
//                reconnectTask.reschedule(500, TimeUnit.MILLISECONDS);
//            }
//        }
//    }
//
//    void run() {
//        //TODO
//        threadPool = this.monitor.getThreadPool();
//        final ThreadGroup tg2 = new ThreadGroup("Monitor Threads");
//        tg2.setMaxPriority(Thread.NORM_PRIORITY);
//        ThreadFactory f2 = new ThreadFactory() {
//            @Override
//            public Thread newThread(Runnable runnable) {
//                return new Thread(tg2, runnable);
//            }
//        };
//        bossExecutor = Executors.newCachedThreadPool(f2);
//        workerExecutor = Executors.newCachedThreadPool(f2);
//        pipelineFactory = new ClientPipelineFactory(this.monitor, this);
//        startClients(pipelineFactory);
//    }
//
//    /**
//     * Connect to remote servers.  We'll initiate the connection to
//     * any nodes with a lower ID so that there will be a single connection
//     * between each pair of nodes which we'll use symmetrically
//     */
//    protected void startClients(ChannelPipelineFactory pipelineFactory) {
//        final ClientBootstrap bootstrap =
//                new ClientBootstrap(
//                     new NioClientSocketChannelFactory(bossExecutor,
//                                                       workerExecutor));
//        bootstrap.setOption("child.reuseAddr", true);
//        bootstrap.setOption("child.keepAlive", true);
//        bootstrap.setOption("child.tcpNoDelay", true);
//        bootstrap.setOption("child.sendBufferSize", SEND_BUFFER_SIZE);
//        bootstrap.setOption("child.connectTimeoutMillis", CONNECT_TIMEOUT);
//        bootstrap.setPipelineFactory(pipelineFactory);
//        clientBootstrap = bootstrap;
//
//        ScheduledExecutorService ses = 
//                threadPool.getScheduledExecutor();
//        reconnectTask = new SingletonTask(ses, new ConnectTask());
//        reconnectTask.reschedule(0, TimeUnit.SECONDS);
//    }
//    
//    /**
//     * Write a message to the node specified
//     * @param nodeId the node ID
//     * @param bsm the message to write
//     * @return <code>true</code> if the message was actually written to 
//     * the channel.  Note this is not the same as having been sent to the 
//     * other node.
//     * @throws InterruptedException 
//     */
//    //public boolean writeToNode(Short nodeId, SyncMessage bsm) 
//    //        throws InterruptedException {
//    //    if (nodeId == null) return false;
//    //    NodeConnection nc = connections.get(nodeId);
//    //    if (nc != null && nc.state == NodeConnectionState.CONNECTED) {
//    //        waitForMessageWindow(bsm.getType(), nodeId, 0);
//    //        nc.nodeChannel.write(bsm);
//    //        return true;
//    //    }
//    //    return false;
//    //}
//
//    /**
//     * Send a request to the server and generate a future for the 
//     * eventual reply.  Note that this call can block if there is no active
//     * connection while a new connection is re-established or if the maximum
//     * number of requests is already pending
//     * @param xid the transaction ID for the request
//     * @param request the actual request to send
//     * @return A {@link Future} for the reply message
//     * @throws InterruptedException 
//     */
//    //public Future<SyncReply> sendRequest(int xid,
//    //                                        Message request) 
//    //                                     throws RemoteStoreException {
//    //    //ensureConnected();
//    //    ClientFuture future = new ClientFuture(xid, connectionGeneration);
//    //    futureMap.put(Integer.valueOf(xid), future);
//
//    //    if (futureMap.size() > MAX_PENDING_REQUESTS) {
//    //        synchronized (futureNotify) {
//    //            while (futureMap.size() > MAX_PENDING_REQUESTS) {
//    //                try {
//    //                    futureNotify.wait();
//    //                } catch (InterruptedException e) {
//    //                    //throw new RemoteStoreException("Could not send request",
//    //                    throw new InterruptedException("Could not send request",
//    //                                                   e);
//    //                }
//    //            }
//    //        }
//    //    }
//    //    channel.write(request); 
//    //    return future;
//    //}
//
//    @LogMessageDoc(level="WARN",
//                   message="Unexpected sync message reply type={type} id={id}",
//                   explanation="An error occurred in the sync protocol",
//                   recommendation=LogMessageDoc.REPORT_CONTROLLER_BUG)
//    public void dispatchReply(int xid,
//                              SyncReply reply) {
//        ClientFuture future = futureMap.get(Integer.valueOf(xid));
//        if (future == null) {
//            logger.warn("Unexpected sync message replyid={}", xid);
//            return;
//        }
//        futureMap.remove(Integer.valueOf(xid));
//        future.setReply(reply);
//        synchronized (futureNotify) {
//            futureNotify.notify();
//        }
//    }
//
//    protected void ensureConnected(String hostName, int port) {
//        if (!ready) {
//            for (int i = 0; i < 2; i++) {
//                synchronized (this) {
//                    connectionGeneration += 1;
//                    if (connect(hostName, port))
//                        return;
//                }
//                try {
//                    Thread.sleep(1000);
//                } catch (Exception e) {}
//            }
//            if (channel == null) 
//                logger.info("channel == null");
//            //throw new SyncRuntimeException(new SyncException("Failed to establish connection"));
//        }
//    }
//
//    protected boolean connect(String hostname, int port) {
//        ready = false;
//        if (channel == null || !channel.isConnected()) {
//            SocketAddress sa =
//                    new InetSocketAddress(hostname, port);
//            ChannelFuture future = clientBootstrap.connect(sa);
//            future.awaitUninterruptibly();
//            if (!future.isSuccess()) {
//                logger.error("Could not connect to " + hostname + 
//                             ":" + port, future.getCause());
//                return false;
//            }
//            channel = future.getChannel();
//        }
//        while (!ready && channel != null && channel.isConnected()) {
//            try {
//                Thread.sleep(10);
//            } catch (InterruptedException e) { }
//        }
//        if (!ready || channel == null || !channel.isConnected()) {
//            logger.warn("Timed out connecting to {}:{}", hostname, port);
//            return false;
//        }
//        logger.debug("Connected to {}:{}", hostname, port);
//        return true;
//    }
//
//    protected void channelDisconnected(SocketException why) {
//        ready = false;
//        connectionGeneration += 1;
//        //if (why == null) why = new RemoteStoreException("Channel disconnected");
//        if (why == null) why = new SocketException("Channel disconnected");
//        for (ClientFuture f : futureMap.values()) {
//            if (f.getConnectionGeneration() < connectionGeneration)
//                dispatchReply(f.getXid(), 
//                              new SyncReply(null, null, false, why, 0));
//        }
//    }
//
//    public void shutdown() {
//        shutDown = true;
//        logger.debug("Shutting down Remote Sync Manager");
//        try {
//            if (!cg.close().await(5, TimeUnit.SECONDS)) {
//                logger.debug("Failed to cleanly shut down remote sync");
//                return;
//            }
//            if (clientBootstrap != null) {
//                clientBootstrap.releaseExternalResources();
//            }
//            clientBootstrap = null;
//            if (pipelineFactory != null)
//                pipelineFactory.releaseExternalResources();
//            pipelineFactory = null;
//            if (workerExecutor != null)
//                workerExecutor.shutdown();
//            workerExecutor = null;
//            if (bossExecutor != null)
//                bossExecutor.shutdown();
//            bossExecutor = null;
//        } catch (InterruptedException e) {
//            logger.debug("Interrupted while shutting down remote sync");
//        }
//    }
//
//    public void disconnectNode(String nodeId) {
//        synchronized (connections) {
//            Short n = Short.valueOf(nodeId);
//            MessageWindow mw = messageWindows.get(n);
//            if (mw != null) {
//                mw.lock.lock();
//                mw.disconnected = true;
//                try {
//                    mw.full.signalAll();
//                    messageWindows.remove(n);
//                } finally {
//                    mw.lock.unlock();
//                }
//            }
//
//            NodeConnection nc = connections.get(nodeId);
//            if (nc != null) {
//                nc.nuke();
//            }
//            connections.remove(nodeId);
//        }
//    }
//
//    /**
//     * Check whether all links are established
//     * @return
//     */
//    public boolean isFullyConnected() {
//    //    for (Node n : syncManager.getClusterConfig().getNodes()) {
//    //        if (n.getNodeId() != syncManager.getLocalNodeId() &&
//    //            !isConnected(n.getNodeId())) {
//    //            if (logger.isTraceEnabled()) {
//    //                logger.trace("[{}->{}] missing connection",
//    //                             syncManager.getLocalNodeId(),
//    //                             n.getNodeId());
//    //            }
//    //            return false;
//    //        }
//    //    }
//        return true;
//    }
//
//    /**
//     * Find out if a particular node is connected
//     * @param nodeId
//     * @return true if the node is connected
//     */
//    public boolean isConnected(String nodeId) {
//        NodeConnection nc = connections.get(nodeId);
//        return (nc != null && nc.state == NodeConnectionState.CONNECTED);
//    }
//
//    /**
//     * Called when a message is acknowledged by a remote node
//     * @param type the message type
//     * @param nodeId the remote node
//     */
//    //public void messageAcked(MessageType type, Short nodeId) {
//    //    if (nodeId == null) return;
//    //    if (!windowedTypes.contains(type)) return;
//
//    //    MessageWindow mw = messageWindows.get(nodeId);
//    //    if (mw == null) return;
//
//    //    int pending = mw.pending.decrementAndGet();
//    //    if (pending < MAX_PENDING_MESSAGES) {
//    //        mw.lock.lock();
//    //        try {
//    //            mw.full.signalAll();
//    //        } finally {
//    //            mw.lock.unlock();
//    //        }
//    //    }
//    //}
//
//    // *************
//    // Local methods
//    // *************
//    /**
//     * Wait for a message window slow to be available for the given node and 
//     * message type
//     * @param type the type of the message
//     * @param nodeId the node Id
//     * @param maxWait the maximum time to wait in milliseconds
//     * @throws InterruptedException 
//     * @return <code>true</code> if the message can be safely written
//     */
//    //private boolean waitForMessageWindow(MessageType type, short nodeId,
//    //                                     long maxWait) 
//    //        throws InterruptedException {
//    //    if (!windowedTypes.contains(type)) return true;
//
//    //    long start = System.nanoTime();
//    //    
//    //    // note that this can allow slightly more than the maximum number
//    //    // of messages.  This is fine.
//    //    MessageWindow mw = getMW(nodeId);
//    //    if (!mw.disconnected && 
//    //        mw.pending.get() >= MAX_PENDING_MESSAGES) {
//    //        mw.lock.lock();
//    //        try {
//    //            while (!mw.disconnected && 
//    //                   mw.pending.get() >= MAX_PENDING_MESSAGES) {
//    //                long now = System.nanoTime();
//    //                if (maxWait > 0 && 
//    //                    (now - start) > maxWait * 1000) return false;
//    //                mw.full.awaitNanos(now - start);
//    //            }
//    //        } finally {
//    //            mw.lock.unlock();
//    //        }
//    //    }
//    //    mw = getMW(nodeId);
//    //    if (mw != null)
//    //        mw.pending.getAndIncrement();
//    //    
//    //    return true;
//    //}
//
//    /**
//     * Get the appropriate {@link MessageWindow} object for the given node. 
//     * @param nodeId the remote node
//     * @return a {@link MessageWindow} object 
//     */
//    //private MessageWindow getMW(short nodeId) {
//
//    //    if (!isConnected(nodeId)) return null;
//
//    //    Short n = Short.valueOf(nodeId);
//    //    MessageWindow mw = messageWindows.get(n);
//    //    if (mw == null) {
//    //        mw = new MessageWindow();
//    //        MessageWindow old = messageWindows.putIfAbsent(n, mw);
//    //        if (old != null) mw = old;
//    //    }
//    //    return mw;
//    //}
//
//    /**
//     * Add the node connection to the node connection map
//     * @param nodeId the node ID for the channel
//     * @param channel the new channel
//     */
//    protected void nodeConnected(String nodeId, Channel channel) {
//        logger.debug("[->] Connection established",
//                     nodeId);
//        synchronized (connections) {
//            NodeConnection c = connections.get(nodeId);
//            if (c == null) {
//                connections.put(nodeId, c = new NodeConnection());
//            }
//            c.nodeChannel = channel;
//            c.state = NodeConnectionState.CONNECTED;
//        }
//    }
//}
//
