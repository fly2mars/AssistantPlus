#pragma once
#include "config.h"
#include "suSocketServer.hpp"

#include <fstream>
#include "config.h"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <algorithm>

#include "ServerSet.h"
#include "suHttpParser.h"

suLock sulock;
void default_resource_send(const  suServer<HTTP> &server, const std::shared_ptr< suServer<HTTP>::Response> &response,
	const std::shared_ptr<std::ifstream> &ifs) {
	//read and send 128 KB at a time
	suLocker locker(sulock);
	static std::vector<char> buffer(131072); // Safe when server is running on one thread
	std::streamsize read_length;
	if ((read_length = ifs->read(&buffer[0], buffer.size()).gcount()) > 0) {
		response->write(&buffer[0], read_length);
		if (read_length == static_cast<std::streamsize>(buffer.size())) {
			server.send(response, [&server, response, ifs](const boost::system::error_code &ec) {
				if (!ec)
					default_resource_send(server, response, ifs);
				else
					std::cerr << "Connection interrupted" << std::endl;
			});
		}
	}
}




void suServer<HTTP> ::init()
{
	//server?wsdl
	resource[R"(^/(ws|WS)_\w+\?(wsdl|WSDL)$)"]["GET"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		try {
			suHttpParser httpParser;
			//remove /ws_***?wsdl
			std::string service_name = request->path.substr(4, request->path.find_last_of('?') - 4);

			suService* pService = CServerSet::gOnly().GetModule(service_name);
			if (pService != NULL)
			{
				httpParser.genWsdlResponse(pService->wsdl());
			}

			*response << httpParser.m_strResponse;

			//std::stringstream content_stream;

			//content_stream << httpParser.m_strResponse;

			/*content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
			content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
			for (auto& header : request->header) {
			content_stream << header.first << ": " << header.second << "<br>";
			}*/

			//find length of content_stream (length received using content_stream.tellp())
			//content_stream.seekp(0, std::ios::end);

			//*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();

		}
		catch (const std::exception &e) {
			std::string content = "Could not open path " + request->path + ": " + e.what();
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		}

	};

	//Upload file
	//Post web service
	resource[R"(^/(upload|UPLOAD)$)"]["POST"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		std::stringstream content_stream;
		content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
		content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
		for (auto& header : request->header) {
			content_stream << header.first << ": " << header.second << "<br>";
		}

		content_stream << "<h1>Content</h1>";
		content_stream << request->content.string().c_str();
		//find length of content_stream (length received using content_stream.tellp())
		content_stream.seekp(0, std::ios::end);

		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
	};

	//Post web service
	resource[R"(^/(ws|WS)_\w+$)"]["POST"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		std::stringstream content_stream;


		//remove "/ws_"
		std::string service_name = request->path.substr(4, request->path.size() - 4);
		suHttpParser httpParser;

		suService* pService = CServerSet::gOnly().GetModule(service_name);
		if (pService != NULL)
		{
			httpParser.getSoapAction(request->content.string());
			std::string strResult = pService->run(httpParser.m_postParameter);  // need copy the result, otherwise it will lead to segmentation falt(invalid memory access) on linux

			httpParser.genPostResonse(strResult);
			httpParser.bKeepAlive_ = false;
			//httpParser.genPostResonse(pService->run(httpParser.m_requestFilename,httpParser.m_postParameter));
		}
		*response << httpParser.m_strResponse;

		//std::cout << httpParser.m_strResponse << std::endl;

		//content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
		//content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
		//for (auto& header : request->header) {
		//	content_stream << header.first << ": " << header.second << "<br>";
		//}

		//content_stream << "<h1>Content</h1>";
		//content_stream << request->content.string().c_str();
		////find length of content_stream (length received using content_stream.tellp())
		//content_stream.seekp(0, std::ios::end);

		//*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
	};


	//Default GET-example. If no other matches, this anonymous function will be called. 
	//Will respond with content in the web/-directory, and its subdirectories.
	//Default file: index.html
	//Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
	default_resource["GET"] =
		[this](std::shared_ptr<HttpServer::Response> response,
			std::shared_ptr<HttpServer::Request> request) {

		try {
			suHttpParser parser;
			boost::filesystem::current_path(boost::filesystem::path(CServerSet::gOnly().GetDir()));
			auto web_root_path = boost::filesystem::canonical(CServerSet::gOnly().GetWebRootDir());
			//web_root_path = boost::filesystem::path(CServerSet::gOnly().GetWebRootDir());
			auto path = boost::filesystem::canonical(web_root_path / request->path);
			//Check if path is within web_root_path
			if (std::distance(web_root_path.begin(), web_root_path.end()) > std::distance(path.begin(), path.end()) ||
				!std::equal(web_root_path.begin(), web_root_path.end(), path.begin()))
				throw std::invalid_argument("path must be within root path");
			if (boost::filesystem::is_directory(path))
				path /= "index.html";
			if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
				throw std::invalid_argument("file does not exist");  //todo: return 404

			auto ifs = std::make_shared<std::ifstream>();
			ifs->open(path.string(), std::ifstream::in | std::ios::binary);

			if (*ifs) {
				ifs->seekg(0, std::ios::end);
				auto length = ifs->tellg();

				ifs->seekg(0, std::ios::beg);

				suString filename = path.string().c_str();
				suString ext = filename.FindExtension();
				ext = ext.SubString(1, ext.Length() - 1);
				std::string strMineType = parser.m_MimeTypes[ext.CString()];
				if (strMineType.empty()) strMineType = "application/octet-stream";

				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n"
					<< "Content-Type: " << strMineType << "\r\n\r\n";
				default_resource_send(*this, response, ifs);
			}
			else {
				throw std::invalid_argument("could not read file");
			}

		}
		catch (const std::exception &e) {
			std::string content = "Could not open path " + request->path + ": " + e.what();
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		}
	};
}