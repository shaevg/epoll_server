#include "io_module.h"
#include <iostream>

using std::cout;
using std::endl;

int main()
{
	cout << "Start server" << endl;
	io_module _io_;
	_io_.start();
	cout << "Server started" << endl;

	std::getchar();
	cout << "Stop server" << endl;
	_io_.stop();

	cout << "Server stopped" << endl;
	return 0;
}
