#pragma once
#include <suServiceInterface.h>

/**\class  DLL class derived from suServiceInterface
 * \brief  For build simple web service with a similary WSDL
 * \date  2014-07-23
 */
class suVirtualDevice : public suService
{
public:
	suVirtualDevice();
	suVirtualDevice(suStatus *pStatus) :suService(pStatus){ suVirtualDevice(); }
	//overload methods
	virtual void initialize(){};                   //initialize method
	virtual string info(){return ""; }             //return module info
	virtual string wsdl(){ return ""; }            //return wsdl
	virtual string run(string cmd = "command"){ return ""; }
	void updateWSDL();
public:
	string name_;
};
