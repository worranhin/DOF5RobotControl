#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include "lib/NatorControl/NatorControl.h"
// #include "lib/RMDControl/RMDControl.h"
#include "RMDControl.h"

enum state_enum {
  Idle,
  Stop,
  Quit,
  ChangeJaw1,
  GoToPosition0,
  GoToPosition11,
  GoToPosition12,
  GoToPosition2,
  GoToPosition3,
  WaitForPosition11
} RobotState;

struct Joints {
  int r1;
  int p2;
  int p3;
  int p4;
  int r5;
};

const Joints BeforeChangeJawPos1 = {0, 5000000, -5000000, -15184980, 0};
const Joints ChangeJawPos1 = {0, -72195, 5174842, -6912012, 0};
const Joints AfterChangeJawPos1 = {0, -72195, -15000000, -6912012, 0};
const Joints BeforeFetchRingPos1 = {0, 8673000, -15000000, -10000000, 0};
const Joints FetchRingPos1 = {0, 8673000, 4000000, -10000000, 0};
const Joints AfterFetchRingPos1 = {0, 8673000, -15000000, -10000000, 0};
const Joints AssemblePos1 = {9000, 15686500, -16819200, -5759600, -10};
const Joints AssemblePos2 = {6000, -8027000, -15911400, 1783100, 0};
const Joints AssemblePos3 = {0, 0, 7004200, 15275000, 0};
const Joints IdlePos = {0, 0, -15000000, -10000000, 0};

const NTU_Point P0 = {0, 0, 0, 0};
// Point p1 = {-16e6, -13e6, -16e6};  // 点位1
const NTU_Point P11 = {-2000000, -5000000, 4000000, 0};  // 点位1
const NTU_Point P12 = {-3760000, -2370000, 8980000, 0};  // 点位1
// Point p2 = {0e6, -10e6, -14e6};  // 点位2
const NTU_Point P2 = {-14000000, 0, 0, 0};  // 点位2
// Point p3 = {-16e6, -3e6, -14e6};
const NTU_Point P3 = {5840000, 10100000, 8170000, 0};
const NTU_Point P4 = {2000000, 5000000, -2000000, -90};

std::atomic<bool> exitFlag(false);

// 线程函数，用于读取终端输入
// void InputThread() {
//   char inputChar;
//   while (!exitFlag) {
//     inputChar = getchar();   // 读取单个字符，不包括换行符
//     if (inputChar == 'e') {  // 检测是否按下 'e' 键
//       std::cout << "The 'e' key was pressed. Executing priority code."
//                 << std::endl;
//       // 执行需要优先执行的代码
//       // ...
//     }
//   }
// }

int D5R_JointsControl(const Joints j) {
  RMD_GoToAngle(j.r1);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_X, j.p2, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Y, j.p3, 0);
  NT_GotoPositionAbsolute_S(ntHandle1, NTU_AXIS_Z, j.p4, 0);
  NT_GotoPositionAbsolute_S(ntHandle2, NTU_ROTATION_X, j.r5, 0);
  
  return 0;
}

void InputThread() {
  static char key;
  static char input[100];

  while (!exitFlag) {
    key = getchar();
    // scanf("%s", &input);
    // printf("%s\n", input);
    // if (input == "s") {
    //   RobotState = Stop;
    // } else if (input == "c1") {
    //   RobotState = ChangeJaw1;
    // } else if (input == "0") {
    //   RobotState = GoToPosition0;
    // } else if (input == "q") {
    //   RobotState = Quit;
    // }
    switch (key) {
      case 's':
        RobotState = Stop;
        break;

        // case "c1":
        //   RobotState = ChangeJaw1;
        //   break;

      case '0':
        // RobotState = GoToPosition0;
        D5R_JointsControl(IdlePos);
        break;

        // case '1':
        //   RobotState = ChangeJaw1;
        //   break;

        // case '2':
        //   RobotState = GoToPosition2;
        //   break;

        // case '3':
        //   RobotState = GoToPosition3;
        //   break;

      case 'j':
        // RobotState = ChangeJaw1;
        D5R_JointsControl(ChangeJawPos1);
        break;

      case 'k':
        D5R_JointsControl(AfterChangeJawPos1);
        break;

      case 'f':
        // D5R_JointsControl(BeforeFetchRingPos1);
        // NTU_WaitUtilPositioned(ntHandle1);
        D5R_JointsControl(FetchRingPos1);
        // NTU_WaitUtilPositioned(ntHandle1);
        // D5R_JointsControl(AfterFetchRingPos1);
        break;

      case 'g':
        D5R_JointsControl(BeforeFetchRingPos1);
        // NTU_WaitUtilPositioned(ntHandle1);
        // D5R_JointsControl(FetchRingPos1);
        // NTU_WaitUtilPositioned(ntHandle1);
        // D5R_JointsControl(AfterFetchRingPos1);
        break;

      case '1':
        // D5R_JointsControl(IdlePos);
        // NTU_WaitUtilPositioned(ntHandle1);
        D5R_JointsControl(AssemblePos1);
        // NTU_WaitUtilPositioned(ntHandle1);
        // D5R_JointsControl(IdlePos);
        break;

      case '2':
        D5R_JointsControl(AssemblePos2);
        break;

      case '3':
        D5R_JointsControl(AssemblePos3);
        break;

      case '\n':
        break;

      case 'q':
        RobotState = Quit;
        break;

      default:
        printf("The input is wrong, please enter again!\n");
        break;
    }
  }
}

int main(int argc, char* argv[]) {
  // char key = 0;
  RobotState = Idle;

  NTU_Init();
  RMD_Init();

  printf("-----------------------------------------------\n");
  printf("Enter numbers to go to the corresponding point.\n");
  printf("Enter \'q\' to quit. Enter \'s\' to stop.\n");
  printf("Enter \'0\' to go to zero position.\n");
  printf("Enter \'j\' for change jaw, \'k\' is the position before change jaw\n");
  printf("Enter \'f\' for fetch ring, \'g\' is the position before fetch ring\n");
  printf("Enter \'1\', \'2\', \'3\' to go to the assemble position.\n");

  std::thread inputThread(InputThread);

  // 主线程执行其他任务
  while (!exitFlag) {
    switch (RobotState) {
      case Idle:
        break;

      case Stop:
        NTU_Stop();
        RMD_Stop();
        break;

      case ChangeJaw1:

      case GoToPosition0:
        NTU_GoToPoint(P0);
        RMD_GoToAngle(0);
        RobotState = Idle;
        break;

      case GoToPosition11:
        NTU_GoToPoint(P11);
        RMD_GoToAngle(0);
        RobotState = WaitForPosition11;
        // NTU_WaitUtilPositioned(ntHandle1);
        // NTU_GoToPoint(P12);
        // RobotState = Idle;
        break;

      case GoToPosition2:
        NTU_GoToPoint(P2);
        RMD_GoToAngle(4000);
        RobotState = Idle;
        break;

      case GoToPosition3:
        NTU_GoToPoint(P3);
        RMD_GoToAngle(0);
        RobotState = Idle;
        break;

      case WaitForPosition11:

        break;

      case Quit:
        exitFlag = true;
        break;

      default:
        break;
    }
  }

  // 等待输入线程结束
  inputThread.join();

  // do {
  //   key = getchar();
  //   switch (key) {
  //     case '0':
  //       NTU_GoToPoint(P0);
  //       RMD_GoToAngle(0);
  //       break;

  //     case '1':
  //       NTU_GoToPoint(P11);
  //       RMD_GoToAngle(0);
  //       NTU_WaitUtilPositioned(ntHandle1);
  //       NTU_GoToPoint(P12);
  //       break;

  //     case '2':
  //       NTU_GoToPoint(P2);
  //       RMD_GoToAngle(4000);
  //       break;

  //     case '3':
  //       NTU_GoToPoint(P3);
  //       RMD_GoToAngle(0);
  //       break;

  //     case '\n':
  //       continue;

  //     case 'q':
  //       break;

  //     default:
  //       printf("The input is wrong, please enter again!\n");
  //       continue;
  //   }
  //   NTU_WaitUtilPositioned(ntHandle1);
  // } while (key != 'q');

  // Close System
  NTU_DeInit();
  RMD_DeInit();

  return 0;
}