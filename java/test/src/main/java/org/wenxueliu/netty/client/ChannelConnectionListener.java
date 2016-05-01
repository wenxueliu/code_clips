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

import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * The listener class. Handles when the node disconnect.
 */
public class ChannelConnectionListener implements ChannelFutureListener {

    protected static final Logger log = LoggerFactory
            .getLogger(ChannelConnectionListener.class);

    /**
     * Constructor from a OvsdbProviderService providerService.
     */
    public ChannelConnectionListener() {
    }

    @Override
    public void operationComplete(ChannelFuture arg0) {
        log.info("Operation Complete");
    }
}
