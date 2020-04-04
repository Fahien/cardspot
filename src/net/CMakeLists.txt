set( SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/curl.cc )
add_library( netspot ${SOURCES} )
target_include_directories( netspot PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_link_libraries( netspot project-options ${CONAN_LIBS} )
