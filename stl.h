#include "common.h"
#ifndef __STL_H__
#define __STL_H__



struct STL_Triangle {
    float Normal[3];
    float a[3];
    float b[3];
    float c[3];
    uint16_t attr;
}__attribute__((packed)); // pack to make exactly 50 bytes (bruh)
std::vector<STL_Triangle> parsestl(std::string path);
#endif