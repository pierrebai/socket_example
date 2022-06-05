add_executable(client
   client.cpp
)

target_compile_features(client PUBLIC
   cxx_std_20
)

target_include_directories(client PUBLIC
   ${PROJECT_SOURCE_DIR}/asio/asio/include
)
