#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <chrono>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cstdint>
#include <optional>


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
