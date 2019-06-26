# This Dockerfile provides a stable build environment for SAMD21 projects across platforms.

FROM ubuntu:18.04
LABEL maintainer="Patrick Rohr"

RUN apt-get update && apt-get install -y \
    apt-utils \
    build-essential \
    gcc-arm-none-eabi \
    git \
    python3.7 \
    software-properties-common

# There is a bug in gcc-arm-none-eabi on Ubuntu 18.04:
# https://bugs.launchpad.net/ubuntu/+source/newlib/+bug/1767223
# Installing libnewlib-arm-none-eabi and libnewlib-dev from bionic-proposed
RUN echo 'deb http://archive.ubuntu.com/ubuntu/ bionic-proposed restricted main multiverse universe' > /etc/apt/sources.list
RUN apt-get update && apt-get install -y -t bionic-proposed \
    libnewlib-arm-none-eabi \
    libnewlib-dev


# Install CMake
# TODO: use update-alternatives instead of direct symlink
ADD https://github.com/Kitware/CMake/releases/download/v3.15.0-rc2/cmake-3.15.0-rc2-Linux-x86_64.tar.gz /opt
RUN cd /opt && tar -xzf cmake-3.15.0-rc2-Linux-x86_64.tar.gz
RUN ln -s /opt/cmake-3.15.0-rc2-Linux-x86_64/bin/cmake /usr/bin/cmake

WORKDIR /app
