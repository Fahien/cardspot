#include <cstdlib>

#include <spot/gfx/graphics.h>
#include <spot/gfx/png.h>
#include <spot/net/curl.h>
#include <fmt/format.h>

using namespace spot::gfx;
using namespace spot::math;

constexpr const float unit = 0.125f;


int32_t create_card( const std::string& icon, Graphics& gfx )
{
	int32_t group = gfx.models.create_node().index;

	spot::net::Curl curl;

	const std::string base_path = "https://game-icons.net/icons/ffffff/000000/1x1/";
	auto url = base_path + icon + ".png";

	auto [bytes, err] = curl.download( url );
	if ( err )
	{
		fmt::print( "{}\n", spot::net::Curl::strerror( err ) );
	}

	auto view = gfx.models.images.load( icon.c_str(), bytes );

	auto& node = gfx.models.create_node(
		Mesh::create_quad(
			gfx.models.create_material( view ).index
		),
		group
	);

	return group;
}

int main( int argc, const char** argv )
{
	std::string icon = "delapouite/labrador-head";
	if ( argc == 2 )
	{
		icon = argv[1];
	}
	auto gfx = Graphics();

	int32_t card = create_card( icon, gfx );

	gfx.view = look_at( Vec3::Z, Vec3::Zero, Vec3::Y );
	gfx.proj = ortho( -1.0f, 1.0, -1.0, 1.0f, 0.125f, 2.0 );

	double tick = 1.0;
	double time = 0.0f;

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );

		if ( gfx.render_begin() )
		{
			gfx.draw( card );
			gfx.render_end();
		}
	}

	gfx.device.wait_idle();

	return EXIT_SUCCESS;
}