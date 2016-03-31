#include "CBasefunction.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "droneMsgHeader.h"

#define messageSize 256

static int32_t msgID = 0;
static int32_t msgCont = 0;

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it
	char * name_machine = "127.0.0.1";

    Type_Message rMessage;

    printf("Initialisation ARINC653\n");

	//Communication initialization
	COMMUNICATION_VECTOR myCvector;

	myCvector = init_communication(argv, NULL);

	int portID;
	int sock;

    char sMessage[256];

	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);

    memset(sMessage, 0, 256);

    while(RECEIVE_QUEUING_MESSAGE(sock, &rMessage) <= 0) {
        ;
    }

    if(strcmp(rMessage.m_message,"P1 INIT_DONE") == 0) {
        printf("P2 Beginning of command loop... \n");
    }
    else {
        printf("P2 failed : Received '%s' from P1\n", rMessage.m_message);
        return 0;
    }

    sleep(1);
    msgID = DRONE_MSG_CMD;
    msgCont = 1;  //take off
    memcpy(sMessage,&msgID, 4);
    memcpy(sMessage+4, &msgCont, 4);
    printf("P2 send takeoff command = %d,%d\n", msgID, msgCont);
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));

    for (int i = 0; i<20 ; i++) { // 400 initially
        if(RECEIVE_QUEUING_MESSAGE(sock, &rMessage)>0) {            
            //TEST
            memcpy(&msgID, rMessage.m_message, 4);
            memcpy(&msgCont, rMessage.m_message+4, 4);
            printf("P2 received msg ID=%d; msgCont=%d\n", msgID, (int)msgCont);         

            switch(msgID) {

            case DRONE_MSG_ALT:
                memcpy(&msgCont, rMessage.m_message+4, 4);
                printf("P2 received - Height = %d cm\n", (int)msgCont);
                break;
            default:
                break;
            }
        }

        sleep(1);
    }

    msgID = DRONE_MSG_CMD;
    msgCont = 2; //land
    memcpy(sMessage,&msgID, 4);
    memcpy(sMessage+4, &msgCont, 4);
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));

    sleep(5);

    printf("End of partition 2...\n");

	return 0;
}
