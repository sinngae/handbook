.PHONY: all release1 debug1 release2 debug2 ssh-copy-id2

DOCKER_IMAGE1_NAME ?= oc1_libs
DOCKER_IMAGE1_TAG ?= master
DOCKER_IMAGE1=${DOCKER_IMAGE1_NAME}:${DOCKER_IMAGE1_TAG}
DOCKER_IMAGE2_NAME ?= oc2_toolchain
DOCKER_IMAGE2_TAG ?= master
DOCKER_IMAGE2=${DOCKER_IMAGE2_NAME}:${DOCKER_IMAGE2_TAG}
REMOTE_BUILD_IP ?= 10.9.193.35
REMOTE_BUILD_USER ?= ${USER}

all: release1 release2

release1:
	docker run --rm -it --network host --privileged=true\
		-e CMAKE_BUILD_TYPE=Release\
		-v $(CURDIR):/build\
		-v $(CURDIR)/output1:/build/build\
		${DOCKER_IMAGE1}

debug1:
	docker run --rm -it --network host --privileged=true\
		-e CMAKE_BUILD_TYPE=Debug\
		-v $(CURDIR):/build\
		-v $(CURDIR)/output1:/build/build\
		${DOCKER_IMAGE1}

release2:
	docker run --rm -it --network host --privileged=true -v $(CURDIR):/build \
		-e REMOTE_BUILD_TYPE=Release \
		-e REMOTE_BUILD_MODULE=`basename ${CURDIR}` \
		-e REMOTE_BUILD_USER=${REMOTE_BUILD_USER} \
	    -e REMOTE_BUILD_IP=${REMOTE_BUILD_IP} \
		-v $(CURDIR)/output2:/build/build\
		--entrypoint=/opt/build-remote\
		${DOCKER_IMAGE2}

debug2:
	docker run --rm -it --network host --privileged=true -v $(CURDIR):/build \
		-e REMOTE_BUILD_TYPE=Debug \
		-e REMOTE_BUILD_MODULE=`basename ${CURDIR}` \
		-e REMOTE_BUILD_USER=${REMOTE_BUILD_USER} \
	    -e REMOTE_BUILD_IP=${REMOTE_BUILD_IP} \
		-v $(CURDIR)/output2:/build/build\
		--entrypoint=/opt/build-remote\
		${DOCKER_IMAGE2}

ssh-copy-id2:
	docker run --rm --network=host --privileged=true --entrypoint=/bin/bash -it ${DOCKER_IMAGE2}\
		ssh-copy-id -o StrictHostKeyChecking=no ${REMOTE_BUILD_USER}@${REMOTE_BUILD_IP}
