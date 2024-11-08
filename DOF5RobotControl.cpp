#include "DOF5RobotControl.h"

/**
 * Initializes the DOF5RobotControl system by initializing the NTU and RMD
 * modules.
 *
 * @param RMDSerialPort The serial port for the RMD module.
 *
 * @return 0 if the initialization is successful.
 *
 * @throws None.
 */
int D5R_Init(const char* RMDSerialPort) {
  int result = 0;
  result |= NTU_Init();
  result |= RMD_Init(RMDSerialPort);

  result |= D5R_SetAccumulateRelative(false);

  return result == 0 ? 0 : -1;
}

/**
 * Deinitializes the DOF5RobotControl system by deinitializing the NTU and RMD
 * modules.
 *
 * @return 0 if the deinitialization is successful.
 *
 * @throws None.
 */
int D5R_DeInit() {
  int result = 0;
  result += NTU_DeInit();
  result += RMD_DeInit();

  return result == 0 ? 0 : -1;
}

/**
 * Stops all motors in the DOF5RobotControl system.
 *
 * @return 0 if stopping is successful.
 *
 * @throws None.
 */
int D5R_Stop() {
  RMD_Stop();
  NTU_Stop();

  return 0;
}

/**
 * 设置当前位置
 *
 * Sets the zero position for the DOF5RobotControl system.
 *
 * @param r1 The rotation angle for the first joint (default: 0). (not
 * implemented)
 * @param p2 The position of the second joint along the x-axis (default: 0).
 * @param p3 The position of the second joint along the y-axis (default: 0).
 * @param p4 The position of the second joint along the z-axis (default: 0).
 * @param r5 The rotation angle for the fifth joint (default: 0).
 *
 * @return 0 if the zero position is set successfully.
 *
 * @throws None.
 */
int D5R_SetZero(int r1, int p2, int p3, int p4, int r5) {
  int result = 0;
  // RMD_SetZero();
  result += NTU_SetZero(p2, p3, p4, r5);

  return result == 0 ? 0 : -1;
}

/**
 * 移动至绝对位置，零点由 `D5R_SetZero` 设置
 *
 * Moves all joints in the DOF5RobotControl system to the specified absolute
 * position.
 *
 * @param j The absolute position of all joints, with the first joint's angle in
 * 0.01 degrees, and the other joints' positions in 0.01 mm.
 *
 * @return 0 if the movement is successful.
 *
 * @throws None.
 */
int D5R_JointsMoveAbsolute(const Joints j) {
  NTU_Point p = {j.p2, j.p3, j.p4, j.r5};

  RMD_GoAngleAbsolute(j.r1);
  NTU_GoToPoint(p);

  return 0;
}

/**
 * 移动相对角度
 *
 * Moves the joints of the DOF5RobotControl system by a relative angle.
 *
 * @param j The relative displacement to move the joints.
 *
 * @return 0 if the joints are moved successfully, otherwise -1.
 *
 * @throws None.
 */
int D5R_JointsMoveRelative(const Joints j) {
  int result = 0;
  result |= RMD_GoAngleRelative(j.r1);
  result |= NT_GotoPositionRelative_S(ntHandle1, NTU_AXIS_X, j.p2, 0);
  result |= NT_GotoPositionRelative_S(ntHandle1, NTU_AXIS_Y, j.p3, 0);
  result |= NT_GotoPositionRelative_S(ntHandle1, NTU_AXIS_Z, j.p4, 0);
  NT_GotoPositionRelative_S(ntHandle2, NTU_ROTATION_X, j.r5, 0);

  return result == 0 ? 0 : -1;
}

/**
 * Sets whether the relative movement commands are accumulated or not.
 *
 * @param accumulate If true, the relative movement commands are accumulated
 *                    (i.e. the movements are added to the current position),
 *                    otherwise the movements are absolute (i.e. the movements
 *                    are relative to the zero position set by `D5R_SetZero`).
 *
 * @return 0 if the setting is successful.
 *
 * @throws None.
 */
int D5R_SetAccumulateRelative(bool accumulate) {
  int result = 0;
  result |=
      NT_SetAccumulateRelativePositions_S(ntHandle1, NTU_AXIS_X, accumulate);
  result |=
      NT_SetAccumulateRelativePositions_S(ntHandle1, NTU_AXIS_Y, accumulate);
  result |=
      NT_SetAccumulateRelativePositions_S(ntHandle1, NTU_AXIS_Z, accumulate);

  return result == 0 ? 0 : -1;
}

int D5R_Test(int x) {
  return x * 2;
}