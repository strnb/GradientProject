add_executable(server
        main.cpp
        ../shared/crypto_utils.cpp
        ../shared/sha256.cpp
)

target_include_directories(server PRIVATE
        ../shared
)