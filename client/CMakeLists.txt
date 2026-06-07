cmake_minimum_required(VERSION 3.20)

project(client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        Network
)

add_executable(client
        main.cpp

        networkmanager.cpp
        networkmanager.h

        loginwindow.cpp
        loginwindow.h
        loginwindow.ui

        userwindow.cpp
        userwindow.h
        userwindow.ui

        adminwindow.cpp
        adminwindow.h

        shawindow.cpp
        shawindow.h
        shawindow.ui

        gradientwindow.cpp
        gradientwindow.h
        gradientwindow.ui

        splinewindow.cpp
        splinewindow.h
        splinewindow.ui

        vigenerewindow.cpp
        vigenerewindow.h
        vigenerewindow.ui

        msewindow.cpp
        msewindow.h
        msewindow.ui

        gradientgraphwidget.cpp
        gradientgraphwidget.h

        ../shared/crypto_utils.cpp
        ../shared/sha256.cpp
        ../shared/spline.cpp
        ../shared/gradient.cpp
        ../shared/vigenere.h
        ../shared/vigenere.cpp
        ../shared/msegraphwidget.cpp
        ../shared/msegraphwidget.h
        ../shared/splinegraphwidget.cpp
        ../shared/splinegraphwidget.h
)

target_link_libraries(client
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

target_include_directories(client PRIVATE
        .
        ../shared
)