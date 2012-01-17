#include "http_server.h"

#include <iostream>
#include "../../engine/logger.h"

#define PORT 80

using namespace http_server;
using namespace engine;

int main()
{

	while(true)
	{
		try
		{
			cHttpServer _http_server(PORT);
			_http_server.Start();
		}
		catch(std::exception& e)
		{
			cLogger log(LOG_SEV_ERROR);
			log<< e.what();
			continue;
		}
		return 0;
	}

};
