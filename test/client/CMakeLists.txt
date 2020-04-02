set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/client.cc
)
add_executable( client-${PROJECT_NAME} ${SOURCES} )
target_link_libraries( client-${PROJECT_NAME} project-options Boost::system netspot gfxspot msgspot cardspot )
