#pragma once
#include <string>
class cvLogging
{
public:
	static cvLogging& gOnly();
	//cvLogging();
	void init_log(const std::string& name);
	void trace(const std::string& msg);
	void debug(const std::string& msg);
	void info(const std::string& msg);
	void warning(const std::string& msg);
	void error(const std::string& msg);
	void fatal(const std::string& msg);
private:
	static cvLogging* sOnly_;
	cvLogging() {};
};
#define cvTrace(msg)\
    cvLogging::gOnly().trace(msg)
#define cvDebug(msg)\
    cvLogging::gOnly().debug(msg)
#define cvInfo(msg)\
    cvLogging::gOnly().info(msg)
#define cvWarning(msg)\
    cvLogging::gOnly().warning(msg)
#define cvError(msg)\
    cvLogging::gOnly().error(msg)
#define cvFatal(msg)\
    cvLogging::gOnly().fatal(msg)