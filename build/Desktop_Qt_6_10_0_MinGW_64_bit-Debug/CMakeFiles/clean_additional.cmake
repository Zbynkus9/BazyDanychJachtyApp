# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BazyDanychJachtyApp_autogen"
  "CMakeFiles\\BazyDanychJachtyApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BazyDanychJachtyApp_autogen.dir\\ParseCache.txt"
  )
endif()
