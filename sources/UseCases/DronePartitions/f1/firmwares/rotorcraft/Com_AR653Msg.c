#include <stdio.h>
#include <string.h>
#include "Com_AR653Msg.h"


static COMMUNICATION_VECTOR myCvector;
static char sMessage[256];

//handle multi partitions
static int portID[10] = {0};
static int sock[10] = {0};
static Type_Message rMessage[2];


static char * name_machine = "127.0.0.1";

void msg653ComInit(int argc, char *argv[])
{
	int i;

	for(i = 0; i<argc; i++)
	{
		printf("Argument[%d] : %s\n", i, argv[i]);
	}

	myCvector = init_communication(argv, NULL);

	printf("Port numbers: %d\n", myCvector.vqueuing_port.size);
    for(i=0; i<myCvector.vqueuing_port.size; i++)
    {
        vector_get(&(myCvector.vqueuing_port), i, &portID[i]);
        vector_get(&(myCvector.vqueuing_socket), i, &sock[i]);
    }

    memset(sMessage, 0, 256);

    strcpy(sMessage, "P1 INIT_DONE");
    SEND_QUEUING_MESSAGE(name_machine, portID[0], sock[0], myCvector.emetteur, sMessage, strlen(sMessage));

}

void msg653ComPeriod(void)
{
	static int msgIndex = 1;


	if (RECEIVE_QUEUING_MESSAGE(sock[0], &rMessage[0]) > 0)
	{
		printf("P1 received: %s", rMessage[0].m_message);
	}

	sprintf(sMessage, "P1 send No.%d msg\n", msgIndex);
	SEND_QUEUING_MESSAGE(name_machine, portID[0], sock[0], myCvector.emetteur, sMessage, strlen(sMessage));
	msgIndex++;
}
