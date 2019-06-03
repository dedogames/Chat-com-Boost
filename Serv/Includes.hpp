#pragma once

#define BOOST_USE_WINAPI_VERSION 0x0600
/* std*/
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include<thread>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>


/* Boost*/
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
using namespace std;
using boost::asio::ip::tcp;

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

/* App */

/* Common includes */
#include "ChatMessage.hpp"
 


/* Server Includes*/
#include "Person.hpp"
#include "Room.hpp"
#include "Connection.hpp"
#include "Server.hpp"
