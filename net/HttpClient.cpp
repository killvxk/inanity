#include "HttpClient.hpp"
#include "Service.hpp"
#include "TcpSocket.hpp"
#include "HttpStream.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <sstream>

BEGIN_INANITY_NET

class HttpClientRequest : public Object
{
private:
	ptr<File> requestFile;
	ptr<SuccessHandler> handler;
	ptr<HttpStream> outputStream;

public:
	HttpClientRequest(ptr<Service> service, const String& host, int port, ptr<File> requestFile, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream)
	: requestFile(requestFile), handler(handler), outputStream(HttpStream::CreateResponseStream(outputStream))
	{
		service->ConnectTcp(host, port, Service::TcpSocketHandler::Bind(MakePointer(this), &HttpClientRequest::OnConnect));
	}

private:
	void OnConnect(const Service::TcpSocketHandler::Result& result)
	{
		ptr<HttpClientRequest> self = this;

		ptr<TcpSocket> socket;
		try
		{
			socket = result.GetData();

			// установить принимающий обработчик
			socket->SetReceiveHandler(TcpSocket::ReceiveHandler::Bind(MakePointer(this), &HttpClientRequest::OnReceive));

			// отправить HTTP-запрос
			socket->Send(requestFile);
			requestFile = nullptr;
			// закрыть передающую сторону
			socket->End();
		}
		catch(Exception* exception)
		{
			if(socket)
				socket->Close();
			handler->FireError(exception);
		}
	}

	void OnReceive(const TcpSocket::ReceiveHandler::Result& result)
	{
		try
		{
			ptr<File> data = result.GetData();

			if(data)
				// пришли данные
				outputStream->OutputStream::Write(data);
			else
			{
				// корректный конец данных
				outputStream->End();
				if(outputStream->IsCompleted())
					handler->FireSuccess();
				else
					THROW("HTTP response is not completed");
			}
		}
		catch(Exception* exception)
		{
			// ошибка получения
			handler->FireError(exception);
		}
	}
};

void HttpClient::Fetch(ptr<Service> service, const String& url, const String& method, const String& data, const String& contentType, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream)
{
	BEGIN_TRY();

	// разобрать URL
	http_parser_url parsedUrl;
	if(http_parser_parse_url(url.c_str(), url.length(), 0, &parsedUrl))
		THROW("Can't parse url");
	String schema = "http";
	if(parsedUrl.field_set & (1 << UF_SCHEMA))
		schema = url.substr(parsedUrl.field_data[UF_SCHEMA].off, parsedUrl.field_data[UF_SCHEMA].len);
	String host;
	if(parsedUrl.field_set & (1 << UF_HOST))
		host = url.substr(parsedUrl.field_data[UF_HOST].off, parsedUrl.field_data[UF_HOST].len);
	int defaultPort = schema == "https" ? 443 : 80, port = defaultPort;
	if(parsedUrl.field_set & (1 << UF_PORT))
		port = parsedUrl.port;
	String path = "/";
	if(parsedUrl.field_set & (1 << UF_PATH))
		path = url.substr(parsedUrl.field_data[UF_PATH].off, parsedUrl.field_data[UF_PATH].len);
	String query = "";
	if(parsedUrl.field_set & (1 << UF_QUERY))
		query = url.substr(parsedUrl.field_data[UF_QUERY].off, parsedUrl.field_data[UF_QUERY].len);
	if(!query.empty())
		query = "?" + query;

	// сформировать запрос
	std::ostringstream request(std::ios::out | std::ios::binary);
	request << method << " " << path << query << " HTTP/1.1\r\n";
	request << "Connection: close\r\n";
	request << "Host: " << host;
	if(port != defaultPort)
		request << ":" << port;
	request << "\r\n";
	request << "User-Agent: Inanity HttpClient/2.0\r\n";
	if(contentType.length())
		request << "Content-Type: " << contentType << "\r\n";
	request << "Content-Length: " << data.length() << "\r\n";
	request << "\r\n";
	request << data;

	ptr<File> requestFile = Strings::String2File(request.str());

	MakePointer(NEW(HttpClientRequest(service, host, port, requestFile, handler, outputStream)));

	END_TRY("Can't fetch http");
}

void HttpClient::Get(ptr<Service> service, const String& url, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream)
{
	return Fetch(service, url, "", "", "", handler, outputStream);
}

END_INANITY_NET
