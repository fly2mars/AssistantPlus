#include "cvLogging.hpp"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

cvLogging* cvLogging::sOnly_ = 0;

cvLogging& cvLogging::gOnly()
{
	if (sOnly_ == 0) {
		sOnly_ = new cvLogging();
	}
	return *sOnly_;
}


void cvLogging::init_log(const std::string& name)
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log
    (
        keywords::file_name = std::string("log/") + name + std::string("-%Y%m%d-%H00.log"),
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%",
        keywords::auto_flush = true
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::trace
    );

    logging::add_common_attributes();
}

void cvLogging::trace(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(trace) << msg.c_str() << std::endl;
}
void cvLogging::debug(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(debug) << msg.c_str() << std::endl;
}
void cvLogging::info(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(info) << msg.c_str() << std::endl;
}
void cvLogging::warning(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(warning) << msg.c_str() << std::endl;
}
void cvLogging::error(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(error) << msg.c_str() << std::endl;
}
void cvLogging::fatal(const std::string& msg)
{
    BOOST_LOG_TRIVIAL(fatal) << msg.c_str() << std::endl;
}
