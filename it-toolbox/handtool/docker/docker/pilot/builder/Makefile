.PHONY: all image clean

DOCKER_IMAGE_NAME ?= pilot
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

all: image 

image:
	docker build --network=host -t ${DOCKER_IMAGE} .

test:
	docker run --entrypoint "" --rm -it --network=host --privileged=true \
		-e CMAKE_BUILD_TYPE=Release \
		-v $(CURDIR):/workspace \
		${DOCKER_IMAGE} /bin/bash

clean:
	-rm -rf build
