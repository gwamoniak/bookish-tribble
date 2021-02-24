#pragma once
#include "net_common.h"
#include "net_message.h"
#include "net_tsqeued.h"
#include "net_connection.h"


namespace net 
{
	template <typename T>
	class client_interface
	{
		client_interface() : m_socket(m_context)
		{

		}
		virtual ~client_interface() 
		{
			Disconnected();
		}

		void Disconnected()
		{

		}
	protected:
		// to handles data transfer......
		asio::io_context m_context;
		// just to execute its work commands
		std::thread threadContext;
	
	private:
		tsqueued<owned_message<T>> m_qMessagesIn;
	};


}