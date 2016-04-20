namespace java org.wenxueliu.thrift.service

include "ThriftStruct.thrift"

service ThriftService {
    string helloString(1:string param)
    i32 helloInt(1:i32 param)
    bool helloBoolean(1:bool param)
    void helloVoid()
    string helloNull()
    i32 add(1:i32 n1, 2:i32 n2)
    i32 size(1:list<ThriftStruct.ThriftStruct> structs)
    bool sendSet(1:set<ThriftStruct.ThriftStruct> structs)
    bool sendMap(1:map<string, ThriftStruct.ThriftStruct> structs)
}
