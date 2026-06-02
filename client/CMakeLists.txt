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
)

add_executable(client
        main.cpp

        loginwindow.cpp
        loginwindow.h
        loginwindow.ui

        userwindow.cpp
        userwindow.h
        userwindow.ui

        shawindow.cpp
        shawindow.h
        shawindow.ui

        ../shared/crypto_utils.cpp
        ../shared/sha256.cpp
        ../shared/spline.cpp
        ../shared/gradient.cpp
        ../shared/msegraphwidget.cpp

        gradientwindow.cpp
        gradientwindow.h
        gradientwindow.ui

        splinewindow.cpp
        splinewindow.h
        splinewindow.ui
        ../shared/vigenere.h
        ../shared/vigenere.cpp
        vigenerewindow.cpp
        vigenerewindow.h
        vigenerewindow.ui
        ../shared/msegraphwidget.cpp
        ../shared/msegraphwidget.h
        msewindow.cpp
        msewindow.h
        msewindow.ui
        ../shared/msegraphwidget.h
        ../shared/msegraphwidget.cpp

)

target_link_libraries(client
        Qt6::Core
        Qt6::Widgets
)
target_include_directories(client PRIVATE
        ../shared
)