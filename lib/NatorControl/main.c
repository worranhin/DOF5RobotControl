#include "NatorControl.h"

const NTU_Point P0 = {0, 0, 0, 0};
// Point p1 = {-16e6, -13e6, -16e6};  // 点位1
const NTU_Point P11 = {-2000000, -5000000, 4000000, 0};  // 点位1
const NTU_Point P12 = {-3760000, -2370000, 8980000, 0};  // 点位1
// Point p2 = {0e6, -10e6, -14e6};  // 点位2
const NTU_Point P2 = {-14000000, 0, 0, 0};  // 点位2
// Point p3 = {-16e6, -3e6, -14e6};
const NTU_Point P3 = {5840000, 10100000, 8170000, 0};
const NTU_Point P4 = {2000000, 5000000, -2000000, -90};

int main(int argc, char* argv[]) {
  char key = 0;

  NTU_Init();

  // test rotation x
  // int num = 0;
  // while (1) {
  //   scanf("%d", &num);
  //   if (num == 'q') {
  //     return 0;
  //   } else if (num == '+') {
  //     NTU_ExitIfError(NT_StepMove_S(ntHandle2, 2, 18000, 4095, 10000));
  //   } else if (num == '-') {
  //     NTU_ExitIfError(NT_StepMove_S(ntHandle2, 2, -18000, 4095, 10000));
  //   } else if (num <= 30000 && num >= -30000) {
  //     NTU_ExitIfError(NT_StepMove_S(ntHandle2, 2, num, 4095, 10000));
  //   }
  // }

  // 测试步进时的状态
  // unsigned int status;
  // NTU_ExitIfError(NT_GetStatus_S(ntHandle2, NTU_ROTATION_X, &status));
  // printf("Before moving: status of rx: %d\n", status);
  // NTU_ExitIfError(NT_StepMove_S(ntHandle2, NTU_ROTATION_X, 20000, 4000, 10000));
  // while (1) {
  //   NTU_ExitIfError(NT_GetStatus_S(ntHandle2, NTU_ROTATION_X, &status));  
  //   printf("Step moving: status of rx: %d\n", status);  // TODO: 问 SDK 开发者，为什么 StepMove 的状态是 LIMIT_POSITION
  //   Sleep(500);
  // }

  printf("-----------------------------------------------\n");
  printf("Enter numbers to go to the corresponding point.\n");
  printf("Enter \'q\' to quit.\n");
  printf("Available points: 0, 1, 2, 3, 4\n");
  do {
    key = getchar();
    switch (key) {
      case '0':
        NTU_GoToPoint(P0);
        break;

      case '1':
        NTU_GoToPoint(P11);
        NTU_WaitUtilPositioned(ntHandle1);
        NTU_GoToPoint(P12);
        break;

      case '2':
        NTU_GoToPoint(P2);
        break;

      case '3':
        NTU_GoToPoint(P3);
        break;

      case '4':
        NTU_GoToPoint(P4);

      case '\n':
        continue;

      case 'q':
        break;

      case 's':
        NT_Stop_S(ntHandle1, NTU_AXIS_X);
        NT_Stop_S(ntHandle1, NTU_AXIS_Y);
        NT_Stop_S(ntHandle1, NTU_AXIS_Z);
        NT_Stop_S(ntHandle2, NTU_ROTATION_X);
        break;

      default:
        printf("The input is wrong, please enter again!\n");
        continue;
    }
    NTU_WaitUtilPositioned(ntHandle1);
  } while (key != 'q');

  // Close System
  NTU_DeInit();
}