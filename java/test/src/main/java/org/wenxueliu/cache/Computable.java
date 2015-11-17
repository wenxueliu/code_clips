package org.wenxueliu.cache;

public interface Computable<A, V> {
    V compute(A arg) throws InterruptedException;
}
