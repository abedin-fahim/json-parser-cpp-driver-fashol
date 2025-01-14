cmake_minimum_required(VERSION 3.10)
project(my_json_driver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add this line to help find the shared library
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add dependencies
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(include)

# Create library
add_library(json_driver_lib SHARED
    src/encryption.cpp
)

# Create executable
add_executable(fashol-json-parser src/main.cpp)
target_link_libraries(fashol-json-parser PRIVATE 
    json_driver_lib
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Installation rules
install(TARGETS fashol-json-parser DESTINATION /usr/local/bin)
install(TARGETS json_driver_lib DESTINATION /usr/local/lib)
install(FILES include/encryption.h DESTINATION /usr/include/my-json-driver)
