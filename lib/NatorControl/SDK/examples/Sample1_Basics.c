/**********************************************************************
 * Copyright (c) 2020 NATORS , All Rights Reserved
 *
 * This is a programming example for the Nano Positioning System API.
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
#include "../include/NTControl.h"

/* All NPS commands return a status/error code which helps analyzing
   problems */
void NTU_ExitIfError(NT_STATUS st) {
  if (st != NT_OK) {
    printf("NPS error %u\n", st);
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  unsigned int version;
  unsigned int sensorEnabled = 0;
  NT_INDEX ntHandle;

  /*Get the version of the DLL*/
  NTU_ExitIfError(NT_GetDLLVersion(&version));
  unsigned int Major_version = (version >> 24) & 0xFF;
  unsigned int Minor_version = (version >> 16) & 0xFF;
  unsigned int Patch_version = version & 0xFFFF;
  printf("The version of dll is:%d.%d.%d\n", Major_version, Minor_version,
         Patch_version);

  /* When opening a controller you must select one of the two communication
    modes:
 sync: only commands from the set of synchronous commands can
     be used in the program. In sync. communication mode commands like
     GetPosition, GetStatus etc. return the requested value directly.
     this is easier to program, especially for beginners.
 async: only asynchronous commands can be used. In this mode Get...
     commands send a request message to the NPS controller but do not
     wait for the reply. The replied message must be caught with special
     commands ReceiveNextPacket, which are only available in async.
 communication mode. Please read the NPS Programmer's Guide for more
 information. */

  /* Open the NPS in synchronous communication mode */
  NTU_ExitIfError(NT_OpenSystem(&ntHandle, "usb:id:7547982319", "sync"));

  /* Now the NPS is initialized and can be used.
     In this demo program all we do is reading the sensor power-mode. */

  NTU_ExitIfError(NT_GetSensorEnabled_S(ntHandle, &sensorEnabled));
  switch (sensorEnabled) {
    case NT_SENSOR_DISABLED:
      printf("Sensors are disabled\n");
      break;
    case NT_SENSOR_ENABLED:
      printf("Sensors are enabled\n");
      break;
    default:
      printf("Error: unknown sensor power status\n");
      break;
  }

  /* At the end of the program you should release all opened systems. */

  NTU_ExitIfError(NT_CloseSystem(ntHandle));

  return 0;
}
