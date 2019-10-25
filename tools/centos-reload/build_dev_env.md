## grpc
`git clone https://github.com/grpc/grpc.git`
`cd grpc`
`git submodule update --init`
`cd third_party/protobuf`
`vim autogen.sh`
+ curl $curlopts -O https://googlemock.googlecode.com/files/gmock-1.7.0.zip
<br>修改为<br>
+ curl $curlopts  -L -o gmock-1.7.0.zip https://github.com/peter-wangxu/gMock/archive/1.7.0.zip
`./autogen.sh`
`./configure --prefix=/home/local`
`make && make install`
`ldconfig`
`protoc --version`
`cd ../..`
`make && make install`
`vim activate.sh`
	DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
	export PATH=$PATH:$DIR/bins/opt:$DIR/bins/opt/protobuf
	export CPATH=$DIR/include:$DIR/third_party/protobuf/src
	export LIBRARY_PATH=$DIR/libs/opt:$DIR/libs/opt/protobuf
	export PKG_CONFIG_PATH=$DIR/libs/opt/pkgconfig:$DIR/third_party/protobuf
	export LD_LIBRARY_PATH=$DIR/libs/opt
`source activate.sh`
`cd examples/cpp/helloworld/`
`make`
