#pragma once

/*!\file suController.h suAgent controller
*  \brief The main agent action contoller
*         
*         提供一个简单的Agent主控进程执行线程，
*         对于不同的Agent，在线程内调用相应插件
*         完成实际功能
*         1.Scan task -> process
*         2.Scan status -> process
*         3.Scan
*
*  \date  2015-02-01
*  \author Yao Yuan
*
*/
#include <Engines.h>

class suController : public suThread
{
public:
	static suController &gOnly();
	void thread();

private:
	static suController* sOnly_;
	suController(){}
};

