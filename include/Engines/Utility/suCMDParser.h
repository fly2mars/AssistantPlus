#pragma once

/**
*\class suCMDParser: Parse command line 
*\usage 
      ////parse option
      suCMDParser option(argv,argc);
      if(option.findOption("-h") || argc == 1)
      {
	      ...
	  }

      if(option.findOption("-test") )
      {
	       if(option.findOption("-i") && option.findOption("-r") )
	       {
	            sampleFile = option.findOptionValue("-i");
	            rtreesConfig = option.findOptionValue("-r");
	       }
	  }
*/
class suCMDParser{
public:
	suCMDParser() :argc_(0){};
	suCMDParser(char** s, int n){
		argc_ = n;
		for(int i=0;i<argc_;i++)
		{
			suString s(s[i]);
			s = s.MakeLowercase();
			cmd.push_back(s);
			//std::cout << s.CString() << std::endl;
		}
	}
	~suCMDParser(){};

	bool noParams(){
		if (argc_ == 1)
		{
			return true;
		}
		return false;
	}
	/**
	*find cmd option 
	*@tag   cmd option
	*@return option string
	*/
	bool findOption(const char* tag){		
		std::vector<suString>::iterator it = std::find(cmd.begin(), cmd.end(), tag);
		if(it == cmd.end()) return false;
		return true;
	}
	/**
	*find cmd option value 
	*@tag   cmd option
	*@return option value string
	*/
	suString findOptionValue(const char* tag){
		std::vector<suString>::iterator it = std::find(cmd.begin(), cmd.end(), tag);
		if((it+1) == cmd.end() || *(it+1) == suString(" "))
		{   suString msg = "No value is found for " + *it;
			throw std::exception(msg.CString());
		}
		return *(it+1);
	}
	/**
	*show help 
	*@msg   help info
	*@return option value string
	*/
	void showHelp(const char* msg){		
		std::string fn = std::string(cmd[0].CString());
		int idx = fn.find_last_of('\\');
		
		printf("use:\n     %s %s \n", fn.substr(idx+1, fn.length()-idx).c_str(), msg);
	}
private:
	std::vector<suString> cmd;
	int argc_;
};
