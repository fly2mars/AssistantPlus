#include "suServerStatus.h"
#include "ServerSet.h"
//事件列表与全局变量类都存储在CServerSet类中
//suEStatus实例指针通过调用suEStatus::gOnly()，以(suEStatus*)传递至dll中
//
//suEStatus* suEStatus::gOnly_ = NULL;
//suEStatus* suEStatus::gOnly()
//{
//	if (gOnly_ == NULL)
//	{
//		gOnly_ = new suEStatus();
//	}
//	return gOnly_;
//}
//Set or add value to the system status list

int suEStatus::addEvent(string username, string moduleName,string invokeService, string callBack, time_t startTime)
{
	//Todo: add start time event
	suEvent event;
	
	GetEventList().push_back(event);
	return event.id;
}
int suEStatus::addEvent(string username, string moduleName,string invokeService, string callBack, int interval)
{
	//Todo: add ingterval invoke event
	suEvent event;

	GetEventList().push_back(event);
	return event.id;
}
void suEStatus::deleteEvent(int id)     //delete event by id
{
	EventList::iterator it=GetEventList().begin();

	for(;it!=GetEventList().end();it++)
	{
		if ((*it).id == id)
		{
			GetEventList().erase(it);
		}
	}
	
}
void suEStatus::setGlobalVar(string key, string value)    //set global variable
{
	GetGlobalVar().setVar(key,value);
}
string suEStatus::getGlobalVar(string key)              //return global variable 
{
	return GetGlobalVar().getVar(key);
}

void suEStatus::stopServer()
{
	CServerSet::gOnly().ShutdownsServer();
}

void suEStatus::pauseServer()
{
	CServerSet::gOnly().PauseResumeServer();
}

