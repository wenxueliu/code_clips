package org.wenxueliu.zookeeper;

import java.io.IOException;

import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.LongNode;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;

import org.projectfloodlight.openflow.types.U64;

class U64Deserializer extends JsonDeserializer<U64> {

    @Override
    public U64 deserialize(final JsonParser jp, final DeserializationContext ctxt) throws IOException, JsonProcessingException {
        final JsonNode node = jp.getCodec().readTree(jp);
        final long latency = node.get("value").asLong();
        System.out.println("latency :" + latency);
        return U64.of(latency);
    }
}

