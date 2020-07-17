#include <string>
#include <string_view>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrExtern.hpp"

using std::string;
using std::wstring;
using std::string_view;
using std::to_string;

#define CP_GB18030 54936

string toString(int num, unsigned short size) {
    string res{ to_string(num) };
    string sign{ "" };
    if (res[0] == '-') {
        res.erase(res.begin());
        sign = "-";
    }
    while (res.length() < size)res = "0" + res;
    return sign + res;
}

int count_char(string s, char ch) {
    int cnt = 0;
    for (auto c : s) {
        if (c == ch)cnt++;
    }
    return cnt;
}

string convert_w2a(const wchar_t* wch) {
    int len = WideCharToMultiByte(CP_GB18030, 0, wch, -1, nullptr, 0, nullptr, nullptr);
    char* m_char = new char[len];
    WideCharToMultiByte(CP_GB18030, 0, wch, -1, m_char, len, nullptr, nullptr);
    std::string str(m_char);
    delete[] m_char;
    return str;
}

wstring convert_a2w(const char* ch) {
    int len = MultiByteToWideChar(CP_GB18030, 0, ch, -1, nullptr, 0);
    wchar_t* m_char = new wchar_t[len];
    MultiByteToWideChar(CP_GB18030, 0, ch, -1, m_char, len);
    std::wstring wstr(m_char);
    delete[] m_char;
    return wstr;
}

string printDuringTime(long long seconds) {
    if (seconds < 0) {
        return "N/A";
    }
    else if (seconds < 60) {
        return std::to_string(seconds) + "秒";
    }
    int mins = seconds / 60;
    seconds = seconds % 60;
    if (mins < 60) {
        return std::to_string(mins) + "分" + std::to_string(seconds) + "秒";
    }
    int hours = mins / 60;
    mins = mins % 60;
    if (hours < 24) {
        return std::to_string(hours) + "小时" + std::to_string(mins) + "分" + std::to_string(seconds) + "秒";
    }
    int days = hours / 24;
    hours = hours % 24;
    return std::to_string(days) + "天" + std::to_string(hours) + "小时" + std::to_string(mins) + "分" + std::to_string(seconds) + "秒";
}