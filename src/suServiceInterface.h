#pragma once
//suServiceInterface
//定义Agent与外部插件的交互接口
#include <string>
#include <vector>
#include <map>
using namespace std;


/*********************************************************************
定义所有基本类Pairs/Rule/Task
**********************************************************************/
namespace suAgent{
   /* Pairs 
	* \brief 保存key/value, act as FACT
	*        
	*/
	struct Pairs
	{
		std::string key;
		std::string value;
	};
   /* Rule
	* \brief 规则表达
	*/
	class Rule
	{
	public:
		void SetAction(std::string key, std::string value){ act.key = key; act.value = value; }
		void AddCondition(std::string key, std::string value){
			Pairs p; p.key = key; p.value = value;
			cond.push_back(p);
		}		
	public:
		Pairs act;
		std::string id;
		std::string name;		
		std::vector<Pairs> cond;   //head
	};

	
	/* Task
	* \brief 描述当前任务或订单（任务组合）
	*/
	class Task
	{	
	public:
		void addKey(std::string key, std::string value){
			Pairs p; p.key = key; p.value = value;
			keyList.push_back(p);
			keys[key] = value;
		}
		//如果状态未设定或为false，可直接在数据中删除此状态值，或设为""
		bool isAssign(){ if (keys["isAsssign"].empty()) return false; return true; }
		bool isInProcess(){ if (keys["isInProcess"].empty()) return false; return true; }
		bool isFinish(){ if (keys["isFinish"].empty()) return false; return true; }
	public:
		std::string id;
		std::string desc;		
		std::vector<Pairs>  keyList;     //A key - value list to descript the Task parameters.
		std::map<std::string, std::string> keys;
	};
}
/*\name suStatus
* \brief  It provids agent interface for dynamic module.
* \date  2010-08-10
*/
class suStatus
{
public:	
	suStatus(){};
	~suStatus(){};

	//static suStatus* gOnly();
	//Set add event to the system event list
	//event functions will be deprecated in future
	virtual int addEvent(std::string username, std::string moduleName, std::string invokeService, std::string callBack, time_t startTime){ return -1; }
	virtual int addEvent(std::string username, std::string moduleName, std::string invokeService, std::string callBack, int interval){ return -1; }
	virtual void deleteEvent(int id=0){}     //delete event by id


	virtual void setGlobalVar(std::string key, std::string value){}    //set global variable
	virtual std::string getGlobalVar(std::string key){ return ""; }      //return global variable 

	virtual void setGlobalList(std::string key, std::vector<string> stringList){}             //we can use database (like Redis) in the derive class
	virtual std::vector<std::string> getGlobalList(std::string key){ std::vector<std::string> s; return s; }      //return global list 

	virtual void remove(std::string key){}

	//shutdown server
	virtual void stopServer(){}
	//pause server
	virtual void pauseServer(){}

	//new interface	functions
	//return 0:false    1:success return number	
	virtual int  getRule (std::string id, suAgent::Rule& rule){ return 0; }
	virtual int  getTask (std::string id, suAgent::Task& task){ return 0; }	
	//get all rules
	virtual int  getRules(std::vector<suAgent::Rule> &ruleList){return 0;}
	//get all tasks
	virtual int  getTasks(std::vector<suAgent::Task> &taskList){ return 0; }
	//get all Pairs
	virtual int  getKeys (std::vector<suAgent::Pairs> &keyList){ return 0; }    
	virtual std::string  addRule (suAgent::Rule rule){ return ""; }  //update if rule.id <> ""  return id
	virtual std::string  addTask (suAgent::Task task){ return ""; }  //update if task.id <> ""  return id
	virtual int  remRule (std::string id){ return 0; }
	virtual int  remTask (std::string id){ return 0; }
	virtual int  remKey(std::string key){ return 0; }

	virtual int getServices(std::vector<suAgent::Pairs> &services){ return 0; }

	
};

/*\brief Dynamic module interface for agent
* 
* \date  2010-08-10
*/
class suService
{  
public:
	//Constructor
	suService():wsdl_(""){}
	suService(suStatus* pStatus):wsdl_(""){setStatus(pStatus);}
	~suService(){}

	//process function mapping with service function
	virtual void initialize(){}                                 ///<Initialize the module (must be overloaded)
	virtual string info(){return "";}                           ///<Get the information about the service module
	virtual string wsdl(){return "";}                           ///<Return wsdl
	virtual string run(string cmd="command"){return "";}        ///<Run the command string 
	//virtual string run(string name, string cmd="command"){return "";}    ///<Run the command string; name=current dll name 直接用参数将dll的名字传给自身

	//member methods
	void      setWsdl   (string wsdl)      {wsdl_=wsdl;   }
	void      setStatus (suStatus* s)      {pStatus_ = s; }  
	string    getWsdl()                    {return wsdl_; }
	suStatus* getStatus()                  {return pStatus_;}


public:
	string       wsdl_;                                ///<WSDL string
	suStatus*    pStatus_;                             ///<Global singleton status object

	
};

