#include <iostream>
#include <sw/redis++/redis++.h>

using namespace sw::redis;

int main()
{
try
{
	ConnectionOptions connection_options;
	connection_options.host = "127.0.0.1";
	connection_options.port = 7200;
	connection_options.password = "Jingxue714329@!.";  
	connection_options.db = 1; 
	connection_options.socket_timeout = std::chrono::milliseconds(200);

	Redis redis(connection_options);
	
	redis.set("key", "val");
   	auto val = redis.get("key");    
    	if (val)
	{
        	std::cout << *val << std::endl;
        }   
}
catch (const Error &e)
{
	//std::cout<<e<<std::endl;
}	

	
}
