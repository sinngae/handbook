Envoy
----
# quick start

## docker
on windows:
```sh

docker pull envoyproxy/envoy:dev-5880c2d25399566a5376c274817ba9d86d2091a8

# gitbash
winpty docker run --rm -p 9901:9901 -p 10000:10000 -v ./envoy-my.yaml:/etc/envoy/envoy.yaml envoyproxy/envoy:dev-5880c2d25399566a5376c274817ba9d86d2091a8 -c /etc/envoy/envoy.yaml
winpty docker run --rm -p 9901:9901 -p 10000:10000 -v ./envoy-my.yaml:/etc/envoy/envoy.yaml envoyproxy/envoy:dev-5880c2d25399566a5376c274817ba9d86d2091a8 -c /etc/envoy/envoy.yaml

# docker auto 
 docker run --hostname=7b2f19020fd3 --mac-address=02:42:ac:11:00:02 --env=PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin --env=DEBIAN_FRONTEND=noninteractive -p 10000:10000 -p 9901:9901 --restart=no --label='org.opencontainers.image.ref.name=ubuntu' --label='org.opencontainers.image.version=20.04' --runtime=runc -t -d envoyproxy/envoy:dev-5880c2d25399566a5376c274817ba9d86d2091a8

# merge with the main configuration.
 --config-yaml
 

```