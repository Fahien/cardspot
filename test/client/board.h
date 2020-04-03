#pragma once

#include <spot/gfx/viewport.h>

#define NODE( index ) ( gfx.models.get_node( index ) )


namespace spot::gfx
{

static const float unit = 1.0f;
static const Viewport viewport = Viewport( math::Vec2::zero, math::Vec2( unit * 16.0f, unit * 16.0f ) );
static const float spacing = unit / 16.0f;

class Graphics;


struct SolidMaterials
{
	SolidMaterials( Graphics& gfx );

	const int32_t black;
	const int32_t white;
	const int32_t red;
	const int32_t green;
	const int32_t blue;
};


int32_t create_mesh( uint32_t material, Graphics& gfx );


struct SolidMeshes
{
	SolidMeshes( Graphics& gfx );

	const SolidMaterials materials;

	const int32_t black;
	const int32_t white;
	const int32_t red;
	const int32_t green;
	const int32_t blue;
};


// 2x2 block
int32_t create_board( Graphics& gfx );


} // namespace spot::gfx
