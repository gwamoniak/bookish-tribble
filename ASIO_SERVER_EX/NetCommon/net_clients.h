#pragma once
#include "net_common.h"
#include "net_tsqeued.h"
#include "net_message.h"
#include "net_connection.h"



namespace net 
{
	template <typename T>
	class client_interface
	{
	public:
		client_interface() : m_socket(m_context)
		{

		}
		virtual ~client_interface() 
		{
			Disconnected();
		}

		void Disconnected()
		{
			if (IsConnected())
			{
				m_connection->Disconnect();
			}

			m_context.stop();

			if (threadContext.joinable())
				threadContext.join();

			m_connection.release();
		}

		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				

				asio::ip::tcp::resolver resolver(m_context);
				asio::ip::tcp::resolver::results_type m_endpoints = resolver.resolve(host, std::to_string(port));

				// create connection
				m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

				//connect to server
				m_connection->ConnectToServer(m_endpoints);

				//Start Context Thread
				threadContext = std::thread([this]() {m_context.run(); });

			}
			catch (const std::exception& e)
			{
				std::cerr << "Error in connection: " << e.what() << std::endl;
			}
			
			return false;
		}

		bool IsConnected()
		{
			if (m_connection)
				return m_connection->IsConnected();
			else
				return false;
		}
	public:
		void Send(const message<T>& msg)
		{
			if (IsConnected())
			{
				m_connection->Send(msg);
			}
		}

		tsqueued<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

	protected:
		// to handles data transfer......
		asio::io_context m_context;
		// just to execute its work commands
		std::thread threadContext;
		asio::ip::tcp::socket m_socket;
		// client should have a single instance
		std::unique_ptr<connection<T>> m_connection;
	
	private:
		tsqueued<owned_message<T>> m_qMessagesIn;
	};


}