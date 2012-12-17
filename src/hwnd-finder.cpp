#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional> 
#include <cctype>
#include <locale>

#include "hwnd-finder.h"

#define DEBUG 0

static inline
void
split(
    const std::string &str,
    const char delim,
    std::vector<std::string> &container
)
{
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delim)) {
        container.push_back(item);
    }
}

static inline
void
ltrim(std::string &str)
{
    str.erase(
        str.begin(),
        std::find_if(
            str.begin(),
            str.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))
        )
    );
}

static inline
void
rtrim(std::string &str)
{
    str.erase(
        std::find_if(
            str.rbegin(),
            str.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))
        ).base(),
        str.end()
    );
}

static inline
void
trim(std::string &str)
{
    rtrim(str);
    ltrim(str);
}

static inline
void
trim(std::vector<std::string> &container)
{
    for (std::vector<std::string>::iterator it = container.begin();
        it != container.end();
        ++it)
    {
        trim(*it);
    }
}

struct EnumData {
    std::vector<std::string> &classNames;
    HWND &hwnd;
};

static BOOL CALLBACK
EnumProc(HWND hwnd, LPARAM lParam) {
    struct EnumData *EnumData = (struct EnumData *)lParam;

    char buf[256] = {0};
    GetClassName(hwnd, buf, 256);
    if (strcmp(buf, EnumData->classNames[0].c_str()) == 0) {
        std::vector<std::string> &classNames = EnumData->classNames;
        HWND parentHwnd = hwnd;
        for (size_t i = 1; i < classNames.size(); i++) {
            parentHwnd = FindWindowEx(parentHwnd, NULL, classNames[i].c_str(), NULL);
            if (!parentHwnd) return TRUE;
            EnumData->hwnd = parentHwnd;
        }
        return FALSE;
    }
    return TRUE;
}

HWND
HwndFinder::find(const std::string &def)
{
    std::vector<std::string> classNames;
    split(def, '>', classNames);

    if (classNames.size() == 0) return NULL;

    trim(classNames);

    if (classNames.size() == 1) {
        return FindWindow(classNames[0].c_str(), NULL);
    }

    HWND hwnd = NULL;
    struct EnumData EnumData = { classNames, hwnd };
    
    EnumWindows(EnumProc, (LPARAM)&EnumData);

    return hwnd;
}

#if DEBUG
int main()
{
    HwndFinder hf;
    HWND c = hf.find("Chrome_WidgetWin_1 > Chrome_WidgetWin_0 > Chrome_RenderWidgetHostHWND");
    std::cout << c << std::endl;

    HwndFinder hff;
    HWND cc = hff.find("X");
    std::cout << cc << std::endl;
}
#endif