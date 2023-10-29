#ifndef BOOST_EXCEPTION_INLINE_H_INCLUDED
#define BOOST_EXCEPTION_INLINE_H_INCLUDED

#include <string>
#include <boost/exception/exception.hpp>
#include <boost/assert/source_location.hpp>

namespace boost
{
#ifdef BOOST_NO_EXCEPTIONS
BOOST_NORETURN void throw_exception(std::exception const& e) {
    throw std::exception(std::string("Boost exception occured!").c_str());
};

BOOST_NORETURN void throw_exception(std::exception const& e, boost::source_location const& loc) {
	throw std::exception(std::string("Boost exception with location occured!").c_str());
};
#endif
}// namespace boost\

#endif // #ifndef BOOST_EXCEPTION_INLINE_H_INCLUDED
