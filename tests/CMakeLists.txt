cmake_minimum_required(VERSION 3.20)
project(GradientProjectTests)

set(CMAKE_CXX_STANDARD 17)

# ─── Google Test ─────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ─── Общий код проекта (без Qt-зависимостей) ─────────────────────────────────
set(SHARED_DIR "${CMAKE_SOURCE_DIR}/shared")

add_library(gradient_lib STATIC
    ${SHARED_DIR}/gradient.cpp
)
target_include_directories(gradient_lib PUBLIC ${SHARED_DIR})
target_compile_definitions(gradient_lib PUBLIC _USE_MATH_DEFINES)

add_library(sha256_lib STATIC
    ${SHARED_DIR}/sha256.cpp
    ${SHARED_DIR}/crypto_utils.cpp
)
target_include_directories(sha256_lib PUBLIC ${SHARED_DIR})

add_library(vigenere_lib STATIC
    ${SHARED_DIR}/vigenere.cpp
)
target_include_directories(vigenere_lib PUBLIC ${SHARED_DIR})

add_library(spline_lib STATIC
    ${SHARED_DIR}/spline.cpp
)
target_include_directories(spline_lib PUBLIC ${SHARED_DIR})

# ─── Тестовые исполняемые файлы ──────────────────────────────────────────────
enable_testing()

# Тесты градиентного спуска
add_executable(test_gradient test_gradient.cpp)
target_link_libraries(test_gradient gradient_lib GTest::gtest_main)
target_compile_definitions(test_gradient PRIVATE _USE_MATH_DEFINES)
add_test(NAME GradientTests COMMAND test_gradient)

# Тесты SHA-256 и hashPassword
add_executable(test_sha256 test_sha256.cpp)
target_link_libraries(test_sha256 sha256_lib GTest::gtest_main)
add_test(NAME SHA256Tests COMMAND test_sha256)

# Тесты шифра Виженера
add_executable(test_vigenere test_vigenere.cpp)
target_link_libraries(test_vigenere vigenere_lib GTest::gtest_main)
add_test(NAME VigenereTests COMMAND test_vigenere)

# Тесты сплайн-интерполяции
add_executable(test_spline test_spline.cpp)
target_link_libraries(test_spline spline_lib GTest::gtest_main)
add_test(NAME SplineTests COMMAND test_spline)

# Единый исполняемый файл со всеми тестами
add_executable(test_all
    test_gradient.cpp
    test_sha256.cpp
    test_vigenere.cpp
    test_spline.cpp
)
target_link_libraries(test_all
    gradient_lib sha256_lib vigenere_lib spline_lib
    GTest::gtest_main
)
target_compile_definitions(test_all PRIVATE _USE_MATH_DEFINES)
add_test(NAME AllTests COMMAND test_all)