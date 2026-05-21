add_executable(client
        main.cpp
        ../shared/crypto_utils.cpp
)

target_include_directories(client PRIVATE
        ../shared
)