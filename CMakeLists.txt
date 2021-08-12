cmake_minimum_required(VERSION 2.8)
project(gbcc)


set(CMAKE_VERBOSE_MAKEFILE ON)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set(CCACHE_CMD "CCACHE_DIR=/tmp/ccachedir ccache")

  if(SLOW)
message("I am sleepy.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "sleep 1;  ${CCACHE_CMD}")
  else()
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_CMD}")
  endif()
endif(CCACHE_FOUND)




if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_FLAGS   "-std=c1z -Werror")
set(CMAKE_C_LINK_FLAGS   "")
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror")
set(CMAKE_CXX_LINK_FLAGS   "")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "")


if(EMSC)
#  set(PRELOADS "--preload-file ")


  set(CMAKE_CXX_COMPILER  /usr/local/emsdk/fastcomp/emscripten/em++)
  set(CMAKE_CXX_FLAGS "-Werror -std=c++1z -Wno-switch -Wno-unused-command-line-argument -Os")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
  set(EMSC_PORTS "-s USE_SDL=2 -s USE_ZLIB=1")
  set(CMAKE_CXX_LINK_FLAGS  "${EMSC_PORTS}") 

  set(BASIC_LIBS "")
else()
  set(BASIC_LIBS "stdc++")
endif()




add_subdirectory(libgbcc)


include_directories(./ /usr/local/include)
link_directories(/usr/local/lib)

add_executable("gbcc_test${EXECUTABLE_EXTENSION}" gbcc_test.cpp)

target_link_libraries("gbcc_test${EXECUTABLE_EXTENSION}" "gbcc" ${BASIC_LIBS})


if(EMSC)
endif()




