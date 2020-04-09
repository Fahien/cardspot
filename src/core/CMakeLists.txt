set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/log.cc
)
add_library( corespot ${SOURCES} )
target_link_libraries( corespot project-options CONAN_PKG::fmt )
target_include_directories( corespot PUBLIC ${CMAKE_SOURCE_DIR}/include )
