.PHONY: all release target debug clean

DOCKER_IMAGE_NAME ?= go113-builder
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

all: release

release:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/build\
		${DOCKER_IMAGE}

target:
	go build -o ./build/target .

proto:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/build\
		${DOCKER_IMAGE} ./gen.sh hiwork

debug:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Debug\
		-v $(CURDIR):/build\

clean:
	-rm -rf build output

test:
	docker run --entrypoint "" --rm -it --network=host --privileged=true \
		-e CMAKE_BUILD_TYPE=Release \
		-v $(CURDIR):/build \
		${DOCKER_IMAGE} /bin/bash
