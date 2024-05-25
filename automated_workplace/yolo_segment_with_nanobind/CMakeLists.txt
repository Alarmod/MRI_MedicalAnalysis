CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

project(yolo_segment_with_nanobind LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release")
endif()

#add_definitions(/arch:AVX2)

#find python
find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)

#find nanobind
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

#find opencv
find_package(OpenCV 4.9 REQUIRED)

#setup build configuration & compile
nanobind_add_module(yolo_segment_ext STABLE_ABI NB_STATIC yolo_segment_with_nanobind.cpp)

target_include_directories(yolo_segment_ext PRIVATE "${OpenCV_INCLUDE_DIRS}")
target_link_libraries(yolo_segment_ext PRIVATE "${OpenCV_LIBS}")

target_include_directories(yolo_segment_ext PRIVATE "${ONNX_DIR}/include")
target_link_libraries(yolo_segment_ext PRIVATE "${ONNX_DIR}/lib/onnxruntime.lib")

install(TARGETS yolo_segment_ext LIBRARY DESTINATION ./)