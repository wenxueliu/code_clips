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

import java.io.IOException;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.handler.timeout.IdleStateEvent;
import io.netty.handler.timeout.ReadTimeoutException;
import java.net.ConnectException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Channel handler deals with the node connection and dispatches
 * ovsdb messages to the appropriate locations.
 */
public final class DefaultHandler extends ChannelInboundHandlerAdapter {
    protected static final Logger log = LoggerFactory
            .getLogger(DefaultHandler.class);

    protected enum ChannelState {
        OPEN,
        CONNECTED,
        AUTHENTICATED;
    }

    protected ChannelState channelState = ChannelState.OPEN;
    //self variable
    // xxService which support specific function is prefer

    /**
     *
     */
    public DefaultHandler() {
        super();
    }

    // ****************************
    // IdleStateAwareChannelHandler
    // ****************************

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg)
            throws Exception {
        log.debug("Receive message");
        /*
        * if (message instanceof SyncMessage) {
        *     handleSyncMessage((SyncMessage)message, ctx.channel());
        * } else if (message instanceof List) {
        *     for (Object i : (List<?>)message) {
        *         if (i instanceof SyncMessage) {
        *             try {
        *                 handleSyncMessage((SyncMessage)i,
        *                                      ctx.channel());
        *             } catch (Exception ex) {
        *                 ctx.fireExceptionCaught(ex);
        *             }
        *         }
        *     }
        * } else {
        *     handleUnknownMessage(ctx, message);
        * }
        */
    }

    // ****************
    // Message Handlers
    // ****************

    /**
     * A handler for messages on the channel that are not of type 
     * {@link SyncMessage}
     * @param ctx the context
     * @param message the message object
     */
    protected void handleUnknownMessage(ChannelHandlerContext ctx, 
                                        Object message) {
        log.warn("[{}->{}] Unhandled message: {}", 
                    new Object[]{getLocalNodeIdString(), 
                                 getRemoteNodeIdString(),
                                 message.getClass().getCanonicalName()});
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        channelState = ChannelState.CONNECTED;
        //specific code
        //bsm = Message();
        //ctx.channel().writeAndFlush(bsm);
    }

    @Override
    public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
    	if (evt instanceof IdleStateEvent) {
    		channelIdle(ctx, (IdleStateEvent) evt);
    	}
        super.userEventTriggered(ctx, evt);
    }

    public void channelIdle(ChannelHandlerContext ctx, IdleStateEvent e) throws Exception {
        //ECHO message
        //bsm = echoMessage();
        //ctx.channel().writeAndFlush(bsm);
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
        ctx.flush();
    }

    protected String getLocalNodeIdString() {
        return "local addr";
    }

    protected String getRemoteNodeIdString() {
        return "remote addr";
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
        if (cause instanceof ReadTimeoutException) {
            // read timeout
            log.error("[{}->{}] Disconnecting RPC node due to read timeout",
                         getLocalNodeIdString(), getRemoteNodeIdString());
            ctx.channel().close();
        } else if (cause instanceof ConnectException ||
        		cause instanceof IOException) {
            log.debug("[{}->{}] {}: {}", 
                         new Object[] {getLocalNodeIdString(),
                                       getRemoteNodeIdString(), 
                                       cause.getClass().getName(),
                                       cause.getMessage()});
        } else {
            log.error("[{}->{}] An error occurred on RPC channel",
                         new Object[]{getLocalNodeIdString(), 
                                      getRemoteNodeIdString(),
                                      cause});
            ctx.channel().close();
        }
    }
}
