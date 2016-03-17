#include <stdio.h> /* Standard input/output definitions */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <float.h>

//#include <iostream> // IOSTREAM POUR COUT

#include "sys_time.h"
#include "log.h"
#include "state.h"
#include "commands.h"
#include "conf.h"

//bloc capteur
#include "capteur/baro_board.h"
#include "capteur/imu.h"
#include "capteur/ahrs.h"
#include "capteur/ahrs/ahrs_int_cmpl_quat.h"
#include "capteur/ahrs/ahrs_aligner.h"
#include "capteur/ins.h"
#include "capteur/ins_int.h"

//bloc asservisement

#include "stabilization/stabilization_attitude_quat_int.h"
#include "stabilization/guidance_v.h"

//bloc moteur
#include "moteur/actuators.h"
#include "moteur/actuators_ardrone2_raw.h"
//Header ARINC653
#include "../../../../include/libApexArinc653/CBasefunction.h"

#define messageSize 256
#define ALTITUDE_MAX 512

FILE* P1 = NULL;

static inline void on_accel_event( void ) {
    ImuScaleAccel(imu);
    
    if (ahrs.status != AHRS_UNINIT) {
        ahrs_update_accel();
    }
}

static inline void on_gyro_event( void ) {
    
    ImuScaleGyro(imu);
    if (ahrs.status == AHRS_UNINIT) {
        ahrs_aligner_run();
        if (ahrs_aligner.status == AHRS_ALIGNER_LOCKED)
            ahrs_align();
    }
    else {
        ahrs_propagate();
        ins_propagate();
    }
}

static inline void on_mag_event(void) {
    ImuScaleMag(imu);
    if (ahrs.status == AHRS_RUNNING) {
        ahrs_update_mag();
    }
}

tid_t main_tid;
tid_t main_periodic_tid;
tid_t print_tid;
tid_t land_tid;
int fin=0;
int inc =0;
int32_t altitude=0;

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it 
	char * name_machine = "192.168.1.1";
	int nbarg = argc;
	// char **argument = new char*[argc];
    //char **argument= malloc( argc *sizeof(char));
	int i = 0;
    Type_Message rMessage;
    
	P1 = fopen("P1.txt", "w");

	for (i = 0; i <= nbarg; i++) 
	{
		//argument[i] = argv[i];
		fprintf(P1, "Argument[%d] : %c\n", i, argv[i]);
	}
    
/*
   	fprintf(P1, "Nom : %s\n", name_machine);
    fprintf(P1, "Port : %d\n", portID);
    fprintf(P1, "MyCVector_Sock : %d\n", myCvector.vqueuing_socket);
	fprintf(P1, "Sock : %d\n", sock);
    fprintf(P1, "Emetteur : %c\n", myCvector.emetteur);
    fprintf(P1, "sMessage : %c\n", sMessage);
    fprintf(P1, "Message size : %d\n", messageSize);
	fclose(P1);*/
    
    /******************************/
    
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
    actuators_led_set(GREEN,GREEN,GREEN,GREEN);
    
	//sock0 = 78576;
    
    printf("Initialisation ARINC653");
    
    //Communication initialization
    COMMUNICATION_VECTOR myCvector;
    
    //myCvector = init_communication(argument, NULL);
    myCvector = init_communication(argv, NULL);
    
    int portID;
    int sock;
    
    char messageReceived[256];
    char sMessage[256];
    
    vector_get(&(myCvector.vqueuing_port), 0, &portID);
    vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    
    /* ENVOIE MESSAGE INIT DONE */
    sprintf(sMessage, "INIT_DONE");
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
    /*****************************/
    
    while(!sys_time_check_and_ack_timer(main_tid)){
        //periodic
        if(sys_time_check_and_ack_timer(main_periodic_tid)){
            imu_periodic();
        }
        
        //event
        imu_event(on_gyro_event, on_accel_event, on_mag_event);
        if(ahrs.status && sys_time_check_and_ack_timer(print_tid)){
            
            stabilization_attitude_run(true);
            guidance_v_run(true,altitude);
            commands[0]=stabilization_cmd[0];
            commands[1]=stabilization_cmd[1];
            commands[2]=stabilization_cmd[2];
            commands[3]=stabilization_cmd[3];
            //printf("%ld\n",stateGetAccelNed_i()->z);
            actuators_set(commands);
            actuators_commit();
        }
        if(ahrs.status && sys_time_check_and_ack_timer(land_tid)){
            /*if(!fin){
                altitude +=512;
                if(altitude >= 5000){
                    fin = 1;
                }
            }else{
                altitude -=512;
            }
            printf("poussée est de %d\n",altitude);*/
            
            if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0) {
                
                sscanf(rMessage.m_message,"%s", &messageReceived); // message received from P2
                printf("Consigne reçu : %s", &messageReceived);
                //altitude+=atoi(&messageReceived.c_str()); //altitude =altitude+&messageReceibed
                // STRING TO INTEGER
                
            } else printf("No new message from P2");
            
            if(altitude >= ALTITUDE_MAX){
                altitude = ALTITUDE_MAX;
            }
            
            printf("Altitude : %d\n",altitude);
        }
        
    }
    
    printf("Fin acquisition\n");
    log_close();
    actuators_ardrone_close();

	return 0;
}

