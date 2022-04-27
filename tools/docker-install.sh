#!/bin/bash

# update
sudo apt-get update && sudo apt-get upgrade

# download docker script
curl -fsSL https://get.docker.com -o get-docker.sh

# install docker
sudo sh get-docker.sh

# display information
docker version
docker info
