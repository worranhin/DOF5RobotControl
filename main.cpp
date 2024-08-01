#include "DOF5RobotControl.h"
#include "NatorControl.h"
#include "RMDControl.h"
#include <atomic>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>

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

std::atomic<bool> exitFlag(false);

int main(int argc, char *argv[]) {
  char key;
  RobotState = Idle;
  std::cout << "System initializing..." << std::endl;

  D5R_Init("COM7");

  printf("-----------------------------------------------\n");
  printf("Enter numbers to go to the corresponding point.\n");
  printf("Enter \'q\' to quit. Enter \'s\' to stop.\n");
  printf("Enter \'0\' to go to zero position.\n");
  printf(
      "Enter \'j\' for change jaw, \'k\' is the position before change jaw\n");
  printf(
      "Enter \'f\' for fetch ring, \'g\' is the position before fetch ring\n");
  printf("Enter \'1\', \'2\', \'3\' to go to the assemble position.\n");

  // std::thread inputThread(InputThread);

  while (!exitFlag) {
    key = getchar();
    switch (key) {
      case 's':
        D5R_Stop();
        break;

      case '0':
        D5R_JointsControl(IdlePos);
        break;

      case 'j':
        D5R_JointsControl(ChangeJawPos1);
        break;

      case 'k':
        D5R_JointsControl(AfterChangeJawPos1);
        break;

      case 'f':
        D5R_JointsControl(FetchRingPos1);
        break;

      case 'g':
        D5R_JointsControl(BeforeFetchRingPos1);
        break;

      case '1':
        D5R_JointsControl(AssemblePos1);
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
        exitFlag = true;
        break;

      default:
        printf("The input is wrong, please enter again!\n");
        break;
    }
  }

  // // 等待输入线程结束
  // inputThread.join();

  // // Close System
  D5R_DeInit();

  return 0;
}