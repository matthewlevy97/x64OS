#!/bin/bash

docker_img=osdev_env
buildroot="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"

# Do we need to build the docker image?
if [[ ! $(docker image ls -q -f reference=${docker_img}) ]]; then
  echo "Docker image does not exist."
  echo "Do you wish to build it? (This could take a while)"
  read -r -p "[y/N]: " response
  case "$response" in
    [Yy][Ee][Ss]|[Yy])
      pushd ${buildroot}/toolchain
      docker build -t ${docker_img} .
      popd
      ;;
    *)
      exit
      ;;
  esac
fi

# Check if image already running
if [[ $(docker ps -q -f name=${docker_img}-run) ]]; then
  docker exec -it -u $(id -u):$(id -g) ${docker_img}-run "$@"
else
  docker run -it --rm -v ${buildroot}:/opt --name ${docker_img}-run -u $(id -u):$(id -g) ${docker_img} "$@"
fi
