#include "CBasefunction.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "droneMsgHeader.h"

#define messageSize 256
#define ALTITUDE_STEP (1<<6)
#define ALTITUDE_MAX (1<<8)


static int32_t msgID = 0;
static int32_t msgCont = 0;

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it
	char * name_machine = "127.0.0.1";
	int nbarg = argc;
	char **argument = new char*[argc];
	int i = 0;
    Type_Message rMessage;

	for (i = 0; i <= nbarg; i++)
	{
		argument[i] = argv[i];
	}

    printf("Initialisation ARINC653\n");

	//Communication initialization
	COMMUNICATION_VECTOR myCvector;

	myCvector = init_communication(argument, NULL);

	int portID;
	int sock;

    char messageReceived[256];
    char sMessage[256];
    char sMessage_rcv[256];

	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);

    int INIT_DONE = 0;

    i = 0;

    memset(sMessage, 0, 256);





#if 0
	while(1)
	{

        if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0)
        {
            printf("P2 received: %s\n", rMessage.m_message);
            i++;
            sprintf(sMessage,"P2 send No %d", i);
            SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
        }
        else
        {
            printf("P2 received nothing\n");
            sleep(1);
        }
	}
#endif


    while(RECEIVE_QUEUING_MESSAGE(sock, &rMessage) <= 0) {
        ;
    }

    if(strcmp(rMessage.m_message,"P1 INIT_DONE") == 0) {
        printf("P2 Beginning of command loop... \n");
    }
    else
    {
        printf("p2 failed : Received %s from P1\n", rMessage.m_message);
        return 0;
    }

    sleep(1);
    msgID = DRONE_MSG_CMD;
    msgCont = 1;  //take off
    memcpy(sMessage,&msgID, 4);
    memcpy(sMessage+4, &msgCont, 4);
    printf("p2 send takeoff cmd = %d\n", msgCont);
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));

    for (i=0; i<400 ; i++) { 
        if(RECEIVE_QUEUING_MESSAGE(sock, &rMessage)>0)
        {
            memcpy(&msgID, rMessage.m_message, 4);
            switch(msgID)
            {
            case DRONE_MSG_ALT:
                memcpy(&msgCont, rMessage.m_message+4, 4);
                printf("P2 received height= %d cm\n", (int)msgCont);
                break;
            default:
                break;
            }
        }

        usleep(100000);
    }

    msgID = DRONE_MSG_CMD;
    msgCont = 2; //land
    memcpy(sMessage,&msgID, 4);
    memcpy(sMessage+4, &msgCont, 4);
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
    sleep(3);



	return 0;
}

