#ifndef WEBEXCEPTION_HPP
#define WEBEXCEPTION_HPP

#include <exception>
#include <sstream>

class ConfException : public std::runtime_error
{
	public:
		ConfException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class BadSyntaxException : public std::runtime_error
{
	public:
		BadSyntaxException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class BadFieldException : public std::runtime_error
{
	public:
		BadFieldException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class BadArgumentException : public std::runtime_error
{
	public:
		BadArgumentException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class BadArgumentNumberException : public std::runtime_error
{
	public:
		BadArgumentNumberException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class LocationException : public std::runtime_error
{
	public:
		LocationException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

class WrongScopeException : public std::runtime_error
{
	public:
		WrongScopeException(std::string error_message) : std::runtime_error(error_message) {}
		const char *what() const throw()
		{
			return std::runtime_error::what();
		}
};

inline static std::string to_string(int val)
{
	std::stringstream s;
	s << val;
	return s.str();
}

#define ERROR std::string("Error : ")

#define ThrowBadArgument(token, field) throw BadArgumentException((ERROR + "`" + (token) + "' is not reconized in field `" + field + "'"))

#define ThrowBadArgumentNumber(field, max_nb_arg, is_too_much)  throw BadArgumentNumberException(ERROR + (field) + ((is_too_much) ? " has too much" : " does not have enough") + " arguments, requires " + to_string((max_nb_arg)))

#define ThrowBadFieldLocation(location, field) throw BadFieldException((ERROR + "`" + (location) + "' scope does not accept `" + (field) + "' field"))


#endif