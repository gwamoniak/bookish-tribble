#pragma warning(disable : 4996)



//
//std::vector<char> vBuffer(1*1024); // define the size of the buffer
//
//void GrabSomeData(asio::ip::tcp::socket& socket)
//{
//	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
//		[&](std::error_code ec, std::size_t length)
//		{
//			if (!ec)
//			{
//				std::cout << "\n\nRead " << length << " bytes\n\n";
//				for (size_t i = 0; i < length; i++)
//				{
//					std::cout << vBuffer[i];
//				}
//
//				GrabSomeData(socket);
//			}
//		});
//}
//
//int main()
//{
//
//	asio::error_code ec;
//
//	// create a "contex" - essentially the platform specific interface
//	asio::io_context context;
//
//	// stall the context for a while
//	asio::io_context::work idleWork(context);
//
//	//start the context
//	std::thread threadContext = std::thread([&]() {context.run();});
//
//	// get address - example.com "93.184.216.34"
//	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);
//	
//	//socket creation
//	asio::ip::tcp::socket socket(context);
//
//	//connect
//	socket.connect(endpoint, ec);
//
//	if (!ec)
//	{
//		std::cout << "connected" << std::endl;
//	}
//	else
//	{
//		std::cerr << "Failed to connect to address: " << ec.message() << std::endl;
//	}
//	
//	if (socket.is_open())
//	{
//		GrabSomeData(socket);
//		
//		std::string sRequest =
//			"GET /index.html HTTP/1.1\r\n"
//			"Host: example.com\r\n"
//			"Connection: close\r\n\r\n";
//		
//		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
//
//		//for debugin purposes
//		using namespace std::chrono_literals;
//		std::this_thread::sleep_for(20000ms);
//		
//		context.stop();
//		if (threadContext.joinable()) threadContext.join();
//
//	}
//
//	system("pause");
//	return 0;
//}
//
//
