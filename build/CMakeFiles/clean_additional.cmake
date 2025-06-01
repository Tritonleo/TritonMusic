# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TritonMusic_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TritonMusic_autogen.dir\\ParseCache.txt"
  "TritonMusic_autogen"
  )
endif()
