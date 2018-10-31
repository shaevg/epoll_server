#include "message.h"

Message_type Message::define_message_type(const char* data, const long data_size)
{
	(void)data;
	(void)data_size;
	return Message_type::GET_USER_PK;
}

Message::Message(const char* data, const long data_size) : _body(nullptr)
{
	deserialization(data, data_size);
}

void Message::deserialization(const char* data, const long data_size)
{
	_type = define_message_type(data, data_size);

	switch (_type)
	{
	case Message_type::GET_USER_PK			: _body = new Get_user_info_body(data, data_size); break;
	case Message_type::REFRESH_USER_ADDRES	: _body = new Refresh_user_addres_body(data, data_size); break;
	case Message_type::REGISTER_USER		: _body = new Register_user_body(data, data_size); break;
	default: _body = nullptr; break;
	}
}

void Message::serialization(char* data, long& data_size)
{
	(void)data;
	(void)data_size;
}

Message::~Message()
{
	if (_body)
	{
		delete _body;
	}
}

Register_user_body::Register_user_body(const char* data, const long data_size)
{
	(void)data;
	(void)data_size;
}

Refresh_user_addres_body::Refresh_user_addres_body(const char* data, const long data_size)
{
	(void)data;
	(void)data_size;
}

Get_user_info_body::Get_user_info_body(const char* data, const long data_size)
{
	(void)data;
	(void)data_size;
}
