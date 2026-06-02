# БЫЛО: FROM ubuntu:24.04
# СТАЛО:
FROM cr.yandex/mirror/ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    g++ \
    cmake \
    make \
    libpq-dev \
    qt6-base-dev \
    qt6-base-plugins \
    libqt6sql6-psql \
    && rm -rf /var/lib/apt/lists/*

ENV QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt6/plugins

WORKDIR /app

COPY . .

RUN mkdir -p server/build && cd server/build && \
    cmake .. && \
    make

CMD ["./server/build/server"]