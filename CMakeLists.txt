cmake_minimum_required( VERSION 3.15 )

# Dep
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# CardSpot
project( cardspot
	VERSION 0.1
	LANGUAGES C CXX )

# Conan
# Download automatically, you can also just copy the conan.cmake file
if( NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake" )
	message( STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan" )
	file( DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
		"${CMAKE_BINARY_DIR}/conan.cmake" )
endif()

include( ${CMAKE_BINARY_DIR}/conan.cmake )

conan_cmake_run(
	REQUIRES fmt/6.1.2
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing )


# Link this 'library' to set the c++ standard / compile-time options requested
add_library( project-options INTERFACE )
target_compile_features( project-options INTERFACE cxx_std_17 )

include( AddGfxSpot )
find_package( Boost REQUIRED system )
find_package( PkgConfig REQUIRED )
find_package( CURL REQUIRED )

add_subdirectory( src )
add_subdirectory( test )
