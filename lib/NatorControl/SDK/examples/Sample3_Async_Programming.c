/**********************************************************************
 * Copyright (c) 2020 NATORS , All Rights Reserved
 *
 * This is a programming example for the Nano Positioning System API.
 * It demonstrates programming with asynchronous functions.
 *
 * THIS  SOFTWARE, DOCUMENTS, FILES AND INFORMATION ARE PROVIDED 'AS IS'
 * WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 * BUT  NOT  LIMITED  TO,  THE  IMPLIED  WARRANTIES  OF MERCHANTABILITY,
 * FITNESS FOR A PURPOSE, OR THE WARRANTY OF NON-INFRINGEMENT.
 * THE  ENTIRE  RISK  ARISING OUT OF USE OR PERFORMANCE OF THIS SOFTWARE
 * REMAINS WITH YOU.
 * IN  NO  EVENT  SHALL  THE  NATORS  BE  LIABLE  FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, CONSEQUENTIAL OR OTHER DAMAGES ARISING
 * OUT OF THE USE OR INABILITY TO USE THIS SOFTWARE.
 **********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "NTControl.h"

void PrintMcsError(NT_STATUS st) {
  printf("MCS error %u\n", st);
}

/* All NPS commands return a status/error code which helps analyzing
   problems */
void NTU_ExitIfError(NT_STATUS st) {
  if (st != NT_OK) {
    PrintMcsError(st);
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  NT_INDEX ntHandle;
  unsigned int numOfChannels = 0;
  NT_INDEX channelA = 0, channelB = 1;
  int stop = 0;
  int positionA = 0, positionB = 0;
  int positionAGotten = 0, positionBGotten = 0;
  NT_PACKET packet;

  /* Open the NPS in synchronous communication mode */
  NTU_ExitIfError(NT_OpenSystem(&ntHandle, "usb:ix:0", "async"));

  /*Get the NPS system's number of available channels */
  NTU_ExitIfError(NT_GetNumberOfChannels(ntHandle, &numOfChannels));
  printf("Number of Channels: %u\n", numOfChannels);

  NTU_ExitIfError(NT_GotoPositionRelative_A(ntHandle, channelA, -1000, 1000));
  NTU_ExitIfError(NT_GotoPositionRelative_A(ntHandle, channelB, -100000, 1000));

  while (!stop) {
    /*Waiting for the completion of the closed-loop instruction*/
    Sleep(500);
    NTU_ExitIfError(NT_GetPosition_A(ntHandle, channelA));
    NTU_ExitIfError(NT_GetPosition_A(ntHandle, channelB));
    for (int i = 0; i < 2; ++i) {
      NTU_ExitIfError(NT_ReceiveNextPacket_A(ntHandle, 1000, &packet));
      switch (packet.packetType) {
        case NT_NO_PACKET_TYPE: /* NT_ReceiveNextPacket_A timed out */
          break;
        case NT_ERROR_PACKET_TYPE: /* the NPS has sent an error message */
          PrintMcsError(packet.data1);
          stop = 1;
          break;
        case NT_POSITION_PACKET_TYPE: /* received a channel position packet */
          if (packet.channelIndex == channelA) {
            if (!positionAGotten) {
              positionAGotten = 1;
              positionA = packet.data2;
              printf("position1A : %d \n", positionA);
            }
          } else if (packet.channelIndex == channelB) {
            if (!positionBGotten) {
              positionBGotten = 1;
              positionA = packet.data2;
              printf("position1A : %d \n", positionA);
            }
          }
          stop = (positionAGotten && positionBGotten);
          break;
      }
    }
  }
  return 0;
}