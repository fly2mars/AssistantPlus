#pragma once
#include "config.h"
#include "suSocketServer.hpp"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <algorithm>

void default_resource_send(const  suServer<HTTP> &server, const std::shared_ptr< suServer<HTTP>::Response> &response,
	const std::shared_ptr<std::ifstream> &ifs) {
	//read and send 128 KB at a time
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
	resource[R"(^/\w+\?(wsdl|WSDL)$)"]["GET"] =
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) 
	{
		std::stringstream content_stream;
		content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
		content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
		for (auto& header : request->header) {
			content_stream << header.first << ": " << header.second << "<br>";
		}

		//find length of content_stream (length received using content_stream.tellp())
		content_stream.seekp(0, std::ios::end);

		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
	};


	//Post web service
	resource[R"(^/\w+$)"]["POST"] = 
		[this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) 
	{
		std::stringstream content_stream;
		content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
		content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
		for (auto& header : request->header) {
			content_stream << header.first << ": " << header.second << "<br>";
		}

		//find length of content_stream (length received using content_stream.tellp())
		content_stream.seekp(0, std::ios::end);

		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
	};


	//Default GET-example. If no other matches, this anonymous function will be called. 
	//Will respond with content in the web/-directory, and its subdirectories.
	//Default file: index.html
	//Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
	default_resource["GET"] =
		[this](std::shared_ptr<HttpServer::Response> response,
			std::shared_ptr<HttpServer::Request> request) {

		try {
			auto web_root_path = boost::filesystem::canonical("web");
			auto path = boost::filesystem::canonical(web_root_path / request->path);
			//Check if path is within web_root_path
			if (std::distance(web_root_path.begin(), web_root_path.end()) > std::distance(path.begin(), path.end()) ||
				!std::equal(web_root_path.begin(), web_root_path.end(), path.begin()))
				throw std::invalid_argument("path must be within root path");
			if (boost::filesystem::is_directory(path))
				path /= "index.html";
			if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
				throw std::invalid_argument("file does not exist");

			auto ifs = std::make_shared<std::ifstream>();
			ifs->open(path.string(), std::ifstream::in | std::ios::binary);

			if (*ifs) {
				ifs->seekg(0, std::ios::end);
				auto length = ifs->tellg();

				ifs->seekg(0, std::ios::beg);

				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
				default_resource_send(*this, response, ifs);
			}
			else
				throw std::invalid_argument("could not read file");
		}
		catch (const std::exception &e) {
			std::string content = "Could not open path " + request->path + ": " + e.what();
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		}
	};
}