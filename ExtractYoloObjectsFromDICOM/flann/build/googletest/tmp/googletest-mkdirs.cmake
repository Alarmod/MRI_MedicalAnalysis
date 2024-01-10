# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest-build"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/tmp"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest-stamp"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src"
  "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest-stamp"
)

set(configSubDirs Release)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/My programs/ExtractYoloObjectsFromDICOM/flann/build/googletest/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
