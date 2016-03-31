#include "CBasefunction.h"
//#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define messageSize 256

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it 
	//char * name_machine = "127.0.0.1";
    
    printf("Initialisation ARINC653 P3 \n");
    
	//Communication initialization
	COMMUNICATION_VECTOR myCvector;
	
	myCvector = init_communication(argv, NULL);
    
	int portID;
	int sock;

	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    
    // Début de la séquence de communication - P3
    
    while(1) {
        printf("P3_ON\n");
        sleep(10);
    }
    
	return 0;
}