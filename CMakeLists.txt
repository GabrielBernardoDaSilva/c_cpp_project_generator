CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)

PROJECT(CCppGeneratorProject)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-value -Wno-unused-result -Wno-unused-label -Wno-unused-local-typedefs -Wno-unused-macros -Wno-unused-const-variable -Wno-unused-const-variable -Wno-unused-const-parameter -Wno-unused-const-variable -Wno-unused-cons ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(CCppGeneratorProject src/main.cpp)

target_compile_features(CCppGeneratorProject PUBLIC cxx_std_23)


install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)