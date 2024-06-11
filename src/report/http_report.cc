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
	//���ں���Ҫ���õ�����I/O��������Ҫ�ö��󣬿����ΪIO�����Ļ���
	static boost::asio::io_context ioc;

	HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger)
		:logger_(logger) {}

	bool HttpReporter::Report() {
		try {
			int version = 11;
			//resolver��stream��������ִ��I/O�����Ķ���
			tcp::resolver resolver(ioc);
			beast::tcp_stream stream(ioc);

			//��������
			tcp::resolver::query query(host_, port_);
			auto const results = resolver.resolve(query);
			//����
			stream.connect(results);
			//����http��������
			http::request<http::string_body> req{ http::verb::post, topic_, version };
			req.set(http::field::host, host_);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "application/json");
			req.body() = body_;
			req.prepare_payload();
			//����
			http::write(stream, req);
			//�����洢���ݵĻ�������ȷ���ظ��ı���
			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;
			//����response����
			http::read(stream, buffer, res);
			//�����ճɹ���ر����ӣ�������һ��error_codeȷ���ر�����
			beast::error_code ec;
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);
			//�ж��Ƿ�ر�����
			if (ec && ec != beast::errc::not_connected) throw beast::system_error{ ec };

			auto res_status = res.result();
			if (res_status != http::status::ok) {
				SPDLOG_LOGGER_ERROR(logger_,
					"report error " + std::to_string(res.result_int()));
				return false;
			}

			std::string res_body = boost::beast::buffers_to_string(res.body().data());
			//�����ͳɹ�д��logger
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