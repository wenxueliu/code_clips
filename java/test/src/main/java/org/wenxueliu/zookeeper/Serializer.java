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

package org.wenxueliu.zookeeper;

import java.util.Arrays;
import java.util.List;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectReader;
import com.fasterxml.jackson.databind.ObjectWriter;
import com.fasterxml.jackson.databind.SerializationFeature;
import com.fasterxml.jackson.dataformat.smile.SmileFactory;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.module.SimpleModule;

import org.projectfloodlight.openflow.types.DatapathId;
//import com.google.common.collect.Lists;

/**
 * Interface for serialization of store artifacts.
 */
public class Serializer<T> implements ISerializer<T> {

    protected static final ObjectMapper mapper =
            new ObjectMapper(new SmileFactory());
    static {
        mapper.configure(SerializationFeature.ORDER_MAP_ENTRIES_BY_KEYS,
                         true);
        SimpleModule module = new SimpleModule();
        mapper.registerModule(module);
    }

    private final ObjectWriter valueWriter;
    private final ObjectReader valueReader;

    private final boolean valueAsTree;

    public Serializer(Class<T> valueClass) {
        if (valueClass.isAssignableFrom(JsonNode.class)) {
            valueAsTree = true;
            this.valueWriter = null;
            this.valueReader = null;
        } else {
            valueAsTree = false;
            this.valueWriter = mapper.writerWithType(valueClass);
            this.valueReader = mapper.reader(valueClass);
        }
    }

    public Serializer(TypeReference<T> valueType) {
        valueAsTree = false;
        this.valueWriter = mapper.writerWithType(valueType);
        this.valueReader = mapper.reader(valueType);
    }

    private byte[] getBytes(T value) {
        if (value == null) {
            throw new IllegalArgumentException("Cannot get null object");
        }

        try {
            byte[] v = null;
            if (valueAsTree)
                v = mapper.writeValueAsBytes(value);
            else
                v = valueWriter.writeValueAsBytes(value);

            //if (logger.isTraceEnabled()) {
            //    logger.trace("Converted value {} to {}", 
            //                 value, Arrays.toString(v));
            //}
            return v;
        } catch (Exception e) {
            System.out.println("getBytes error:" + e.getMessage());
            //throw new SerializationException(e);
            return null;
        }
    }

    @SuppressWarnings("unchecked")
    private T getObject(byte[] value) {
        try {
            if (value == null) return null;
            if (valueAsTree)
                return (T)mapper.readTree(value);
            else
                return valueReader.readValue(value);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            //throw new SerializationException(e);
            return null;
        }
    }

    //private List<Versioned<V>> convertValues(List<Versioned<byte[]>> values) 
    //        throws SyncException {
    //    if (values != null) {
    //        List<Versioned<V>> objectvalues =
    //            new ArrayList<Versioned<V>>(values.size());
    //        for (Versioned<byte[]> vb : values) {
    //            objectvalues.add(new Versioned<V>(getObject(vb.getValue()),
    //                    vb.getVersion()));
    //        }
    //        return objectvalues;        
    //    }
    //    return null;
    //}

    /**
     * Serialize the specified object.
     * @param object object to serialize.
     * @return serialized bytes.
     * @param <T> encoded type
     */
    @Override
    public byte[] encode(T obj) {
        return getBytes(obj);
    }

    /**
     * Deserialize the specified bytes.
     * @param bytes byte array to deserialize.
     * @return deserialized object.
     * @param <T> decoded type
     */
    @Override
    public T decode(byte[] bytes) {
        return getObject(bytes);
    }

    ///**
    // * Creates a new Serializer instance from a KryoNamespace.
    // *
    // * @param kryo kryo namespace
    // * @return Serializer instance
    // */
    //static Serializer using(KryoNamespace kryo) {
    //    return using(Arrays.asList(kryo));
    //}

    ///**
    // * Creates a new Serializer instance from a KryoNamespace and some additional classes.
    // *
    // * @param namespace kryo namespace
    // * @param classes variable length array of classes to register
    // * @return Serializer instance
    // */
    //static Serializer using(KryoNamespace namespace, Class<?>... classes) {
    //    return using(Arrays.asList(namespace), classes);
    //}

    ///**
    // * Creates a new Serializer instance from a list of KryoNamespaces and some additional classes.
    // *
    // * @param namespaces kryo namespaces
    // * @param classes variable length array of classes to register
    // * @return Serializer instance
    // */
    //static Serializer using(List<KryoNamespace> namespaces, Class<?>... classes) {
    //    KryoNamespace.Builder builder = new KryoNamespace.Builder();
    //    for(KryoNamespace kns : namespaces) {
    //        builder.register(kns);
    //    }
    //    for(Class<?> cls : classes) {
    //        builder.register(cls);
    //    }
    //    KryoNamespace namespace = builder.build();
    //    return new KryoSerializer() {
    //        //@Override
    //        public <T> byte[] encode(T object) {
    //            return namespace.serialize(object);
    //        }

    //        //@Override
    //        public <T> T decode(byte[] bytes) {
    //            return namespace.deserialize(bytes);
    //        }
    //    };
    //}

    //static Serializer forTypes(Class<?>... classes) {
    //    return using(KryoNamespace.newBuilder()
    //                              .register(classes)
    //                              .build());
    //}
}
