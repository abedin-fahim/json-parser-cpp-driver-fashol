cmake_minimum_required(VERSION 3.10)
project(my_json_driver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
add_executable(json-parser src/main.cpp)
target_link_libraries(json-parser PRIVATE 
    json_driver_lib
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Installation rules
install(TARGETS json-parser DESTINATION /usr/local/bin)
install(TARGETS json_driver_lib DESTINATION /usr/local/lib)
install(FILES include/encryption.h DESTINATION /usr/include/my-json-driver)
