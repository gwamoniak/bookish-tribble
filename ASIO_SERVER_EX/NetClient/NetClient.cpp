#include <iostream>
#include <net.h>

enum class CustomMsgTypes : uint32_t
{
	X_ray_Online,
	X_ray_Offline

};

class CustomClient : public net::client_interface<CustomMsgTypes>
{

public:
	bool X_ray_Offline()
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::X_ray_Offline;
		msg << "X-ray is Offline";
		Send(msg);
	}
};
int main()
{
	CustomClient client;
	
	
	return 0;
}