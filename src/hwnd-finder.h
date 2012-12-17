#ifndef HWNDFINDER_H
#define HWNDFINDER_H

#include <string>
#include <windows.h>

class HwndFinder {
public:
    HWND find(const std::string &);
};

#endif