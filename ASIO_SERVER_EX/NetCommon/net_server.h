#pragma once

#include "net.h"
#include "net_tsqeued.h"
#include "net_message.h"
#include "net_connection.h"

namespace net
{
	template<typename T>
	class server_interface 
	{
	
	public:
		server_interface(uint16_t port)
			: m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
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
				return false;
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
					if (!ec)
					{
						std::cout << "[SERVER] connection... : " << socket.remote_endpoint() << "\n";
						std::shared_ptr<connection<T>> conn =
							std::make_shared<connection<T>>(connection<T>::owner::server,
								m_asioContext, std::move(socket), m_qMessagesIn);

						if (onClientConnect(conn))
						{
							// add  new connection
							m_deqConnections.push_back(std::move(conn));
							m_deqConnections.back()->ConnectToClient(nIDCounter++);

							std::count << "<-------> :ID=" << m_deqConnections.back()->GetID() << " Connection approved\n";
						}
						else
						{
							std::count << "<-------> Connection Denied \n";
						}
					}
					else
					{
						std::cout << "[SERVER] connection error: " << ec.message() << "\n";
					}
					// if the context is busy just wait for another connection
					WaitForClientConnection();
				});
		}
		// msg for specific client
		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			if (client && client->IsConnected())
			{
				client->Send(msg);
			}
			else
			{
				onClientDisconnected(client);
				client.reset();
				m_deqConnections.erase(
					std::remove(m_deqConnections.begin(), m_deqConnections.end(), client),
					m_deqConnections.end()
					);
			}
		}

		// msg for all clients
		void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoredClient = nullptr  )
		{
			
			// flag
			bool bInvalidClientExist = false;
			
			for (auto& client : m_deqConnections)
			{
				// client is connected
				if (client && client->IsConnected())
				{
					// it is
					if (client != pIgnoredClient)
						client->Send(msg);
				}
				else
				{
					onClientDisconnected(client);
					client.reset();
					bInvalidClientExist = true;
				}

				
			
			}

			if (bInvalidClientExist)
				m_deqConnections.erase(
					std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr),
					m_deqConnections.end()
					);


		}

		void Update(size_t nMaxMessages = -1, bool bWait = false)
		{
			
			if (bWait) m_qMessagesIn.wait();
			size_t nMessageCount = 0;
			while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
			{
				auto msg = m_qMessagesIn.pop_front(); // grab the front message
				OnMessage(msg.remote, msg.msg);
				nMessageCount++;
			}
		}

	protected:
		//clients connects
		virtual bool onClientConnect(std::shared_ptr<connection<T>> client)
		{
			return false;
		}
		// clients disconnected

		virtual bool onClientDisconnected(std::shared_ptr<connection<T>> client)
		{
			
		}
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
		{

		}

	protected:
		tsqueued<owned_message<T>> m_qMessagesIn;

		// container for validated connection
		std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

		asio::io_context m_asioContext;
		std::thread m_threadContext;

		// for asio context
		asio::ip::tcp::acceptor m_asioAcceptor;

		uint32_t nIDCounter = 10000;
	};


}
