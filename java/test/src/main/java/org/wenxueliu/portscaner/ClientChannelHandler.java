//package net.floodlightcontroller.monitor;
//  
//import java.io.IOException;
//import java.net.ConnectException;
//
//import org.jboss.netty.channel.ChannelHandlerContext;
//import org.jboss.netty.channel.ChannelStateEvent;
//import org.jboss.netty.channel.MessageEvent;
//import org.jboss.netty.channel.ExceptionEvent;
//import org.jboss.netty.handler.timeout.IdleStateAwareChannelHandler;
//import org.jboss.netty.handler.timeout.IdleStateEvent;
//import org.jboss.netty.handler.timeout.ReadTimeoutException;
//
//import org.sdnplatform.sync.error.HandshakeTimeoutException;
//
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
//
///**
// * * Handles a client-side channel.
// * */
//public class ClientChannelHandler extends IdleStateAwareChannelHandler {
//
//    protected static final Logger logger =
//            LoggerFactory.getLogger(ClientChannelHandler.class);
//
//    IMonitorService monitorService;
//    ClientManager clientManager;
//
//    protected enum ChannelState {
//        OPEN,
//        CONNECTED,
//    }
//
//    protected ChannelState channelState = ChannelState.OPEN;
//
//    public ClientChannelHandler(IMonitorService monitorService, ClientManager clientManager) {
//        this.monitorService = monitorService;
//        this.clientManager = clientManager;
//    }
//
//    @Override
//    public void channelOpen(ChannelHandlerContext ctx, 
//                            ChannelStateEvent e) throws Exception {
//        clientManager.cg.add(ctx.getChannel());
//    }
//
//    @Override
//    public void channelDisconnected(ChannelHandlerContext ctx,
//                                    ChannelStateEvent e) throws Exception {
//        this.clientManager.channel = null;
//        clientManager.ready = false;
//        clientManager.channelDisconnected(null);
//    }
//
//    @Override
//    public void channelConnected(ChannelHandlerContext ctx,
//                                 ChannelStateEvent e) throws Exception {
//
//        //set the host active true
//    }
//
//    @Override
//    public void channelIdle(ChannelHandlerContext ctx,
//                            IdleStateEvent e) throws Exception {
//        // can set some data ?
//    }
//
//    @Override
//    public void messageReceived(ChannelHandlerContext ctx, MessageEvent msg) {
//        System.err.println("message received");
//    }
//
//    @Override
//    public void exceptionCaught(ChannelHandlerContext ctx,
//                                ExceptionEvent e) throws Exception {
//        if (e.getCause() instanceof ReadTimeoutException) {
//            // read timeout, 
//            // TODO remoteNode ip:port
//            logger.error("[->] Disconnecting due to read timeout");
//            ctx.getChannel().close();
//        } else if (e.getCause() instanceof HandshakeTimeoutException) {
//            // read timeout
//            logger.error("[->] Disconnecting RPC node due to " +
//                    "handshake timeout");
//            ctx.getChannel().close();
//        } else if (e.getCause() instanceof ConnectException ||
//                   e.getCause() instanceof IOException) {
//            logger.debug("[->] {}: {}",
//                         new Object[] {
//                                       e.getCause().getClass().getName(),
//                                       e.getCause().getMessage()});
//        } else {
//            logger.error("[->] An error occurred on RPC channel {}",
//                         new Object[]{
//                                      e.getCause()});
//            ctx.getChannel().close();
//        }
//    }
//
//    protected void handleUnknownMessage(ChannelHandlerContext ctx, 
//                                        Object message) {
//        logger.warn("[{}->{}] Unhandled message: {}", 
//                    new Object[]{ message.getClass().getCanonicalName()});
//    }
//}
