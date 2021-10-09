
#include <fstream>

class cLogHandle
{
private:
	static std::ofstream f;
	static std::string g_date;
	static std::string g_path;
public:
	static int InitLog(std::string path);
	static int LogcMsg(const char * str, bool timeStamp = false);
	static int LogMsg(std::string str, bool timeStamp = false);
	static int CloseLog();
	static int printer(bool timestamp, const char * fmt, ...);
};