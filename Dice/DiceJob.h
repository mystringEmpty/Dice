#pragma once
#include "DiceConsole.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "DiceSchedule.h"
#include "StrExtern.hpp"
#include "ManagerSystem.h"
#include "DiceCloud.h"
#pragma warning(disable:28159)

inline time_t tNow = time(NULL);

int sendSelf(const string msg) {
	static long long selfQQ = CQ::getLoginQQ();
	return CQ::sendPrivateMsg(selfQQ, msg);
}

void cq_exit(DiceJob& job) {
	int pid = getpid();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		job.note("重启失败：进程快照创建失败！", 1);
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	int ppid(0);
	while (bResult) {
		if (pe32.th32ProcessID == pid) {
			ppid = pe32.th32ParentProcessID;
			break;
		}
		bResult = Process32Next(hProcessSnap, &pe32);
	}
	if (!ppid) {
		job.note("重启失败：未找到进程！", 1);
	}
	string strCMD("taskkill /f /pid " + to_string(ppid));
	job.note("已命令" + getMsg("self") + "在5秒后自杀", 1);
	std::this_thread::sleep_for(5s);
	job.echo(strCMD);
	Enabled = false;
	dataBackUp();
	system(strCMD.c_str());
}
void cq_restart(DiceJob& job) {
	char** path = new char* ();
	_get_pgmptr(path);
	string strSelfPath(*path);
	delete path;
	string strSelfName;
	int pid = getpid();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		job.note("重启失败：进程快照创建失败！", 1);
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	int ppid(0);
	while (bResult) {
		if (pe32.th32ProcessID == pid) {
			ppid = pe32.th32ParentProcessID;
			job.echo("确认进程" + strSelfPath + "\n本进程id:" + to_string(pe32.th32ProcessID) + "\n父进程id:" + to_string(pe32.th32ParentProcessID));
			strSelfName = pe32.szExeFile;
			break;
		}
		bResult = Process32Next(hProcessSnap, &pe32);
	}
	if (!ppid) {
		job.note("重启失败：未找到进程！", 1);
	}
	string command = "taskkill /f /pid " + to_string(ppid) + "\nstart .\\" + strSelfName + " /account " + to_string(console.DiceMaid);
	ofstream fout("reload.bat");
	fout << command << std::endl;
	fout.close();
	job.note(command, 0);
	std::this_thread::sleep_for(3s);
	Enabled = false;
	dataBackUp();
	switch (UINT res = -1; res = WinExec(".\\reload.bat", SW_SHOW)) {
	case 0:
		job.note("重启失败：内存或资源已耗尽！", 1);
		break;
	case ERROR_FILE_NOT_FOUND:
		job.note("重启失败：指定的文件未找到！", 1);
		break;
	case ERROR_PATH_NOT_FOUND:
		job.note("重启失败：指定的路径未找到！", 1);
		break;
	default:
		if (res > 31)job.note("重启成功√", 0);
		else job.note("重启失败：未知错误" + to_string(res), 0);
		break;
	}
}

void dice_update(DiceJob& job) {
	job.note("开始更新Dice\n版本:" + job.strVar["ver"], 1);
	char** path = new char* ();
	_get_pgmptr(path);
	string strAppPath(*path);
	strAppPath = strAppPath.substr(0, strAppPath.find_last_of("\\")) + "\\app\\com.w4123.dice.cpk";
	delete path;
	string strURL("http://shiki.stringempty.xyz/DiceVer/" + job.strVar["ver"] + "?" + to_string(job.fromTime));
	switch (Cloud::DownloadFile(strURL.c_str(), strAppPath.c_str())) {
	case -1:
		job.echo("更新失败:" + strURL);
		break;
	case -2:
		job.echo("更新失败!文件未找到:" + strAppPath);
		break;
	case 0:
		job.note("更新Dice!" + job.strVar["ver"] + "版成功√\n可用.system reload 重启应用更新", 1);
	}
}