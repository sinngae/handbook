## grpc
`git clone https://github.com/grpc/grpc.git`<br>
`cd grpc`<br>
`git submodule update --init`<br>
`cd third_party/protobuf`<br>
`vim autogen.sh`<br>
+ curl $curlopts -O https://googlemock.googlecode.com/files/gmock-1.7.0.zip
<br>修改为<br>
+ curl $curlopts  -L -o gmock-1.7.0.zip https://github.com/peter-wangxu/gMock/archive/1.7.0.zip
`./autogen.sh`<br>
`./configure --prefix=/home/local`<br>
`make && make install`<br>
`ldconfig`<br>
`protoc --version`<br>
`cd ../..`<br>
`make && make install`<br>
`vim activate.sh`<br>
+ DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
+ export PATH=$PATH:$DIR/bins/opt:$DIR/bins/opt/protobuf
+ export CPATH=$DIR/include:$DIR/third_party/protobuf/src
+ export LIBRARY_PATH=$DIR/libs/opt:$DIR/libs/opt/protobuf
+ export PKG_CONFIG_PATH=$DIR/libs/opt/pkgconfig:$DIR/third_party/protobuf
+ export LD_LIBRARY_PATH=$DIR/libs/opt
`source activate.sh`
`cd examples/cpp/helloworld/`
`make`
