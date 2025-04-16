.PHONY: all image clean

DOCKER_IMAGE_NAME ?= cc-builder
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

all: image 

copy_depends:
	rm -rf lib &&\
	rm -rf include &&\
	rm -rf static_lib &&\
	cp -r ../../lib . &&\
	cp -r ../../include . &&\
	cp -r ../../static_lib . 

image:
	docker build --network=host -t ${DOCKER_IMAGE} .

test:
	docker run --entrypoint "" --rm -it --network=host --privileged=true \
		-e CMAKE_BUILD_TYPE=Release \
		-v $(CURDIR):/build \
		${DOCKER_IMAGE} /bin/bash

clean:
	-rm -rf build
