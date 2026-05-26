FROM ubuntu:24.04

# Отключаем интерактивные диалоги при установке пакетов
ENV DEBIAN_FRONTEND=noninteractive

# Обновляем зависимости и доставляем g++, cmake, make, PostgreSQL dev библиотеки и модули Qt6
RUN apt update && apt install -y \
    g++ \
    cmake \
    make \
    libpq-dev \
    qt6-base-dev \
    libqt6sql6-psql \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем исходники проекта
COPY . .

# Переходим в папку сервера, собираем проект через CMake
RUN mkdir -p server/build && cd server/build && \
    cmake .. && \
    make

# Меняем команду запуска контейнера на исполняемый файл сервера
CMD ["./server/build/server"]
