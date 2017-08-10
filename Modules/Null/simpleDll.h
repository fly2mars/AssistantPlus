/***************************************************************************
 *                                                                         *
 *   Assistant+ - A framework for building personal AI tools and services  *
 *                                                                         *
 *   Copyright (C) 2017 by Yao, Yuan                                       *
 *                                                                         *
 *   This file may not be copied or shared without permission              *
 *   Contact yaoyuan@shu.edu.cn  or visit www.cloudsfab.com                *
 *                                                                         *
 ***************************************************************************/
#pragma once
#include "../suVirtualDevice.h"

/*\class  simple dll class derived from suServiceInterface
* \brief
* \date  2010-08-17
*/
class SimpleDll : public suVirtualDevice
{
public:
	SimpleDll() :suVirtualDevice(){};
	SimpleDll(suStatus *pStatus) :suVirtualDevice(pStatus){};
	//overload methods
	virtual void initialize();                    //initialize method
	virtual string info();                         //return module info
	virtual string wsdl();                         //return wsdl
	virtual string run(string cmd="command");
};
