cmake_minimum_required(VERSION 3.5)
project(VTRAY CXX)

# require a C++11 compiler
# use it for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
find_package(Threads REQUIRED)

# configure Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5Core REQUIRED)

set(src
  json.hpp json.cpp
  #ray.hpp ray.cpp
  rayHandler.hpp rayHandler.cpp
  threadsafequeue.hpp
  )


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# create the vtray executable
add_executable(vtray vtray.cpp ${src})
target_link_libraries(vtray Qt5::Gui)


