add_subdirectory( net )

set( LIB_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/card.cc
)

add_library( ${CMAKE_PROJECT_NAME} ${LIB_SOURCES} )
target_include_directories( ${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_link_libraries( ${CMAKE_PROJECT_NAME} project-options gfxspot netspot CONAN_PKG::boost CONAN_PKG::fmt )

set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

add_executable( game-${CMAKE_PROJECT_NAME} ${SOURCES} )
target_link_libraries( game-${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME} )
