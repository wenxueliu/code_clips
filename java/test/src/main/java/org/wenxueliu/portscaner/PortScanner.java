//public class MyPipelineFactory implements ChannelPipelineFactory {
//
//     private final Timer timer;
//     private final ChannelHandler idleStateHandler;
//
//     public MyPipelineFactory(Timer timer) {
//         this.timer = timer;
//         this.idleStateHandler = new IdleStateHandler(timer, 60, 30, 0), // timer must be shared.
//     }
//
//     public ChannelPipeline getPipeline() {
//         return Channels.pipeline(
//             idleStateHandler,
//             new MyHandler());
//     }
// }
//
// // Handler should handle the IdleStateEvent triggered by IdleStateHandler.
// public class MyHandler extends IdleStateAwareChannelHandler {
//
//     @Override
//     public void channelIdle(ChannelHandlerContext ctx, IdleStateEvent e) {
//         if (e.getState() == IdleState.READER_IDLE) {
//             e.getChannel().close();
//         } else if (e.getState() == IdleState.WRITER_IDLE) {
//             e.getChannel().write(new PingMessage());
//         }
//     }
// }
//
// ServerBootstrap bootstrap = ...;
// Timer timer = new HashedWheelTimer();
// ...
// bootstrap.setPipelineFactory(new MyPipelineFactory(timer));
//
// // Decoders
// pipeline.addLast("frameDecoder", new DelimiterBasedFrameDecoder(80, Delimiters.nulDelimiter()));
// pipeline.addLast("customDecoder", new OneToOneDecoder() { ... });
//
// // Encoder
// pipeline.addLast("customEncoder", new OneToOneEncoder() { ... });
