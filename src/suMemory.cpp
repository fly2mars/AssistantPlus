#include "suServerStatus.h"
#include "ServerSet.h"
#include "redispp.h"
#include <strstream>
#include <ctime>
static const char* RULE_INDEX_TABLE = "RuleList";
static const char* STATE_INDEX_TABLE = "StateList";
static const char* TASK_INDEX_TABLE = "TaskList";

class suAgentMemory::DB_
{
public:
	DB_() :pConn_(0){}
	~DB_(){}
	bool connect(){
		std::string HOST = CServerSet::gOnly().GetOption("Memory Address");
		std::string PORT = CServerSet::gOnly().GetOption("Memory Port");
		if (HOST.empty())
		{
			HOST = "127.0.0.1";
			PORT = "6379";
		}
		try{
			pConn_ = new redispp::Connection(HOST.c_str(), PORT.c_str(), "");
		}
		catch (const std::exception& ex){
			pConn_ = 0;
			std::cout << ex.what() << std::endl;
			return 0;
		}
		return 1;
	}
public:
	redispp::Connection *pConn_;
	suLock lock_;
};

suAgentMemory::suAgentMemory()
{
	pDB_ = new DB_();
}
suAgentMemory::~suAgentMemory()
{
	if (!pDB_) delete pDB_;
}
std::string suAgentMemory::getGlobalVar(string key)
{	
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return "";
	redispp::StringReply stringReply = pDB_->pConn_->get(key.c_str());
	if (stringReply.result().is_initialized())
	{
		std::string str = stringReply;
		return str;
	}
	
	return "";
}

void suAgentMemory::setGlobalVar(string key, string value)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return;
	pDB_->pConn_->sadd(STATE_INDEX_TABLE, key.c_str());
	pDB_->pConn_->set(key.c_str(), value.c_str());
}

int suAgentMemory::init()
{
	if (!pDB_->connect())
	{
		std::cout << "Cannot connect Redis\n";
		return 0;
	}
	return 1;
}

void suAgentMemory::setGlobalList(std::string key, std::vector<std::string> stringList)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return;  //cerr out

	if (pDB_->pConn_->exists(key.c_str()))  pDB_->pConn_->del(key);

	for (int i = 0; i < (int)stringList.size(); i++)
	{
		pDB_->pConn_->rpush(key.c_str(), stringList[i].c_str());
	}
}

std::vector<std::string> suAgentMemory::getGlobalList(std::string key)
{
	suLocker locker(pDB_->lock_);

	std::vector<std::string> _list;
	if (!pDB_->pConn_)  return _list;  //cerr out

	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->lrange(key.c_str(), 0, -1);
	std::string result;
	while (multi_result.next(&result))
		_list.push_back(result);

	return _list;
}


void suAgentMemory::remove(std::string key)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return;
	pDB_->pConn_->del(key.c_str());
}

int suAgentMemory::getRule(std::string id, suAgent::Rule& rule)
{ 
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;

	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->lrange(id, 0, -1);
	std::string item;

	rule.id = id;

	if (multi_result.next(&item))
	{
		rule.act.key = item;
	}
	if (multi_result.next(&item))
	{
		rule.act.value = item;
	}
	if (multi_result.next(&item))
	{
		if (item == "name"){
			if (multi_result.next(&item))
			{
				rule.name = item;
			}
		}
	}

	while (multi_result.next(&item))
	{
		std::string key_ = item;
		if (multi_result.next(&item))
		{
			std::string value_ = item;
			rule.AddCondition(key_, value_);
		}
	}
	return 1;	
}
int suAgentMemory::getKeys(std::vector<suAgent::Pairs> &keyList)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;

	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->smembers(STATE_INDEX_TABLE);
	std::string item;

	while (multi_result.next(&item))
	{
		suAgent::Pairs param;
		param.key = item;
		param.value = getGlobalVar(item);
		keyList.push_back(param);
	}
	return (int)keyList.size();
}
int suAgentMemory::getTask(std::string id, suAgent::Task& task)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;

	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->lrange(id, 0, -1);
	std::string item;

	task.id = id;

	if (multi_result.next(&item))
	{
		task.desc = item;
	}

	while (multi_result.next(&item))
	{
		suAgent::Pairs param;
		param.key = item;
		if (multi_result.next(&item))
		{
			param.value = item;
		}
		task.keyList.push_back(param); //deprecated
		task.keys[param.key] = param.value;
	}
	return 1;
}
int suAgentMemory::getRules(std::vector<suAgent::Rule> &ruleList)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;
	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->smembers(RULE_INDEX_TABLE);
	std::string id;		

	ruleList.clear();
	while (multi_result.next(&id))
	{
		suAgent::Rule _rule;
		getRule(id, _rule);
		ruleList.push_back(_rule);
	}
	return (int)ruleList.size();
}
int suAgentMemory::getTasks(std::vector<suAgent::Task> &taskList)
{ 
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;
	redispp::MultiBulkEnumerator multi_result = pDB_->pConn_->smembers(TASK_INDEX_TABLE);
	std::string id;

	taskList.clear();
	while (multi_result.next(&id))
	{
		suAgent::Task _task;
		getTask(id, _task);
		taskList.push_back(_task);
	}
	return (int)taskList.size();
}
std::string suAgentMemory::addRule(suAgent::Rule rule)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return "";
	
	if (rule.id.empty())
	{
		//generate a new rule id
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%Y%m%d%I%M%S-", timeinfo);
		std::string id(buffer);
		std::strstream buf;
		buf << rand()  << std::ends;
		id = id + rule.name + std::string(buf.str());

		rule.id = id;
	}
	else{
		//update, del original record.
		pDB_->pConn_->del(rule.id.c_str());
	}
	
	//add the rule id to rule list	
	pDB_->pConn_->sadd(RULE_INDEX_TABLE, rule.id.c_str());
	//add a rule 
	pDB_->pConn_->rpush(rule.id.c_str(), rule.act.key.c_str());
	pDB_->pConn_->rpush(rule.id.c_str(), rule.act.value.c_str());
	pDB_->pConn_->rpush(rule.id.c_str(), "name");
	pDB_->pConn_->rpush(rule.id.c_str(), rule.name);

	for (int i = 0; i < (int)rule.cond.size(); i++)
	{
		pDB_->pConn_->rpush(rule.id.c_str(), rule.cond[i].key.c_str());
		pDB_->pConn_->rpush(rule.id.c_str(), rule.cond[i].value.c_str());
	}
	return rule.id; 
}  
std::string suAgentMemory::addTask(suAgent::Task task)
{ 
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return "";
		
	if (task.id.empty())
	{
		//generate a new rule id
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%Y%m%d%I%M%S-", timeinfo);
		std::string id(buffer);
		std::strstream buf;
		buf << rand() << std::ends;
		id = id + "Task" + std::string(buf.str());

		task.id = id;
	}
	else{
		//if update, del original record.
		pDB_->pConn_->del(task.id.c_str());
	}

	//add the rule id to rule list	
	pDB_->pConn_->sadd(TASK_INDEX_TABLE, task.id.c_str());
	//add a rule 
	pDB_->pConn_->rpush(task.id.c_str(), task.desc.c_str());

	for (int i = 0; i < (int)task.keyList.size(); i++)
	{
		pDB_->pConn_->rpush(task.id.c_str(), task.keyList[i].key.c_str());
		pDB_->pConn_->rpush(task.id.c_str(), task.keyList[i].value.c_str());
	}
	return task.id;
}  
int suAgentMemory::remRule(std::string id)
{ 
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;
	if (pDB_->pConn_->exists(id.c_str()))
	{
		pDB_->pConn_->srem(RULE_INDEX_TABLE, id.c_str());
		remove(id.c_str());
		return 1;
	}
	return 0; 
}
int suAgentMemory::remTask(std::string id)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;
	if (pDB_->pConn_->exists(id.c_str()))
	{
		pDB_->pConn_->srem(TASK_INDEX_TABLE, id.c_str());
		remove(id.c_str());
		return 1;
	}
	return 0;
}
int  suAgentMemory::remKey(std::string key)
{
	suLocker locker(pDB_->lock_);

	if (!pDB_->pConn_)  return 0;
	if (pDB_->pConn_->exists(key.c_str()))
	{
		pDB_->pConn_->srem(STATE_INDEX_TABLE, key.c_str());
		remove(key.c_str());
		return 1;
	}
	return 0;
}

int suAgentMemory::getServices(std::vector<suAgent::Pairs> &services)
{
	suLocker locker(pDB_->lock_);

	std::vector < std::string> sList;
	CServerSet::gOnly().GetServices(sList);

	for (int i = 0; i < (int)sList.size(); i++)
	{
		suAgent::Pairs p;
		p.key = "name";
		p.value = sList[i];
		services.push_back(p);
	}
	return (int)sList.size();
}