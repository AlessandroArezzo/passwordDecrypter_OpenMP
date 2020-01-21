cmake_minimum_required(VERSION 3.6)
project(passwordDecrypter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_C_COMPILER "usr/local/Cellar/gcc/")

# in Linux just use -fopenmp, without -lomp and -Xpreprocessor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp -lomp")
# the following 2 lines are needed on macOS.
# update paths as required
include_directories(/usr/local/opt/libomp/include)
link_directories(/usr/local/opt/libomp/lib)
include_directories(/usr/local/Cellar/openssl@1.1/1.1.1d/include)
link_directories(/usr/local/Cellar/openssl@1.1/1.1.1d/lib)



set(SOURCE_FILES main.cpp Decrypter.cpp Decrypter.h)
add_executable(passwordDecrypter ${SOURCE_FILES})
target_link_libraries(passwordDecrypter crypto)