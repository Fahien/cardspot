cmake_minimum_required( VERSION 3.15 )

# Dep
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake )

# Set the project name to your project name, my project isn't very descriptive
project( cardspot C CXX )

# Link this 'library' to set the c++ standard / compile-time options requested
add_library( project-options INTERFACE )
target_compile_features( project-options INTERFACE cxx_std_17 )

include( AddGfxSpot )
find_package( Boost REQUIRED system )
find_package( PkgConfig REQUIRED )
find_package( fmt REQUIRED )
find_package( CURL REQUIRED )

add_subdirectory( src )
add_subdirectory( test )
