#pragma once
#include "net_common.h"

namespace net
{
	// message header is sent at start of all messages.
	// you can extract by using this template

	template <typename T>
	struct message_header
	{
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct message
	{
		message_header<T> header{};
		std::vector<uint8_t> body; // bytes

		size_t size() const
		{
			return sizeof(message_header<T>) + body.size();
		}

		// overide for std::cout using "friend" access by other functions
		// maybe use std::string_view or std::put and std::format in C++17
		friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
		{
			os << "ID: " << int(msg.header.id) << "Size: " << msg.header.size;
			return os;
		}

		// push any POD like data into the message buffer
		template<typename DataType>
		friend message<T>& operator << (message<T>& msg, const DataType& data)
		{
			// check that data pushed is trivially copyable
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

			// cache current size
			size_t i = msg.body.size();

			// resize the by the size of the data being pushed
			msg.body.resize(msg.body.size() + sizeof(DataType));

			// copy and recalculate the message size
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

			msg.header.size = msg.size();

			return msg;
		}

		template<typename DataType>
		friend message<T>& operator >> (message<T>& msg, const DataType& data)
		{
			// check that data pushed is trivially copyable
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

			// cache current size
			size_t i = msg.body.size() - sizeof(DataType);

			// resize the by the size of the data being pushed
			msg.body.resize(i);

			msg.header.size = msg.size();

			return msg;
		}


	};

	// forward declear the connection
	template <typename T>
	class connection;


	template <typename T>
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};

}
