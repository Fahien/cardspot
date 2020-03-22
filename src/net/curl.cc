#include "spot/net/curl.h"

#include <curl/curl.h>


namespace spot::net
{


Curl::Global::Global()
{
	curl_global_init( CURL_GLOBAL_ALL );
}


Curl::Global::~Global()
{
	curl_global_cleanup();
}


Curl::Global& Curl::get_global()
{
	static Curl::Global global;
	return global;
}


std::string Curl::strerror( const Curl::Result r )
{
	get_global();
	return curl_easy_strerror( static_cast<CURLcode>( r ) );
}


size_t write_data( void* buffer, size_t size, size_t nmemb, void* payload )
{
	auto& bytes = *reinterpret_cast<std::vector<uint8_t>*>( payload );

	auto bytes_size = bytes.size();
	bytes.resize( bytes_size + nmemb );
	std::memcpy( bytes.data() + bytes_size, buffer, nmemb );

	return nmemb;
}


Curl::Curl()
{
	get_global();
	handle = curl_easy_init();
	curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, write_data );
}


Curl::Curl( Curl&& other )
: handle { other.handle }
{
	other.handle = nullptr;
}


Curl& Curl::operator=( Curl&& other )
{
	std::swap( handle, other.handle );
	return *this;
}


std::pair<std::vector<uint8_t>, Curl::Result> Curl::download( const std::string& url ) const
{
	curl_easy_setopt( handle, CURLOPT_URL, url.c_str() );
	std::vector<uint8_t> ret;
	curl_easy_setopt( handle, CURLOPT_WRITEDATA, &ret );
	auto res = curl_easy_perform( handle );
	return { std::move( ret ), static_cast<Result>( res ) };
}


} // namespace spot::net
