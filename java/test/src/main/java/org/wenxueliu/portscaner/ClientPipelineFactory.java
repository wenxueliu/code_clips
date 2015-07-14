//package net.floodlightcontroller.monitor;
//
//import org.jboss.netty.channel.ChannelPipeline;
//import org.jboss.netty.channel.ChannelPipelineFactory;
//import org.jboss.netty.channel.Channels;
//import org.jboss.netty.handler.timeout.IdleStateHandler;
//import org.jboss.netty.handler.timeout.ReadTimeoutHandler;
//import org.jboss.netty.handler.codec.string.StringDecoder;
//import org.jboss.netty.handler.codec.string.StringEncoder;
//import org.jboss.netty.util.ExternalResourceReleasable;
//import org.jboss.netty.util.HashedWheelTimer;
//import org.jboss.netty.util.Timer;
//
//
///**
// * Pipeline factory for the sync service.
// * @see SyncManager
// * @author readams
// */
//public class ClientPipelineFactory
//    implements ChannelPipelineFactory, ExternalResourceReleasable {
//
//    protected IMonitorService monitorService;
//    protected ClientManager clientManager;
//    protected Timer timer;
//
//    private static final int maxFrameSize = 512 * 1024;
//    
//    public ClientPipelineFactory(IMonitorService monitorService,
//                              ClientManager clientManager) {
//        super();
//        this.monitorService = monitorService;
//        this.clientManager = clientManager;
//        this.timer = new HashedWheelTimer();
//    }
//
//    @Override
//    public ChannelPipeline getPipeline() throws Exception {
//        ClientChannelHandler channelHandler = 
//                new ClientChannelHandler(this.monitorService, this.clientManager);
//
//        IdleStateHandler idleHandler = 
//                new IdleStateHandler(timer, 5, 10, 0);
//        ReadTimeoutHandler readTimeoutHandler = 
//                new ReadTimeoutHandler(timer, 30);
//        
//        ChannelPipeline pipeline = Channels.pipeline();
//        pipeline.addLast("idle", idleHandler);
//        pipeline.addLast("timeout", readTimeoutHandler);
//        pipeline.addLast("handshaketimeout",
//                         new HandshakeTimeoutHandler(channelHandler, timer, 10));
//
//        //pipeline.addLast("frameDecoder",
//        //                 new ThriftFrameDecoder(maxFrameSize));
//        //pipeline.addLast("frameEncoder",
//        //                 new ThriftFrameEncoder());
//
//        pipeline.addLast("decoder", new StringDecoder());
//        pipeline.addLast("encoder", new StringEncoder());
//
//        pipeline.addLast("handler", channelHandler);
//        return pipeline;
//    }
//
//    @Override
//    public void releaseExternalResources() {
//        timer.stop();        
//    }
//}
