#include "CBasefunction.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define messageSize 256
#define ALTITUDE_STEP (1<<6)
#define ALTITUDE_MAX (1<<8)

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it
	char * name_machine = "192.168.1.1";
	int nbarg = argc;
	char **argument = new char*[argc];
	int i = 0;
    Type_Message rMessage;

	for (i = 0; i <= nbarg; i++)
	{
		argument[i] = argv[i];
	}

    printf("Initialisation ARINC653");

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

    while(RECEIVE_QUEUING_MESSAGE(sock, &rMessage) <= 0) {
        ;
    }

    printf("Message received from P1: %s", rMessage.m_message);

    strcpy(sMessage_rcv,"INIT_DONE");

    if(strcmp(rMessage.m_message,sMessage_rcv) == 0) {

        printf("Beginning of command loop... \n");
        int32_t altitude = 0;

        for ( ; ; ) {

            while (altitude < ALTITUDE_MAX) {
                sprintf(sMessage, "%d", ALTITUDE_STEP);
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
                printf("Message sent from P2: %s", sMessage);

                altitude += ALTITUDE_STEP;

                usleep(10000);
            }

            usleep(10000);

            // while (altitude != 0) {
            //     sprintf(sMessage, "%d", -ALTITUDE_STEP);
            //     SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
            //     printf("Message sent from P2: %s", sMessage);

            //     altitude -= ALTITUDE_STEP;

            //     usleep(10000);
            // }

        }
    }

	return 0;
}

