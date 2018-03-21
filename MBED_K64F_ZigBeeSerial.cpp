/* 
 * UART Example
 * 
 * Send data over UART to ZigBee
 * 
 */

#include "msb.h"
#include "mbed.h"
#include "msbService.h"

time_t rawtime;

void appService(void)
{
    // Declare UART for zigbee
    Serial zigbee(PTC17, PTC16); // tx, rx for K64F UART Arduino 
    
    printf("[SERVICE] Start msb service on K64F version 0.2\r\n"); // Print to stdio

    time_t sendTimeout = 0;
   
    // Run while msb.runMsbService active, msb core can stop service
    
    while( msb.runMsbService ){
        
        printf("[SERVICE] Loop..\r\n"); // Print to stdio
        
        if( time(NULL) > sendTimeout )
        {
            time(&rawtime); // Get current time

            // Send message to UART ZigBee
            zigbee.printf( "Time is: %s\r\n", ctime(&rawtime) );

            sendTimeout = time(NULL) + 30;

        }
        

        // Read message queue from service thread
        osEvent evt = mailHubRecive.get((uint32_t) 500);
        if (evt.status == osEventMail)
        {
            messageHub *mail = (messageHub *)evt.value.p;
            printf("[SERVICE] Message from Hub -> Service: %s\r\n", mail->message);
            
            mailHubRecive.free(mail);
        }

        wait(10.0f);
        
    }
    
}
 
