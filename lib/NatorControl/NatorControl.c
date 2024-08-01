/**
 * @file NatorControl.c
 * @author worranhin (worranhin@foxmail.com)
 * @brief source file of the Nators control module
 * @version 0.2
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "NatorControl.h"

const char Loc1[] = "usb:id:7547982319";
const char Loc2[] = "usb:id:2250716012";

const int StepsPerAngle_Positive = 197;
const int StepsPerAngle_Negative = 285;
const int StepModeAmplitude = 4095;
const int StepModeFrequency = 7500;

static int AbsoluteAngleX = 0;

unsigned int version;
unsigned int sensorEnabled = 0;
unsigned int numOfChannels = 0;
unsigned int status = 0;
int xPosition = 0;
int yPosition = 0;
int zPosition = 0;
NT_INDEX ntHandle1;
NT_INDEX ntHandle2;
NT_STATUS result;

/**
 * @brief All NPS commands return a status/error code which helps analyzing
 * problems
 *
 * @param st
 */
void NTU_ExitIfError(NT_STATUS st) {
  if (st != NT_OK) {
    printf("NPS error %u\n", st);
    exit(1);
  }
}

int NTU_Init() {
  /*Get the version of the DLL*/
  // result = NT_GetDLLVersion(&version);
  // if(result != NT_OK)
  //   return result;
  // unsigned int Major_version = (version >> 24) & 0xFF;
  // unsigned int Minor_version = (version >> 16) & 0xFF;
  // unsigned int Patch_version = version & 0xFFFF;
  // printf("The version of dll is:%d.%d.%d\n", Major_version, Minor_version,
  //        Patch_version);

  /* Open the NPS in synchronous communication mode */

  result = NT_OpenSystem(&ntHandle1, Loc1, "sync");
  if (result != NT_OK) {
    return result;
  }
  result = NT_OpenSystem(&ntHandle2, Loc2, "sync");
  if (result != NT_OK) {
    return result;
  }
  // printf("System opened\n");

  // 获取可用通道数
  // result = NT_GetNumberOfChannels(ntHandle1, &numOfChannels);
  // NTU_ExitIfError(result);
  // printf("Number of Channels: %u\n", numOfChannels);
  // result = NT_GetNumberOfChannels(ntHandle2, &numOfChannels);
  // NTU_ExitIfError(result);
  // printf("Number of Channels: %u\n", numOfChannels);

  // 设置使能操作杆
  result = NT_SetHCMEnabled(ntHandle1, NT_HCM_ENABLED);
  if (result != NT_OK)
    return result;

  result = NT_SetHCMEnabled(ntHandle2, NT_HCM_ENABLED);
  if (result != NT_OK)
    return result;
  // printf("HCM enabled\n");

  // get current position
  // NTU_ExitIfError(result = NT_GetPosition_S(ntHandle1, NTU_AXIS_X,
  // &xPosition)); NTU_ExitIfError(result = NT_GetPosition_S(ntHandle1,
  // NTU_AXIS_Y, &yPosition)); NTU_ExitIfError(result =
  // NT_GetPosition_S(ntHandle1, NTU_AXIS_Z, &zPosition)); if (result == NT_OK)
  // {
  //   // current position is in position variable
  //   printf("Current position: (%d, %d, %d)\n", xPosition, yPosition,
  //   zPosition);
  // }
  // printf("Please manually set zero before using.\n");

  return 0;
}

int NTU_DeInit() {
  result = NT_CloseSystem(ntHandle1);
  // NTU_ExitIfError(result);
  // printf("System closed\n");

  return result;
}

/**
 * @brief Stop motors
 * 
 */
void NTU_Stop() {
  NT_Stop_S(ntHandle1, NTU_AXIS_X);
  NT_Stop_S(ntHandle1, NTU_AXIS_Y);
  NT_Stop_S(ntHandle1, NTU_AXIS_Z);
  NT_Stop_S(ntHandle2, NTU_ROTATION_X);
}

/**
 * @brief
 *
 * @param ntHandle
 * @param p 4d point, includes x, y, z, rx information
 */
void NTU_GoToPoint(NTU_Point p) {
  // NT_STATUS resultX, resultY, resultZ = 0;
  // NTU_ExitIfError(resultX =
  //                     NT_GotoPositionAbsolute_S(ntHandle, NTU_AXIS_X, p.x,
  //                     0));
  // NTU_ExitIfError(resultY =
  //                     NT_GotoPositionAbsolute_S(ntHandle, AXIS_Y, p.y, 0));
  // NTU_ExitIfError(resultZ =
  //                     NT_GotoPositionAbsolute_S(ntHandle, AXIS_Z, p.z, 0));
  int angleX = p.rx;
  int deltaAngleX = angleX - AbsoluteAngleX;
  int steps = 0;
  if (deltaAngleX > 0) {
    steps = deltaAngleX * StepsPerAngle_Positive;
  } else if (deltaAngleX < 0) {
    steps = deltaAngleX * StepsPerAngle_Negative;
  }

  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_X, p.x, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Y, p.y, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Z, p.z, 0);
  NT_StepMove_S(ntHandle2, NTU_ROTATION_X, steps, StepModeAmplitude,
                StepModeFrequency);

  AbsoluteAngleX = angleX;
}

/**
 * @brief wait util the motor reach the point after calling `NTU_GoToPoint`
 *
 * @param ntHandle
 */
void NTU_WaitUtilPositioned(NT_INDEX ntHandle) {
  unsigned int status = 0;
  do {
    Sleep(500);
    NT_GetStatus_S(ntHandle, NTU_AXIS_X, &status);
  } while (status == NT_TARGET_STATUS);
  do {
    Sleep(500);
    NT_GetStatus_S(ntHandle, NTU_AXIS_Y, &status);
  } while (status == NT_TARGET_STATUS);
  do {
    Sleep(500);
    NT_GetStatus_S(ntHandle, NTU_AXIS_Z, &status);
  } while (status == NT_TARGET_STATUS);
}

/**
 * @brief Get sensor status and print the message.
 *
 * @param ntHandle
 */
void NTU_GetSensorStatus(NT_INDEX ntHandle) {
  NT_STATUS result = 0;
  unsigned int sensorEnabled = 0;
  NTU_ExitIfError(result = NT_GetSensorEnabled_S(ntHandle, &sensorEnabled));
  if (result == NT_OK) {
    printf("Sensor enabled: %u\n", sensorEnabled);
    switch (sensorEnabled) {
      case NT_SENSOR_ENABLED:
        printf("Sensor enabled\n");
        break;

      case NT_SENSOR_DISABLED:
        printf("Sensor disabled\n");
        break;

      default:
        printf("Sensor unknown\n");
        break;
    }
  }
}