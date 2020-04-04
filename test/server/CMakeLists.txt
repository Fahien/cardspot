set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/connection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/server.cc
)
add_executable( server-${PROJECT_NAME} ${SOURCES} )
target_link_libraries( server-${PROJECT_NAME} project-options netspot gfxspot msgspot cardspot )
