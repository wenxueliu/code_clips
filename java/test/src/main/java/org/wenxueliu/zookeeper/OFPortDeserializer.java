package org.wenxueliu.zookeeper;

import java.io.IOException;

import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.LongNode;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;

import org.projectfloodlight.openflow.types.OFPort;

class OFPortDeserializer extends JsonDeserializer<OFPort> {

    @Override
    public OFPort deserialize(final JsonParser jp, final DeserializationContext ctxt) throws IOException, JsonProcessingException {
        final JsonNode node = jp.getCodec().readTree(jp);
        final int port = node.get("portNumber").asInt();
        System.out.println("port " + port);
        return OFPort.of(port);
    }
}

