#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "src/third_package/boost/asio/connect.hpp"
#include "src/third_package/boost/asio/ip/tcp.hpp"
#include "src/third_package/boost/beast/core.hpp"
#include "src/third_package/boost/beast/http.hpp"
#include "src/third_package/boost/beast/version.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

TEST(BuriedHttpTest, DISABLE_HttpPost) {
	try {
		auto const host = "localhost";
		auto const target = "/buried";
		int version = 11;

		net::io_context ioc;

		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		tcp::resolver::query query(host, "8765");

		auto const result = resolver.resolve(query);

		stream.connect(result);

		http::request<http::string_body> req{http::verb::post, target, version};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		req.set(http::field::content_type, "application/json");
		req.body() = "{\"hello\":\"world\"}";
		req.prepare_payload();

		http::write(stream, req);

		beast::flat_buffer buffer;

		http::response<http::dynamic_body> res;

		http::read(stream, buffer, res);

		std::string bdy = boost::beast::buffers_to_string(res.body().data());
		std::cout << "bdy " << bdy << std::endl;
		std::cout << "res " << res << std::endl;
		std::cout << "res code " << res.result_int() << std::endl;

		beast::error_code ec;
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		if (ec && ec != beast::errc::not_connected) throw beast::system_error{ ec };
		
	}
	catch (std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
	}
}