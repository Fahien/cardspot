set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/client.cc
	${CMAKE_CURRENT_SOURCE_DIR}/connection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/board.cc
)
add_executable( client-${PROJECT_NAME} ${SOURCES} )
target_link_libraries( client-${PROJECT_NAME} project-options ${CONAN_LIBS} netspot gfxspot msgspot cardspot )
