# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-src"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-build"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/tmp"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src"
  "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/bigred/Projects_C/TaskManager2/cmake-build-debug/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
