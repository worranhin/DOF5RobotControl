#include "DOF5RobotControl.h"

/**
 * Initializes the DOF5RobotControl system by initializing the NTU and RMD modules.
 *
 * @param RMDSerialPort The serial port for the RMD module.
 *
 * @return 0 if the initialization is successful.
 *
 * @throws None.
 */
int D5R_Init(const char* RMDSerialPort) {
  NTU_Init();
  RMD_Init(RMDSerialPort);

  return 0;
}

D5R_API int D5R_DeInit() {
  NTU_DeInit();
  RMD_DeInit();

  return 0;
}

D5R_API int D5R_Stop() {
  RMD_Stop();
  NTU_Stop();

  return 0;
}

D5R_API int D5R_JointsControl(const Joints j) {
  NTU_Point p = {j.p2, j.p3, j.p4, j.r5};

  RMD_GoToAngle(j.r1);
  NTU_GoToPoint(p);

  return 0;
}

int D5R_Test(int x) {
  return x * 2;
}