#include "board.h"

#include <spot/gfx/graphics.h>


namespace spot::gfx
{

SolidMaterials::SolidMaterials( Graphics& gfx )
: white { gfx.models.create_material( Color::white ).index }
, black { gfx.models.create_material( Color::black ).index }
, red   { gfx.models.create_material( Color::red  ).index }
, green { gfx.models.create_material( Color::green ).index }
, blue  { gfx.models.create_material( Color::blue  ).index }
{}


int32_t create_mesh( uint32_t material, Graphics& gfx )
{
	auto hs = unit / 2.0f - spacing / 2.0f;
	return gfx.models.create_mesh(
		Mesh::create_rect(
			math::Vec3( -hs, -hs, 0.0f ),
			math::Vec3( hs, hs, 0.0f ),
			material
		)
	).index;
}


SolidMeshes::SolidMeshes( Graphics& gfx )
: materials { gfx }
, black { create_mesh( materials.black, gfx ) }
, white { create_mesh( materials.white, gfx ) }
, red { create_mesh( materials.red, gfx ) }
, green { create_mesh( materials.green, gfx ) }
, blue { create_mesh( materials.blue, gfx ) }
{}


int32_t create_board( Graphics& gfx )
{
	auto block = gfx.models.create_node().index;

	auto add_child = [block, &gfx]( uint32_t mesh, math::Vec3 translation ) {
		auto& node = gfx.models.create_node( block );
		node.mesh = mesh;
		node.translation.x = translation.x;
		node.translation.y = translation.y;
	};

	size_t n = 16;
	for ( size_t col = 0; col < n; ++col )
	{
		uint32_t offset = col % 2;
		for ( size_t row = 0; row < n; ++row )
		{
			uint32_t color = ( row + offset ) % 2;
			add_child( color,
				math::Vec3(
					0.0f + unit / 2.0f + unit * row,
					0.0f + unit / 2.0f + unit * col
				)
			);
		}
	}

	return block;
}


} // namespace spot::gfx
