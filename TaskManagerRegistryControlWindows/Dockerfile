FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    mingw-w64 \
    qt5-default \
    qt5-qmake \
    build-essential \
    git \
    && apt-get clean

WORKDIR /app

COPY . .

RUN /usr/lib/x86_64-linux-gnu/qt5/bin/qmake -spec win32-g++ TaskManagerAndRegistryControl.pro
RUN mingw32-make