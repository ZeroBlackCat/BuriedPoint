#include "report/http_report.h"

#include "boost/asio/connect.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "spdlog/spdlog.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace buried{
	//对于后续要调用的所有I/O操作均需要该对象，可理解为IO操作的环境
	static boost::asio::io_context ioc;

	HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger)
		:logger_(logger) {}

	bool HttpReporter::Report() {
		try {
			int version = 11;
			//resolver和stream皆是用于执行I/O操作的对象
			tcp::resolver resolver(ioc);
			beast::tcp_stream stream(ioc);

			//解析域名
			tcp::resolver::query query(host_, port_);
			auto const results = resolver.resolve(query);
			//连接
			stream.connect(results);
			//设置http报文内容
			http::request<http::string_body> req{ http::verb::post, topic_, version };
			req.set(http::field::host, host_);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "application/json");
			req.body() = body_;
			req.prepare_payload();
			//发送
			http::write(stream, req);
			//声明存储数据的缓冲区和确定回复的变量
			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;
			//接收response报文
			http::read(stream, buffer, res);
			//若接收成功则关闭连接，并声明一个error_code确保关闭正常
			beast::error_code ec;
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);
			//判断是否关闭正常
			if (ec && ec != beast::errc::not_connected) throw beast::system_error{ ec };

			auto res_status = res.result();
			if (res_status != http::status::ok) {
				SPDLOG_LOGGER_ERROR(logger_,
					"report error " + std::to_string(res.result_int()));
				return false;
			}

			std::string res_body = boost::beast::buffers_to_string(res.body().data());
			//将发送成功写入logger
			SPDLOG_LOGGER_TRACE(logger_, "report success" +res_body);
		}
		catch (std::exception const& e) {
			SPDLOG_LOGGER_ERROR(logger_,
				"report error " + std::string(e.what()));
			return false;
		}
		return true;
	}
}