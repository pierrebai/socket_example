add_executable(server
   server.cpp
)

if(WIN32)
   # Target Windows XP.
   add_definitions(-D_WIN32_WINNT=0x0501)
endif()

target_compile_features(server PUBLIC
   cxx_std_20
)

target_include_directories(server PUBLIC
   ${PROJECT_SOURCE_DIR}/asio/asio/include
)
