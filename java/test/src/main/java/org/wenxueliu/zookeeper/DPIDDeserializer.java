package org.wenxueliu.zookeeper;

import java.io.IOException;

import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.LongNode;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;

import org.projectfloodlight.openflow.types.DatapathId;

class DPIDDeserializer extends JsonDeserializer<DatapathId> {

    @Override
    public DatapathId deserialize(final JsonParser jp, final DeserializationContext ctxt) throws IOException, JsonProcessingException {
        final JsonNode node = jp.getCodec().readTree(jp);
        final long dpid = node.get("long").asLong();
        System.out.println("dpid " + dpid);
        return DatapathId.of(dpid);
    }
}
