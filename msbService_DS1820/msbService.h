#pragma once

void appService(void);

static int sendMessage(const char* message){
        messageHub *mail = mailHubSend.alloc();

        if( mail != NULL ){
            strcpy(mail->message, message);
            mailHubSend.put(mail);
        }else{
            printf("[Service Error]: Message queue to hub is full\r\n");
            return -1;
        }
    return 0;
}