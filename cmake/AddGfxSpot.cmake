# Add GfxSpot as a subdirectory if there is not a target already
set( GFXSPOT gfxspot )
set( GFXSPOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../gfx )
if( NOT TARGET ${GFXSPOT} )
	add_subdirectory( ${GFXSPOT_DIR} ${CMAKE_BINARY_DIR}/dep/gfx )
endif()
