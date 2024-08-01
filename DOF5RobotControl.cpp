#include "DOF5RobotControl.h"

int D5R_Init() {
  NTU_Init();
  RMD_Init();
  std::cout << "System Initialized." << std::endl;

  return 0;
}

D5R_API int D5R_DeInit() {
  NTU_DeInit();
  RMD_DeInit();
  std::cout << "System Closed." << std::endl;

  return 0;
}

D5R_API int D5R_Stop() {
  RMD_Stop();
  NTU_Stop();

  return 0;
}

D5R_API int D5R_JointsControl(const Joints j) {
  RMD_GoToAngle(j.r1);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_X, j.p2, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Y, j.p3, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Z, j.p4, 0);
  NT_GotoPositionAbsolute_S(ntHandle2, NTU_ROTATION_X, j.r5, 0);

  return 0;
}

int D5R_Test(int x) {
  return x * 2;
}