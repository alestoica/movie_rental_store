# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\movie_rental_store_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\movie_rental_store_autogen.dir\\ParseCache.txt"
  "movie_rental_store_autogen"
  )
endif()
