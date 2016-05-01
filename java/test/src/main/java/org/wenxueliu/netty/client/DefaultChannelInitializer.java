package org.wenxueliu.netty.client;

import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.handler.codec.string.StringEncoder;
import io.netty.util.Timer;
import io.netty.util.CharsetUtil;

public class DefaultChannelInitializer extends ChannelInitializer<Channel> {
    private final ClientTest bootstrap;
    private static final int maxFrameSize = 1024 * 1024 * 10;
    protected Timer timer;

    public DefaultChannelInitializer(Timer timer, ClientTest bootstrap) {
        super();
        this.timer = timer;
        this.bootstrap = bootstrap;
    }

    @Override
    protected void initChannel(Channel ch) throws Exception {

        ChannelPipeline pipeline = ch.pipeline();

        pipeline.addLast("MessageDecoder", new MessageDecoder());

        pipeline.addLast("MessageEncoder", new StringEncoder(CharsetUtil.UTF_8));

        pipeline.addLast("timeout", new DefaultTimeoutHandler(timer, 10));

        pipeline.addLast("handler", new DefaultHandler());
    }
}
