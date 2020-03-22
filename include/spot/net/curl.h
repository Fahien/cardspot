#pragma once

#include <vector>
#include <string>

using CURL = void;

namespace spot::net
{

class Curl
{
  public:
	enum Result { OK };
	static std::string strerror( Result r );

	Curl();

	Curl( Curl&& );
	Curl& operator=( Curl&& );

	std::pair<std::vector<uint8_t>, Result> download( const std::string& url ) const;

  private:
	struct Global
	{
		Global();
		~Global();
	};

	static Global& get_global();

	CURL* handle = nullptr;
};

} // namespace spot::net
