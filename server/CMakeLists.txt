add_executable(server
   server.cpp
)

target_compile_features(server PUBLIC
   cxx_std_20
)

target_include_directories(server PUBLIC
   ${PROJECT_SOURCE_DIR}/asio/asio/include
)
