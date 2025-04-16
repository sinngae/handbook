.PHONY: all release target debug clean

DOCKER_IMAGE_NAME ?= go113-builder
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

PROTO_FILES = $(wildcard ./proto/*.proto)
PB_GO_FILES = $(patsubst ./proto/%.proto, ./go/%.pb.go, $(PROTO_FILES))

all: release

release:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/build\
		${DOCKER_IMAGE}

target: ${PB_GO_FILES} ./docs/apidoc.html

$(PB_GO_FILES):./go/%.pb.go:./proto/%.proto
	protoc -I=${GOPATH}/src -I=${GOPATH}/src/github.com/gogo/protobuf/protobuf -I=./proto/ \
	--go_out=./go --go-grpc_out=./go/ --govalidators_out=./go/ $^

./docs/apidoc.html:${PROTO_FILES}
	protoc --doc_out=../docs --doc_opt=html,apidoc.html $^



debug:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Debug\
		-v $(CURDIR):/build\

clean:
	-rm -rf build output
	-rm -rf ./go/*
	-rm -rf ./python/*
	-rm -rf ./docs/*

test:
	docker run --entrypoint "" --rm -it --network=host --privileged=true \
		-e CMAKE_BUILD_TYPE=Release \
		-v $(CURDIR):/build \
		${DOCKER_IMAGE} /bin/bash
