cmake_minimum_required(VERSION 2.8.7)
if(POLICY CMP0046)
  cmake_policy(SET CMP0046 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(NOT COMPILER_SUPPORTS_CXX11)
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if(USE_libstdcpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  message("-- Warning: forcing libstdc++ (controlled by USE_libstdcpp option in cmake)")
endif()

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# compile flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_REENTRANT -fPIC -Wall -Werror")

set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake/sanitizers ${PROJECT_ROOT_DIR}/cmake)
find_package(Sanitizers)

# ---[ opencv
set(OpenCV_FIND_QUIETLY true)
#find_package(OpenCV REQUIRED) //this will include opencv_ts which result in crash on centos
find_package(OpenCV REQUIRED core imgproc highgui features2d)
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  message(STATUS "opencv include: ${OpenCV_INCLUDE_DIRS}")
  message(STATUS "opencv libraries: ${OpenCV_LIBS}")
else()
  message(FATAL_ERROR "opencv not found!")
endif()

# ---[ add targets
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
add_executable(main main)
add_sanitizers(main)
if(NOT "${SAMPLES_DEPENDENCIES} " STREQUAL " ")
  add_dependencies(main ${SAMPLES_DEPENDENCIES})
endif()
target_link_libraries(main ${OpenCV_LIBS} ${GFLAGS_LIBRARIES} pthread dl)