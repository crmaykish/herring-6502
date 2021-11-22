FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive 

RUN apt-get update --fix-missing && \
    apt-get install -y \
    wget \
    build-essential \
    cmake \
    sudo \
    python3 \
    libusb-dev \
    pkgconf

ARG UNAME=colin
ARG UID=1000
ARG GID=1000

RUN groupadd --gid $GID $UNAME
RUN useradd -m -u $UID -g $GID -s /bin/sh $UNAME

WORKDIR /mnt/pico

RUN wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh

RUN chmod +x pico_setup.sh

ENV SKIP_VSCODE=1
ENV SKIP_UART=1

RUN ./pico_setup.sh

WORKDIR /home/$UNAME/Workspace/