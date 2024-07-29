/**
 * @file NatorControl.h
 * @author worranhin (worranhin@foxmail.com)
 * @brief head file of the Nators motor control module.
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef NATOR_CONTROL_H
#define NATOR_CONTROL_H

#include "SDK/include/NTControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define NTU_AXIS_X 3 - 1
#define NTU_AXIS_Y 2 - 1
#define NTU_AXIS_Z 1 - 1
#define NTU_ROTATION_X 3 - 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NTU_point_s {
  int x;  // 单位: nm
  int y;
  int z;
  int rx;  // 旋转轴 单位: °
} NTU_Point;

extern NT_INDEX ntHandle1;
extern NT_INDEX ntHandle2;

extern const NTU_Point P0;
// Point p1 = {-16e6, -13e6, -16e6};  // 点位1
extern const NTU_Point P11;  // 点位1
extern const NTU_Point P12;  // 点位1
// Point p2 = {0e6, -10e6, -14e6};  // 点位2
extern const NTU_Point P2;  // 点位2
// Point p3 = {-16e6, -3e6, -14e6};
extern const NTU_Point P3;
extern const NTU_Point P4;

int NTU_Init();
int NTU_DeInit();
void NTU_Stop();
void NTU_ExitIfError(NT_STATUS st);
void NTU_GoToPoint(NTU_Point p);
void NTU_WaitUtilPositioned(NT_INDEX ntHandle);
void NTU_GetSensorStatus(NT_INDEX ntHandle);

#ifdef __cplusplus
}
#endif

#endif  // NATOR_CONTROL_H