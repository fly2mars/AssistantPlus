#include "../config.h"
#include "simpleDll.h"

using namespace std;

string  SimpleDll::info()            {return "Test Dll";}
string  SimpleDll::wsdl()
{
	return getWsdl();
}
void    SimpleDll::initialize()
{	
	updateWSDL();	
	std::cout << name_.c_str() << " is loaded" << std::endl;
}
string  SimpleDll::run(string cmd)
{
	//setlocale(LC_ALL, ".936");
	std::cout << cmd << std::endl;
	return cmd;
}
