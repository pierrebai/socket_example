option(multi_threaded_server "Control if the server example use multi-threaded accept." 0)

add_executable(server
   server.cpp
)

if(${multi_threaded_server})
   add_definitions(-DMULTI_THREADED_SERVER)
endif()

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
