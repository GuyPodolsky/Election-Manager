#pragma once
#include <exception>

using namespace std;

class invalid_command : public exception {
	const char* what_arg;
public:
	invalid_command(const char* _what_arg) : what_arg(_what_arg) {}
	virtual const char* what() const noexcept { return what_arg; }
};

class invalid_input : public exception {
	const char* what_arg;
public:
	invalid_input(const char* _what_arg) : what_arg(_what_arg) {}
	virtual const char* what() const noexcept { return what_arg; }
};


class item_not_found : public exception {
	const char* what_arg;
public:
	item_not_found(const char* _what_arg) : what_arg(_what_arg) {}
	virtual const char* what() const noexcept { return what_arg; }
};

class elections_exception : public exception {
	string what_arg;
public:
	elections_exception(const char* _what_arg) : what_arg(_what_arg) {}
	elections_exception(string _what_arg) : what_arg(_what_arg) {}
	virtual const char* what() const noexcept { return what_arg.c_str(); }
};

class filesystem_exception : public exception {
	const char* what_arg;
private:
	const string file_name;
public:
	filesystem_exception(const char* _what_arg,const string& _filename) : what_arg(_what_arg), file_name(_filename) {}
	filesystem_exception(const char* _what_arg) : what_arg(_what_arg), file_name(nullptr) {}
	virtual const char* what() const noexcept { return what_arg; }
	const string& filename() const{ return file_name; }
};

class allocation_failed : public bad_alloc {
	const char* what_arg;
private:
	const string message;
public:
	allocation_failed(const char* _what_arg, const string& _message) : what_arg(_what_arg), message(_message) {}
	allocation_failed(const char* _what_arg) : what_arg(_what_arg), message(nullptr) {}
	virtual const char* what() const noexcept { return what_arg; }
	const string& Message() const { return message; }
};