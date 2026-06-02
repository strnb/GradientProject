cmake_minimum_required(VERSION 3.16)
project(GradientServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую обработку метаобъектов Qt (MOC, user интерфейсы, ресурсы)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск компонентов Qt (поддерживает как Qt5, так и Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

# Конфигурируем исполняемый файл "server" со всеми исходниками
add_executable(server
        main.cpp
        mytcpserver.cpp
        databasemanager.cpp
        ../shared/crypto_utils.cpp
        ../shared/sha256.cpp
)

# Подключаем директории с заголовочными файлами
target_include_directories(server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../shared
)

# Линкуем модули Qt к нашему серверу
target_link_libraries(server PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql
)
