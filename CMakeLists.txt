cmake_minimum_required(VERSION 2.8.11)

# Name of the project
project(swep1rcr_fix)

# Specify output locations for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Collect source and header files
set(HEADERS
  src/wrapper/myIDirect3D3.h
  src/wrapper/myIDirect3DDevice3.h
  src/wrapper/myIDirectDraw.h
  src/wrapper/myIDirectDraw4.h
  src/wrapper/wrapper.h
  src/proxy.h
  src/util.h
  src/exports.def
)

set(SOURCES
  src/wrapper/myIDirect3D3.cpp
  src/wrapper/myIDirect3DDevice3.cpp
  src/wrapper/myIDirectDraw.cpp
  src/wrapper/myIDirectDraw4.cpp
  src/wrapper/wrapper.cpp
  src/main.cpp
  src/proxy.cpp
  src/util.cpp
)

# Create VS filters representing the underlying folder structure
# http://stackoverflow.com/questions/15989634
foreach(f ${HEADERS} ${SOURCES})
  # Extract the folder, ie remove the filename part
  get_filename_component(SRCGR ${f} DIRECTORY)

  # Source_group expects \\ (double antislash), not / (slash)
  string(REPLACE / \\ SRCGR ${SRCGR})
  source_group("${SRCGR}" FILES ${f})
endforeach()

# Create the library
add_library(ddraw_wrapper SHARED ${HEADERS} ${SOURCES})

# Set output name to ddraw.dll
set_target_properties(ddraw_wrapper PROPERTIES OUTPUT_NAME ddraw)
