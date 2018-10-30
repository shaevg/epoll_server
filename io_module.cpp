#include "io_module.h"

io_module::io_module(unsigned short int port):
	_socket(0),
	_epoll_fd(0),
	_port(port),
	_state(io_module_state::INITIAL),
	_listen_thread(nullptr),
	_handle_thread(nullptr)
{

}

void io_module::start()
{
	if (_state != io_module_state::RUN && !_listen_thread && !_handle_thread)
	{
		init_server();
		_state = io_module_state::RUN;
		_listen_thread = new std::thread(&io_module::start_listen, this);
		_handle_thread = new std::thread(&io_module::handle_events, this);
	}
}

void io_module::stop()
{
	if (_state == io_module_state::RUN)
	{
		_state = io_module_state::STOP;

		if (_listen_thread)
		{
			printf("listen stop");
			_listen_thread->join();
		}

		if (_handle_thread)
		{
			printf("handle stop");
			_handle_thread->join();
		}

		delete _listen_thread;
		delete _handle_thread;

		_listen_thread = nullptr;
		_handle_thread = nullptr;
	}
}

void io_module::init_server()
{
	// Create server socket
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		throw 1;
	}

	// by default, system doesn't close socket, but set it's status to TIME_WAIT; allow socket reuse
	int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		throw 2;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET; // don't care IPv4 or IPv6
	serv_addr.sin_addr.s_addr = INADDR_ANY; // receive packets destined to any of the interfaces
	serv_addr.sin_port = htons(_port);
	// bind socket to address
	if (bind(_socket, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
	{
		close(_socket);
		throw 3;
	}

	// create epoll instance
	if ((_epoll_fd = epoll_create(5)) == -1)
	{
		perror("epoll_create");
		throw 4;
	}
}

void io_module::handle_events()
{
	while (_state == io_module_state::RUN)
	{
		int ready = epoll_wait(_epoll_fd, _evlist, _MAX_EVENTS, -1);
		if (ready == -1)
		{
			perror("epoll_wait");
			//continue;
		}

		char request[_MAX_BUF] = "";
		for (int i = 0; i < ready; ++i)
		{
			char buf[_MAX_BUF];
			int _client_socket = _evlist[i].data.fd;

			auto byte_count = read(_client_socket, buf, _MAX_BUF);
			if (byte_count == -1)
			{
				perror("read");
			}
			else
			{
				memcpy(request, buf, static_cast<size_t>(byte_count));
				puts(" ******** REQUEST ******** ");
				puts(request);
			}

			if (send(_client_socket, request, static_cast<size_t>(byte_count), 0) == -1)
			{
				perror("send error");
				close(_socket);
				close(_client_socket);
				throw 1;
			}
			puts(" ******** RESPONSE ******** ");
			puts(request);

			close(_client_socket);
		}
	}
}

void io_module::start_listen()
{
	if (listen(_socket, 1) == -1)
	{
		close(_socket);
		throw 1;
	}

	sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	while (_state == io_module_state::RUN)
	{
		// wait for incoming connection
		int _client_socket = accept(_socket, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_size);
		if (_client_socket == -1)
		{
			close(_socket);
			throw 2;
		}

		epoll_event _event;
		_event.data.fd = _client_socket;
		_event.events = EPOLLIN;

		epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _client_socket, &_event);
	}
}
