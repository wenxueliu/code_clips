package org.wenxueliu.zookeeper;

import java.io.IOException;

import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.LongNode;
import com.fasterxml.jackson.databind.node.IntNode;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;

import org.projectfloodlight.openflow.types.DatapathId;
import org.projectfloodlight.openflow.types.OFPort;
import org.projectfloodlight.openflow.types.U64;

class LinkDeserializer extends JsonDeserializer<Link> {

    @Override
    public Link deserialize(final JsonParser jp, final DeserializationContext ctxt) throws IOException, JsonProcessingException {
        final JsonNode node = jp.getCodec().readTree(jp);
        System.out.println(node.get("src"));
        System.out.println(node.get("srcPort"));
        System.out.println(node.get("dst"));
        System.out.println(node.get("dstPort"));
        System.out.println(node.get("latency"));
        final long src = node.get("src").get("long").asLong();
        final int srcPort = node.get("srcPort").get("portNumber").asInt();
        final int dstPort = node.get("dstPort").get("portNumber").asInt();

        System.out.println("src " + src + " " + node.get("src").get("long"));
        System.out.println("src port " + srcPort);
        //final int srcPort = (Integer)((IntNode)node.get("srcPort")).numberValue();
        //final long dst = (Long)((LongNode)node.get("dst")).longValue();
        //final int dstPort = (Integer)((IntNode)node.get("dstPort")).numberValue();
        //final long latency = (Long)((LongNode)node.get("latency")).longValue();
        return new Link(DatapathId.of(1L), OFPort.of(srcPort), DatapathId.of(2L), OFPort.of(1), U64.of(100));
    }
}

