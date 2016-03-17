#include "CBasefunction.h"
//#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define messageSize 256

FILE* P2 = NULL;

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it 
	char * name_machine = "192.168.1.1";
	int nbarg = argc;
//	char **argument = new char*[argc];
	int i = 0;
    Type_Message rMessage;
    
	P2 = fopen("P2.txt", "w");

	for (i = 0; i <= nbarg; i++) 
	{
//		argument[i] = argv[i];
		fprintf(P2, "Argument[%d] : %s\n", i, argv[i]);
	}
    
    printf("Initialisation ARINC653");
    
	//Communication initialization
	COMMUNICATION_VECTOR myCvector;
	
	myCvector = init_communication(argv, NULL);

    int j = 0;
	int portID;
	int sock;
    
    char messageReceived[256];
    char sMessage[256];
    sprintf(sMessage, "P2 %d", j);

	vector_get(&(myCvector.vqueuing_port), 0, &portID);
	vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    

   	fprintf(P2, "Nom : %s\n", name_machine);
    fprintf(P2, "Port : %d\n", portID);
    fprintf(P2, "MyCVector_Sock : %d\n", myCvector.vqueuing_socket);
	fprintf(P2, "Sock : %d\n", sock);
    fprintf(P2, "Emetteur : %s\n", myCvector.emetteur);
    fprintf(P2, "sMessage : %s\n", sMessage);
    fprintf(P2, "Message size : %d\n", messageSize);
	fclose(P2);
    
    /******************************/
    /*
    printf("Initialisation\n");
    actuators_init();
    actuators_led_set(RED,RED,RED,RED);
    
    sys_time_init();
    log_init();
    stateInit();
    
    
    imu_init();
    ahrs_aligner_init();
    ahrs_init();
    
    stabilization_attitude_init();
    guidance_v_init();
    ins_init();
    
    main_tid = sys_time_register_timer(40,NULL);
    main_periodic_tid = sys_time_register_timer((1./PERIODIC_FREQUENCY),NULL);
    print_tid = sys_time_register_timer((1./500.),NULL);
    land_tid = sys_time_register_timer((4),NULL);
    
    
    printf("Update imu\n");
    actuators_led_set(GREEN,GREEN,GREEN,GREEN);*/
    
	//sock0 = 78576;
    
    for ( ; ; ){

        /* ENVOI DU MESSAGE EN QUEUING */
        SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
        printf("Message sent from P2: %s\n\n", sMessage);
        
        
        /* UPDATING MESSAGE */
        sprintf(sMessage, "test P2 %d", j);
        j++;
        
        /* RECEPTION MESSAGE EN QUEUING */
        
        if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0) {
            
            sscanf(rMessage.m_message,"%s", &messageReceived); // message received from P2
            printf("Message received from P1: %s\n\n", &messageReceived);
            
        } else printf("<<< No new message from P1\n");
        
        usleep(100000);
        
    }
    /*
	usleep(100*1000);

	const int c_takeoff = (1<<9)|(1<<18)|(1<<20)|(1<<22)|(1<<24)|(1<<28);
	const int c_land = (1<<18)|(1<<20)|(1<<22)|(1<<24)|(1<<28);
	
	//We have to send several time the same command to be sure that the drone didn't miss it
	//Take off command
	for (int i = 0; i < c_takeoff_ms; i+=c_interval_ms)
	{
		sprintf(sMessage, "AT*REF=%d,%d\r", sequence++, c_takeoff);
		SEND_QUEUING_MESSAGE(name_machine, portID0, sock0, myCvector.emetteur, sMessage, messageSize);
		usleep(c_interval_us);
		memset(sMessage, 0, 30); 
	}

	//Landing command
	for (int i = 0; i < c_land_ms; i+=c_interval_ms)
	{
		sprintf(sMessage, "AT*REF=%d,%d\r", sequence++, c_land);
		SEND_QUEUING_MESSAGE(name_machine, portID0, sock0, myCvector.emetteur, sMessage, messageSize);
		usleep(c_interval_us);
		memset(sMessage, 0, 30); 
	}

	//We put the partition in an infinite loop because the simulator keeps working
	while(1)
	{
	 std::cout<<" End of control partition " <<std::endl;
	}*/

	return 0;
}

