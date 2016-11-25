/*!\file ServerMain.cpp Assistant+节点主程序
*  \brief 通过配置文件设定服务器参数、启动Socket监听服务，关闭和资源释放。
*         suAgentService中通过suAgentHelper类直接调实现ServerMain中的初始化和监听服务启动，
*         使suAgent节点可以其通过windows服务方式运行
*
*  \date  2016-11-25
*  \author Yao Yuan
*
*/
/*
 Main Documentation
! \mainpage 代码文档

<center><img src = "../IDCenter.jpg"></center>

这个文档给出了“Assistant+”的代码说明。本文档主要由以下几个部分组成：


- \subpage subpage1 模块介绍
- \subpage subpage2 代码说明
- \subpage subpage3 使用手册
*/

//-----------------------------------------------------------

/*! \page subpage1 Assistant+模块介绍
Assistant+是一个可配置、独立运行的智能Agent程序，其功能包括：
\li 可通过规则和状态感知数据分析
\li 能够与其它Agent协同
\li 可通过外部插件进行扩展

\section sec_module   模块组成

Assitant+分为以下几个部分:
\li \ref 一个web server，建立在boost::asio和现有代码<a href = "https://github.com/eidheim/Simple-Web-Server">
Socket Server </a>：介绍了Socket服务器的详细实现。
\li <b>插件</b>部分：针对不同应用，开发应用服务插件，通过suServiceInterface.h中定义的suService接口接入到系统，并转为内部/外部可访问的Web服务。
\li 知识库：使用Redis数据库存储。
\li <b>主控模块</b>：实现在CServerSet中，嵌入到Socket监听循环中，通过监听当前知识库中的状态处理。主控模块中包括监听、规则引擎和动作执行，同样也通过外部插件实现
*/

//-----------------------------------------------------------

/*! \page subpage2 代码说明

*/

/*! \page subpage3 使用手册

*/
#include "config.h"
#include "suSocketServer.hpp"
#include "ServerSet.h "
#include "suAgentController.h"

int main(int /*argc*/, char * /*argv[ ]*/)
{
	//suAgentHelper agent;

	CServerSet::gOnly().LoadSetting();
	CServerSet::gOnly().ReadModules("");

	try
	{
		int nThread = CServerSet::gOnly().GetOption().GetOptionVal(L"Number of Threads");
		int nPOrt = CServerSet::gOnly().GetOption().GetOptionVal(L"Serverport");

		HttpServer server(nPOrt, nThread);

		std::thread server_thread([&server]() {
			//Start server
			server.start();
		});

		//init agent memory
		/*
		if (!CServerSet::gOnly().init())
		{
			Utility::MessageBoxA("请先启动Redis！");
			server.stop();
			CServerSet::gOnly().gClear();
			server_thread.join(); //wait until server is done

			return 0;
		}
		*/
		//start agent AI Controller
		//suController::gOnly().start();

		server_thread.join(); //wait until server is done

		CServerSet::gOnly().gClear();
		return 0;


	}
	catch (const suException &e)
	{	    
		Console::WriteLine(e.what());
	}
	catch (...)
	{
		Console::WriteLine("Unexpected exception");
	}

}