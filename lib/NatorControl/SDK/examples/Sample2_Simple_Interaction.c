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
#include <Windows.h>
#include "NTControl.h"

int main(int argc, char* argv[])
{
    NT_STATUS error = NT_OK;
    NT_INDEX ntHandle;
    unsigned int numOfChannels = 0;
    unsigned int channel = 0;
    int linearSensorPresent = 0;
    int key;
    unsigned int status;
    int position = 0;

    // ----------------------------------------------------------------------------------
    /* Open the NPS in synchronous communication mode */
    error = NT_OpenSystem(&ntHandle, "usb:ix:0", "sync");
    if (error)
    {
        printf("Open system: Error: %u\n", error);
        return 1;
    }
    else
    {
        printf("Open system  successfully!\n");
    }
    
    /*Get the NPS system's number of available channels */
    error = NT_GetNumberOfChannels(ntHandle, &numOfChannels);
    printf("Number of Channels: %u\n", numOfChannels);


    // ----------------------------------------------------------------------------------
    /* check availability of linear sensor. If NT_SetPosition_S returns NT_OK, 
       the linear sensor is available */
    error = NT_SetPosition_S(ntHandle, channel, position);
    if (error == NT_OK)
    {
        linearSensorPresent = 1;
        printf("Linear sensor present\n");
    }
    else 
    {
        linearSensorPresent = 0;
        printf("No linear sensor present\n");
    }
    // ----------------------------------------------------------------------------------
    if (linearSensorPresent)
    {
        printf("\nENTER COMMAND AND RETURN\n"
            "+  Move positioner up by 100um\n"
            "-  Move positioner down by 100um\n"
            "q  Quit program\n");
    }
    else 
    {
        printf("\nENTER COMMAND AND RETURN\n"
            "+  Move positioner up by 200 steps\n"
            "-  Move positioner down by 200 steps\n"
            "q  Quit program\n");
    }

    // ----------------------------------------------------------------------------------
    do
    {
        key = getchar();
        switch (key)
        {
        case '-':
            if (linearSensorPresent)
                NT_GotoPositionRelative_S(ntHandle, channel, -10000, 1000);
            else
                NT_StepMove_S(ntHandle, channel, -200, 4095, 2000);
            break;

        case '+':
            if (linearSensorPresent)
                NT_GotoPositionRelative_S(ntHandle, channel, 10000, 1000);
            else
                NT_StepMove_S(ntHandle, channel, 200, 4095, 2000);
            break;

        case '\n':
            continue;

        default:
            printf("The input is wrong, please enter again!\n");
            continue;
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // wait until movement has finished
        // in synchronous communication mode, the current status of each channel
        // must be checked periodically ('polled') to know when a movement has
        // finished:
        do 
        {
            Sleep(500);
            NT_GetStatus_S(ntHandle, channel, &status);
        } while (status == NT_TARGET_STATUS);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        if (linearSensorPresent) 
        {
            NT_GetPosition_S(ntHandle, channel, &position);
            printf("Position: %d nm\n", position);
        }
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    } while (key != 'q');

    error = NT_CloseSystem(ntHandle);
    printf("Close system: Error: %u\n", error);
	return 0;
}