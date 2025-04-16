.PHONY: all release target run debug clean

DOCKER_IMAGE_NAME ?= pilot
DOCKER_IMAGE_TAG ?= master
DOCKER_IMAGE=${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}

all: release

release:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/workspace\
		-p 8000:8000\
		${DOCKER_IMAGE}

target:
	go build -o target cmd/api/main.go && \
	chmod +x target

run:
	mkdir -p /workspace/log && \
	supervisord -n -c deploy/rest.api.ini

debug:
	docker run --rm --network=host --privileged=true\
		-e CMAKE_BUILD_TYPE=Debug\
		-v $(CURDIR):/workspace\

clean:
	-rm -rf build output log target deploy/supervisord.pid deploy/supervisord.sock

test:
	docker run --entrypoint "" --rm -it --network=host --privileged=true \
		-e CMAKE_BUILD_TYPE=Release \
		-v $(CURDIR):/workspace \
		${DOCKER_IMAGE} /bin/bash
