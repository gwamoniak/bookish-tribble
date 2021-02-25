#pragma once

#include "net.h"

namespace net
{
	template<typename T>
	class server_interface 
	{
		server_interface(uint16_t port)
			:m_asioAcceptor(m_asioContext,asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{
			
		}
		virtual ~server_interface()
		{
			Stop();
		}

		bool Start()
		{
			try
			{
				WaitForClientConnection();

				m_threadContext = std::thread([this]() {m_asioContext.run(); });
			}
			catch (const std::exception& e)
			{
				std::cerr << "[SERVER] Exception: " << e.what() << "\n";
				retrun false;
			}

			std::cout << "[SERVER] Started! \n";
			return true;
		}

		void Stop()
		{
			m_asioContext.stop();
			//tidy up the context
			if (m_threadContext.joinable)
				m_threadContext.join();

			std::cout << "[SERVER] Stoped! \n";
		}

		//ASYNC
		void WaitForClientConnection()
		{
			// this it might not be safe ???
			m_asioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) 
			{
					if(!ec)
					{
						std::cout << "[SERVER] connection... : " << socket.remote_endpoint() << "\n";
						std::shared_ptr<connection<T>> conn =
							std::make_shared<connection<T>>(connection<T>::owner::server,
								m_asioContext, std::move(socket), m_qMessagesIn);

						if (onClientConnect(conn))
						{
							// next time
						}
					}
					else
					{
						std::cout << "[SERVER] connection error: "<< ec.message() << "\n";
					}
					// if the context is busy just wait for another connection
					WaitForClientConnection();
			})
		}
		// msg for specific client
		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			
		}

		// msg for all clients
		void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoredClient = nullptr > )
		{

		}
	protected:
		//clients connects
		virtual bool onClientConnect(std::shared_ptr<connection<T>> client)
		{
			return false;
		}
		// clients disconnected
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
		{

		}

		tsqueued<owned_message<T>> m_qMessagesIn;

		asio::io_context m_asioContext;
		std::thread m_threadContext;

		// for asio context
		asio::ip::tcp::acceptor m_asioAcceptor;

		uint32_t nIDCounter = 10000;
	};


}
