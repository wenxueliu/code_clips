//package net.floodlightcontroller.monitor;
//
//import java.util.List;
//
//import org.sdnplatform.sync.Versioned;
//import org.sdnplatform.sync.error.SyncException;
//import org.sdnplatform.sync.thrift.KeyedValues;
//import java.net.SocketException;
//
//
///*
// * Represent a reply to a remote message
// */
//public class SyncReply {
//    private List<KeyedValues> keyedValues;
//    private List<Versioned<byte[]>> values;
//    private boolean success;
//    private SocketException error;
//    private int intValue;
//
//    public SyncReply(List<Versioned<byte[]>> values,
//                        List<KeyedValues> keyedValues,
//                        boolean success, SocketException error, int intValue) {
//        super();
//        this.values = values;
//        this.keyedValues = keyedValues;
//        this.success = success;
//        this.error = error;
//        this.intValue = intValue;
//    }
//
//    public int getIntValue() {
//        return intValue;
//    }
//    public List<KeyedValues> getKeyedValues() {
//        return keyedValues;
//    }
//    public List<Versioned<byte[]>> getValues() {
//        return values;
//    }
//    public SocketException getError() {
//        return error;
//    }
//    public boolean isSuccess() {
//        return success;
//    }
//}
