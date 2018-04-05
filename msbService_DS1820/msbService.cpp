/* 
 * Service template for mBed c++
 * 
 * To use this template, simply add your code in appService
*/

#include "msbConfig.h"

#include "msb.h"
#include "mbed.h"
#include "msbService.h"
#include "msbServiceConfig.h"
#include "msbDebug.h"

#include "DS1820.h"

time_t rawtime;
void appService(void)
{
    DS1820 probe(PTA1);

    TRACE(INFO, DEBUG_CONSOLE | DEBUG_MSB, "[SERVICE] Start msb service\r\n");

    time_t sendTimeout = 0;
    struct tm *tmStamp;
    char tmStampBuffer[26];

    char serviceMessage[100];

    double temp = -1000.0;

    // Run while msb.runMsbService active, msb core can stop service
    while (msb.runMsbService)
    {

        printf("[SERVICE] Loop..\r\n"); // Print to stdio

        if (time(NULL) > sendTimeout)
        {
            // Get current time and format it for print
            time(&rawtime);
            tmStamp = localtime(&rawtime);
            strftime(tmStampBuffer, 26, "%Y-%m-%d %H:%M:%S", tmStamp);

            //Start temperature conversion, wait until ready
            probe.convertTemperature(true, DS1820::all_devices);

            temp = probe.temperature();

            // Check that readings is within valid range
            if ((temp > -55.0) && (temp < 125.0))
            {
                // Create message to hub
                sprintf(serviceMessage, "{\"mt\": \"%s\",\"data\": \"%3.1f\",\"timeStamp\": \"%s\"}",
                        "jonasdemo", probe.temperature(), tmStampBuffer);
            }
            else
            {
                // Create message to hub
                sprintf(serviceMessage, "{\"mt\": \"%s\",\"data\": \"%s\",\"timeStamp\": \"%s\"}",
                        "jonasdemo", "null", tmStampBuffer);

                temp = -1000.0;
            }

            // Send message to IoT Hub queue
            sendMessage(serviceMessage);
            TRACE(INFO, DEBUG_CONSOLE | DEBUG_MSB, "[SERVICE] Message from Service -> Hub: Temperature: %3.1f\r\n", temp);

            sendTimeout = time(NULL) + MESSAGE_INTERVAL;
        }

        // Read message queue from service thread
        osEvent evt = mailHubRecive.get((uint32_t)500);
        if (evt.status == osEventMail)
        {
            messageHub *mail = (messageHub *)evt.value.p;

            TRACE(INFO, DEBUG_CONSOLE | DEBUG_MSB, "[SERVICE] Message from Hub -> Service: %s\r\n", mail->message);

            mailHubRecive.free(mail);
        }

        wait(LOOP_DELAY);
    }
}
