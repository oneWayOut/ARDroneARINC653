#include "CBasefunction.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define messageSize 256
#define ALTITUDE_STEP 100
#define ALTITUDE_MAX 500

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
    
	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    
    int INIT_DONE = 0;
    
    if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0) {
        sscanf(rMessage.m_message,"%s", &messageReceived); // message received from P2
        printf("Message received from P1: %s", &messageReceived);
    }
    
    if (&messageReceived == "INIT_DONE") {
        
        int altitude = 0;
        
        for ( ; ; ) {
            
            while (altitude < ALTITUDE_MAX) {
                sprintf(sMessage, "100");
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
                printf("Message sent from P2: %s", sMessage);
                
                altitude += ALTITUDE_STEP;
                
                usleep(10000000); // 1s
            }
            
            usleep(100000000); // 10s
            
            while (altitude != 0) {
                sprintf(sMessage, "-100");
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
                printf("Message sent from P2: %s", sMessage);
                
                altitude -= ALTITUDE_STEP;
                
                usleep(10000000); // 1s
            }
            
        }
    }

	return 0;
}

