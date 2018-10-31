#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

enum class Message_type
{
	UNKNOWN = 0,
	REGISTER_USER = 1,
	REFRESH_USER_ADDRES = 2,
	GET_USER_PK = 3
};

class Message_body
{
private:
	std::string _raw;
public:
	virtual ~Message_body() = default;
};

class Register_user_body : public Message_body
{
private:
	std::string _login;
	std::string _crc;
public:
	Register_user_body(const char* data, const long data_size);
};

class Refresh_user_addres_body : public Message_body
{
private:
	std::string _login;
	std::string _crc;
public:
	Refresh_user_addres_body(const char* data, const long data_size);
};

class Get_user_info_body : public Message_body
{
private:
	std::string _login;
	std::string _crc;
public:
	Get_user_info_body(const char* data, const long data_size);
};

class Message
{
private:
	Message_type _type;
	Message_body* _body;

private:
	Message_type define_message_type(const char* data, const long data_size);
public:
	Message(const char* data, const long data_size);
	void deserialization(const char* data, const long data_size);
	void serialization(char* data, long& data_size);
	~Message();
};

#endif // MESSAGE_H
