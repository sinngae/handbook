# Protobuf
业务层和传输层之间序列化和反序列化的网络通讯协议。
也可以用作数据格式存储到分布式缓存。

压缩比高，效率高；使用简单，提供兼容，易维护；可读性差，加密性好；跨平台，跨编程语言，且提供了不同语言的代码自动生成工具；
## 原理
编码后的二进制数据由一个个feild组成，每个feild由tag和value组成，如果field没有赋值，编码的时候不会被写入。tag包含了field-num（5位）、wire-type（3位）信息`Tag = (field_number << 3) | wire_type`。

protobuf使用**varint编码**编码整数，具有压缩效果，其编码高位有特殊含义：如果是1，表示后续的字节也是该数字的一部分；如果是0，表示这是最后一个字节，且剩余7位都用来表示数字（字节顺序由低到高处理，数值型是变长的，除非使用fixed整形型、浮点型或其他）。负数时，以ZigZag辅助编码。整数由tag和value组成。

string类型的value使用UTF编码，由tag-length-value组成。

**Base 128 Varints 编码（压缩算法）**

**zigzag**
负数编码压缩/补码/取反/再使用varint编码

**ProtoBuf消息升级原则**
在实际的开发中会存在这样一种应用场景，既消息格式因为某些需求的变化而不得不进行必要的升级，但是有些使用原有消息格式的应用程序暂时又不能被立刻升级，这便要求我们在升级消息格式时要遵守一定的规则，从而可以保证基于新老消息格式的新老程序同时运行。规则如下：
1. 不要修改已经存在字段的标签号。
2. 任何新添加的字段必须是optional和repeated限定符，否则无法保证新老程序在互相传递消息时的消息兼容性。
3. 在原有的消息中，不能移除已经存在的required字段，optional和repeated类型的字段可以被移除，但是他们之前使用的标签号必须被保留，不能被新的字段重用。
4. int32、uint32、int64、uint64和bool等类型之间是兼容的，sint32和sint64是兼容的，string和bytes是兼容的，fixed32和sfixed32，以及fixed64和sfixed64之间是兼容的，这意味着如果想修改原有字段的类型时，为了保证兼容性，只能将其修改为与其原有类型兼容的类型，否则将打破新老消息格式的兼容性。


## 对比
### protobuf与thrift
thrift嵌入thrift RPC服务框架，没有独立的序列化及反序列化接口，很难与其他协议灵活嵌套（如HTTP）。
protobuf提供了IDL和IDL编译器及编码和解析的API，对开发人员友好。压缩比高，编码解析速度也快。
protobuf非常适用于对性能要求高的RPC调用，还可以嵌入HTTP，利用HTTP的良好的跨防火墙的访问。也非常适用于压缩比高，编码解析速度要求快的持久化场景。

### protobuf与Avro
Avro hadoop持久化首选？静态类型语言vs动态类型语言

### XML SOAP协议？

### json
易于调试，可读。
非常适用于浏览器的Ajax或手机APP端与服务器之间的通讯。

## proto 发版问题
从test分支会有“proto协议比release分支先行”这个问题，gitlab/protocenter这种方式有同样的问题，除非我们用test/uat/release/feature 这套gitflow来维护proto协议发版。但是用这套gitflow，会导致proto协议更容易发生冲突（proto字段下标冲突），而且不同环境需要各维护一套proto版本，更容易出问题。所以，一般还是选择一个分支来打tag这种方式。至于如果对test分支有疑问，我们做个专用的proto分支也是一样的，feature分支里修改go.mod来选择不同的tag就行。关键的问题还是做到proto协议兼容，新增字段的下标单调增长且唯一，修改字段仅限于校验部分。