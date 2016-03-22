#include "CBasefunction.h"
//#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define messageSize 256

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it 
	char * name_machine = "127.0.0.1";
	int nbarg = argc;
//	char **argument = new char*[argc];
	int i = 0;
    Type_Message rMessage;
    
	/*P2 = fopen("P2.txt", "w");

	for (i = 0; i <= nbarg; i++) 
	{
//		argument[i] = argv[i];
		fprintf(P2, "Argument[%d] : %s\n", i, argv[i]);
	}*/
    
    printf("Initialisation ARINC653");
    
	//Communication initialization
	COMMUNICATION_VECTOR myCvector;
	
	myCvector = init_communication(argv, NULL);
    
	int portID;
	int sock;
    
    char sMessage[256];
    char sMessage_rcv[256];
    char sMessage_stop[256];

	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    
    // Début de la séquence de communication - P2
    
    int j = 0;
    int STOP=0;
    sprintf(sMessage_stop, "SENT_FROM_P1_STOP_OK");
    
    while (STOP == 0) {

    	sprintf(sMessage_rcv, "SENT_FROM_P1_%d_P2", j);
        
        if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0) {
            

            
            if (j > 20) {
                sprintf(sMessage, "SENT_FROM_P2_STOP");
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
            }
            else if (strcmp(rMessage.m_message,sMessage_rcv) == 0) {
                sprintf(sMessage, "SENT_FROM_P2_%d", j);
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
                printf("Message received from P1: %s\n\n", rMessage.m_message);
                j++;
                
            }
            else if (strcmp(rMessage.m_message,sMessage_stop) == 0) {
                
               // STOP = 1;
                printf("Fin communication P2\n");
                
            } else {
            	//printf("ERROR P2 : %s\n\n", rMessage.m_message);
            	//printf("sMessage_rcv : %s\n", sMessage_rcv);
            	//printf("sMessage : %s\n", sMessage);
            	//printf("rMessage : %s\n", rMessage.m_message);
            	//printf("rMessage.m_message : %s\n", rMessage.m_message);
            	//printf("strcmp : %d" , strcmp(rMessage.m_message,sMessage_rcv));
            }
            
        } else {
        	//printf("<<< No new message from P1\n");
        	sleep(5);
        }
        
        sleep(1);
        
    }
    
	return 0;
}

