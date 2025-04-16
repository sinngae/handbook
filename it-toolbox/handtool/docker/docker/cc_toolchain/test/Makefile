.PHONY: all release target debug clean

DOCKER_IMAGE_NAME ?= cc-builder
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

all: release

release:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/build\
		${DOCKER_IMAGE}

target:
	mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-Release} ..

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
