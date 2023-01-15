#cmake lowest version required
cmake_minimum_required(VERSION 3.1)

# 指定C++编译版本
set (CMAKE_CXX_STANDARD 14)

# Define project name
project(HelloSLAM)

#set(CMAKE_CXX_FLAGS "-std=c++14 -mfma")
# -O2 编译优化选项
# -g 使编译器生成gdb专用的调试信息
#set(CMAKE_CXX_FLAGS "-std=c++14 -O2 ${SSE_FLAGS} -g -march=native")
set(CMAKE_CXX_FLAGS "-std=c++14 -O2 ${SSE_FLAGS} -g ${CMAKE_CXX_FLAGS} -mpopcnt")

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
# find_package(OpenCV REQUIRED)

# find_package(Sophus REQUIRED)
# include_directories(${Sophus_INCLUDE_DIRS})

# find_package(glog REQUIRED)

# find_package(Pangolin REQUIRED)
# include_directories(${Pangolin_INCLUDE_DIRS})

# find_package(Ceres REQUIRED)
# include_directories(${CERES_INCLUDE_DIRS})

# find_package(g2o REQUIRED)
# include_directories(${G2O_INCLUDE_DIRS})

# find_package(Eigen3 REQUIRED)
# include_directories("/usr/include/eigen3")

# add pango head
#include_directories("/usr/include/pango-1.0")

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
# message(STATUS "OpenCV library status:")
# message(STATUS "    config: ${OpenCV_DIR}")
# message(STATUS "    version: ${OpenCV_VERSION}")
# message(STATUS "    libraries: ${OpenCV_LIBS}")
# message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(Find_the_Odd_Integer Find_the_Odd_Integer.cpp)
target_link_libraries(Find_the_Odd_Integer PRIVATE)







