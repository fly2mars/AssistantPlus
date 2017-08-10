#include "../src/config.h"
#include <boost/filesystem/operations.hpp>

#include <boost/filesystem/path.hpp>

#include <iostream>
#include "../src/ServerSet.h"
#ifdef __linux
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef WIN32

#pragma comment(lib,"ws2_32.lib")
#endif
UTFUNC(suStringTest)
{
	return;
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	suString filename;

	filename = "test.dll";
	std::cout << filename.FindExtension() << std::endl;
	suString ext = filename.FindExtension();
	suString service_name = filename.SubString(0, filename.Length() - ext.Length()).MakeUppercase();
	std::cout << service_name.CString() << std::endl;

	suVector<suString> fileList;
	Utility::GetFilesFromDir("E:/FangCloudSync/Books/AI", fileList, "*.pdf");
	
	for(int i=0; i<fileList.Length(); i++)
	  std::cout << fileList[i].CString() << std::endl;
}

char *pSelfName;
UTFUNC(getCurdir)
{
	return;
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	//namespace fs = boost::filesystem;

	//fs::path full_path(fs::initial_path<fs::path>());

	//full_path = fs::system_complete(fs::path(pSelfName));

	//std::cout << full_path << std::endl;
	//std::cout << "root path: " << full_path.root_path() << std::endl;

	////Without file name
	//std::cout << "whithout filename: " << full_path.parent_path() << std::endl;
	////std::cout << fs::basename(full_path) << std::endl;
	std::cout << SU::GetModuleDir() << std::endl;

}

UTFUNC(testTinyXML2)
{
	return;
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	
	COption m_Options;
	m_Options.LoadSetting();
	m_Options.dump();

}


#ifdef __linux

UTFUNC(readDir)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;

	suString path = "/home/share/AssistantPlus/build";
	
	/*struct dirent *dp;
	DIR *dirp = opendir(".");

	while ((dp = readdir(dirp)) != NULL) {
		suString file_name = dp->d_name;
		if (file_name.FindExtension() == ".so") {
			std::cout << "file: " << dp->d_name << std::endl;
		}
		
		
	}
	closedir(dirp);*/
	suVector<suString> fileList;
	Utility::GetFilesFromDir(path, fileList, "*.so");
	for (int i = 0; i < fileList.Length(); i++) {
		std::cout << fileList[i].CString() << std::endl;
	}
	

}

std::string GetPrimaryIp()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sock != -1);

	const char* kGoogleDnsIp = "8.8.8.8";
	uint16_t kDnsPort = 53;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons(kDnsPort);

	int err = connect(sock, (const sockaddr*)&serv, sizeof(serv));
	assert(err != -1);

	sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (sockaddr*)&name, &namelen);
	assert(err != -1);

	int buflen = 16;
	char buffer[16];
	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, buflen);
	assert(p);
	close(sock);
	return std::string(buffer);
}
UTFUNC(getIP)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	char ip[16];

	std::cout << "GetPrimaryIp: " << GetPrimaryIp() << std::endl;

	setenv("LANG", "C", 1);
	FILE * fp = popen("ifconfig", "r");
	if (fp) {
		char *p = NULL, *e; size_t n;
		while ((getline(&p, &n, fp) > 0) && p) {
			if (p = strstr(p, "inet ")) {
				p += 5;
				if (p = strchr(p, ':')) {
					++p;
					if (e = strchr(p, ' ')) {
						*e = '\0';
						printf("%s\n", p);
					}
				}
			}
		}
	}
	pclose(fp);
}
#endif
int main(int argc, char** argv)
{
	pSelfName = argv[0];
	bool state = suUnitTest::gOnly().run ();
	suUnitTest::gOnly().dumpResults (std::cout);

	return state;
}
