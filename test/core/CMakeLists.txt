set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/message.cc
	${CMAKE_CURRENT_SOURCE_DIR}/log.cc
)
add_library( msgspot ${SOURCES} )
target_link_libraries( msgspot project-options fmt::fmt )
target_include_directories( msgspot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
