#pragma once

#include <iostream>
#include "NatorControl.h"
#include "RMDControl.h"

#ifdef D5R_EXPORTS
#define D5R_API __declspec(dllexport)
#else
#define D5R_API __declspec(dllimport)
#endif

struct Joints {
  int r1;
  int p2;
  int p3;
  int p4;
  int r5;
};

#ifdef __cplusplus
extern "C" {
#endif

D5R_API int D5R_Init();
D5R_API int D5R_DeInit();
D5R_API int D5R_Stop();
D5R_API int D5R_JointsControl(const Joints j);
D5R_API int D5R_Test(int x);

#ifdef __cplusplus
}
#endif