#pragma once

#include "net_common.h"
#include "net_tsqeued.h"
#include "net_message.h"

namespace net
{
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>> // shared_ptr*this
	{
	public:
		connection() {}
		virtual ~connection(){}
		
		bool ConnectToServer(); // only by servers
		bool Disconnect(); // servers and clients
		bool IsConnected() const;

		bool Send(const message<T>& msg);

	protected:

		//unique connection
		asio::ip::tcp::socket m_socket;

		asio::io_context& m_asioContext;

		// thread safe queue out and in
		tsqueued<message<T>> m_qMessagesOut;
		tsqueued<owned_message<T>> m_qMessagesIn;

	};

}

