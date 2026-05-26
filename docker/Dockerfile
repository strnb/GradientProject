FROM ubuntu:24.04

RUN apt update && apt install -y \
    g++ \
    cmake \
    make \
    libpq-dev

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

CMD ["bash"]