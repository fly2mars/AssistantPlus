#include "suAgentController.h"
#include "ServerSet.h"
suController* suController::sOnly_ = 0;
suController& suController::gOnly()
{
	if (!sOnly_)
	{
		sOnly_ = new suController();
	}
	return *sOnly_;
}

void suController::thread()
{
	CServerSet &pServerSet = CServerSet::gOnly();
	std::string strCAgentIp = pServerSet.GetOption("CAgent Address");
	std::string strCAgentPort = pServerSet.GetOption("CAgent Port");
	while (true)
	{
		if (!strCAgentPort.empty())
		{
			std::cout << "I 'm checking....\n" << std::endl;
			//pServerSet.pStatus->setGlobalVar("ACTION_CONTROLLER", "Checking...");

			//TODO: running with task pool.
			//Now is a single task model.
			string Config = strCAgentIp + "&" + strCAgentPort;
			std::string task = CServerSet::gOnly().GetModule("TaskQuery")->run(Config); // 向CAgent取任务，返回任务XML; 

			if (task != "tasknull" && !task.empty())
			{
				std::string taskID_ = CServerSet::gOnly().GetModule("Translator")->run(task); //本地解析任务XML，并存储，返回任务ID.

				std::string selectedService = CServerSet::gOnly().GetModule("RuleEngine")->run(taskID_); //判定是能够执行该任务,并返回执行的服务名称
						
								
				if (!selectedService.empty())
				{					
					string assign = CServerSet::gOnly().GetModule("Executor")->run(taskID_ + "," + Config); //是否分配，并执行
					if (assign == "OK")
					{
						string RunRes = CServerSet::gOnly().GetModule(selectedService)->run(taskID_);

						//如果任务执行完毕，提交任务
						if (RunRes == "OK")
						{
							string strRes_ = CServerSet::gOnly().GetModule("TaskSubmit")->run(taskID_ + "," + Config);  //提交任务信息
							//TODO: If fail(!OK), push into a un-submited array
						}
					}

				}

			}

		}
		sleep(2000);
	}
}