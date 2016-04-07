
package org.wenxueliu.zookeeper;

public interface ISerializer<T> {

    public byte[] encode(T obj);
    public T decode(byte[] bytes);
}

