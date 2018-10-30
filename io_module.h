#ifndef IO_MODULE_H
#define IO_MODULE_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <thread>
#include <mutex>

class io_module
{
private:
	enum class io_module_state {INITIAL, RUN, STOP};
	static const unsigned int _MAX_EVENTS = 1;
	static const unsigned int _MAX_BUF = 1024;
	static const unsigned short int _DEFAULT_PORT = 7000;
	const char* _response =
		 "HTTP/1.1 200 OK\nContent-Type: text/html;charset=utf-8 \n\n"
		 "<!doctype html>"
		 "<html>"
		 "<head>"
			"<meta charset=\"UTF-8\">"
			"<title>Document</title>"
		 "</head>"
		 "<body>"
			"ваш запрос: <strong>1</strong>"
		 "</body>"
		 "</html>\n";

private:
	int _socket;
	int _epoll_fd;
	unsigned short int _port;
	io_module_state _state;
	epoll_event _evlist[_MAX_EVENTS];

private:
	std::thread* _listen_thread;
	std::thread* _handle_thread;

private:
	void init_server();
	void handle_events();
	void start_listen();

public:
	io_module(unsigned short int port = _DEFAULT_PORT);
	void start();
	void stop();
};

#endif // IO_MODULE_H
