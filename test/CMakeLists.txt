function( add_proj_test TEST )
	set( SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.cc )
	add_executable( test-${TEST}-${PROJECT_NAME} ${SOURCES} )
	target_link_libraries( test-${TEST}-${PROJECT_NAME} project-options netspot gfxspot cardspot )
endfunction()

add_proj_test( asio )
add_proj_test( curl )

add_subdirectory( server )
add_subdirectory( client )
