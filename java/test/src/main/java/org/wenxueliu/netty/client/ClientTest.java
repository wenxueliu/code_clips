/*
 * Copyright 2015 Open Networking Laboratory
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.wenxueliu.netty.client;


import io.netty.bootstrap.Bootstrap;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.PooledByteBufAllocator;
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.channel.Channel;
import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoop;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.ServerChannel;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.string.StringEncoder;
import io.netty.handler.timeout.IdleState;
import io.netty.handler.timeout.IdleStateEvent;
import io.netty.handler.timeout.IdleStateHandler;
import io.netty.handler.timeout.TimeoutException;
import io.netty.util.CharsetUtil;
import io.netty.util.Timer;
import io.netty.util.HashedWheelTimer;

import io.netty.util.concurrent.GlobalEventExecutor;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;
import java.net.InetSocketAddress;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ClientTest {

    protected static final Logger log = LoggerFactory
            .getLogger(ClientTest.class);

    private final ExecutorService executorService = Executors
            .newFixedThreadPool(10);

    private ChannelGroup cg;
    private EventLoopGroup bossGroup;
    private EventLoopGroup workerGroup;
    private Class<? extends ServerChannel> serverChannelClass;

    private Timer timer;
    Bootstrap bootstrap = null;
    DefaultChannelInitializer pipelineFactory;

    private static final int MAX_RETRY = 5;
    private static final int IDLE_TIMEOUT_SEC = 10;

    /**
     * Buffer size for sockets
     */
    public static final int SEND_BUFFER_SIZE = 4 * 1024 * 1024;

    /**
     * Connect timeout for client connections
     */
    public static final int CONNECT_TIMEOUT = 500;

    protected volatile boolean succeeded = false;

    public ClientTest() {
        super();
    }
    /**
     * Initialization.
     */
    private void initEventLoopGroup() {
        cg = new DefaultChannelGroup("Cluster Bootstrap", GlobalEventExecutor.INSTANCE);

        bossGroup = new NioEventLoopGroup();
        workerGroup = new NioEventLoopGroup();
        timer = new HashedWheelTimer();

        serverChannelClass = NioServerSocketChannel.class;
    }

    /**
     * Accepts incoming connections.
     */
    private void startAcceptingConnections() throws InterruptedException {
        ServerBootstrap b = new ServerBootstrap();

        b.group(bossGroup, workerGroup).channel(serverChannelClass)
                .childHandler(new CommunicationChannelInitializer());
        b.option(ChannelOption.SO_BACKLOG, 128);
        b.option(ChannelOption.WRITE_BUFFER_HIGH_WATER_MARK, 32 * 1024);
        b.option(ChannelOption.WRITE_BUFFER_LOW_WATER_MARK, 8 * 1024);
        b.option(ChannelOption.ALLOCATOR, PooledByteBufAllocator.DEFAULT);
        b.childOption(ChannelOption.SO_KEEPALIVE, true);
        b.bind(80).sync();
    }

    /**
     * @throws InterruptedException if thread is interrupted
     */
    public void run() throws InterruptedException {
        initEventLoopGroup();
        startAcceptingConnections();
    }

    /**
     * Adds channel pipeline to handle a new connected node.
     */
    private class CommunicationChannelInitializer
            extends ChannelInitializer<SocketChannel> {
        protected void initChannel(SocketChannel channel) throws Exception {
            log.info("New channel created");
            channel.pipeline().addLast(new StringEncoder(CharsetUtil.UTF_8));
            channel.pipeline().addLast(new MessageDecoder());
            handleNewNodeConnection(channel);
        }
    }

    /**
     * Handles the new connection of node.
     *
     * @param channel the channel to use.
     */
    private void handleNewNodeConnection(final Channel channel) {
        executorService.execute(new Runnable() {
            @Override
            public void run() {
                log.info("Handle new node connection");

                String ipAddress =
                        ((InetSocketAddress) channel.remoteAddress())
                                .getAddress().getHostAddress();
                long port = ((InetSocketAddress) channel.remoteAddress())
                        .getPort();

                log.info("Get connection from ip address {} : {}",
                         ipAddress.toString(), port);

                //specific logic
                DefaultHandler defaultHandler = new DefaultHandler();
                channel.pipeline().addLast(defaultHandler);

                ChannelFuture closeFuture = channel.closeFuture();
                closeFuture
                        .addListener(new ChannelConnectionListener());

                //try {
                //    closeFuture.await();
                //} catch (InterruptedException e) {
                //    log.debug("Interrupted while waiting for bootstrap");
                //}
            }
        });
    }

    /**
     * Starts controller.
     *
     * @param agent OvsdbAgent
     * @param monitorCallback Callback
     */
    public void start() {
        try {
            this.run();
        } catch (InterruptedException e) {
            log.warn("Interrupted while waiting to start");
            Thread.currentThread().interrupt();
        }
    }

    /**
     * Stops controller.
     *
     */
    public void stop() {
        if (cg != null) {
            cg.close().awaitUninterruptibly();
            cg = null;
        }

        bootstrap = null;
        pipelineFactory = null;
        if (workerGroup != null) {
            try {
				workerGroup.shutdownGracefully();
			} catch (TimeoutException e) {
				log.warn("Error waiting for gracefull shutdown of BootstrapClient {}", e);
			}
            workerGroup = null;
        }

        if (bossGroup != null) {
            try {
				bossGroup.shutdownGracefully();
			} catch (TimeoutException e) {
				log.warn("Error waiting for gracefull shutdown of BootstrapClient {}", e);
			}
            bossGroup = null;
        }

        if (timer != null) {
        	timer.stop();
        	timer = null;
        }
    }

    /**
     * Connect to the ovsdb server with given ip address and port number.
     *
     * @param ip ip address
     * @param port port number
     */
    public void connect(String ip, int port) {
        ChannelFutureListener listener = new ConnectionListener(this, ip, port);
        connectRetry(ip, port, listener);
    }

    private void connectRetry(String ip, int port, ChannelFutureListener listener) {
        try {
            bootstrap = new Bootstrap()
                .group(workerGroup)
                .channel(NioSocketChannel.class)
                .option(ChannelOption.TCP_NODELAY, true)
                .option(ChannelOption.SO_REUSEADDR, true)
                .option(ChannelOption.SO_KEEPALIVE, true)
                .option(ChannelOption.SO_SNDBUF, SEND_BUFFER_SIZE)
                .option(ChannelOption.SO_RCVBUF, SEND_BUFFER_SIZE)
                .option(ChannelOption.CONNECT_TIMEOUT_MILLIS, CONNECT_TIMEOUT);

            pipelineFactory = new DefaultChannelInitializer(timer, this);
            bootstrap.handler(pipelineFactory);
                    //.handler(new ChannelInitializer<SocketChannel>() {

                    //    @Override
                    //    protected void initChannel(SocketChannel channel) throws Exception {
                    //        ChannelPipeline p = channel.pipeline();
                    //        p.addLast(new MessageDecoder(),
                    //                  new StringEncoder(CharsetUtil.UTF_8),
                    //                  new IdleStateHandler(IDLE_TIMEOUT_SEC, 0, 0),
                    //                  new BootstrapTimeoutHandler(timer, 10),
                    //                  new ConnectionHandler());
                    //    }
                    //});
            bootstrap.remoteAddress(ip, port);
            ChannelFuture future = bootstrap.connect();
            future.awaitUninterruptibly();
            future.addListener(listener);
        } catch (Exception e) {
            log.warn("Connection to the server {}:{} failed", ip, port);
        }
    }

    private class ConnectionListener implements ChannelFutureListener {
        private ClientTest controller;
        private String ip;
        private int port;
        private AtomicInteger count = new AtomicInteger();

        public ConnectionListener(ClientTest controller,
                                  String ip,
                                  int port) {
            this.controller = controller;
            this.ip = ip;
            this.port = port;
        }

        @Override
        public void operationComplete(ChannelFuture channelFuture) throws Exception {
            if (!channelFuture.isSuccess()) {
                channelFuture.channel().close();

                if (count.incrementAndGet() < MAX_RETRY) {
                    final EventLoop loop = channelFuture.channel().eventLoop();

                    //loop.schedule(() -> {
                    //    controller.connectRetry(this.ip, this.port, this);
                    //}, 1L, TimeUnit.SECONDS);
                } else {
                    log.info("Connection to the ovsdb {}:{} failed",
                             this.ip, this.port);
                }
            } else {
                handleNewNodeConnection(channelFuture.channel());
            }
        }
    }

    private class ConnectionHandler extends ChannelDuplexHandler {

        @Override
        public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
            IdleStateEvent e = (IdleStateEvent) evt;

            if (e.state() == IdleState.READER_IDLE) {
                ctx.close();
            }
        }
    }

    public ChannelGroup getChannelGroup() {
    	return cg;
    }
}
