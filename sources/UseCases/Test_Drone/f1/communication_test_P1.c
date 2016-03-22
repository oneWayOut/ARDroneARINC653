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
int inc=0;
int32_t altitude=0;

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it 
	char * name_machine = "127.0.0.1";
	int nbarg = argc;
	// char **argument = new char*[argc];
    // char **argument= malloc( argc *sizeof(char));
	int i = 0;
    Type_Message rMessage;
 
 #if 0   
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
    actuators_led_set(RED,GREEN,GREEN,GREEN);
  #endif

    //printf("Initialisation ARINC653");
    printf("Hello Cai");
    //Communication initialization
    COMMUNICATION_VECTOR myCvector;
    
    //myCvector = init_communication(argument, NULL);
    myCvector = init_communication(argv, NULL);
    
    int portID;
    int sock;
    
    char sMessage[256];
    char sMessage_P2[256];
    char sMessage_P3[256];
    char sMessage_rcv_1[256];
    char sMessage_rcv_2[256];
    char sMessage_stop[256];
    
    vector_get(&(myCvector.vqueuing_port), 0, &portID);
    vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    
    // Début de la séquence de communication - P1
    
    int k = 0;
    int j = 0;
    int STOP = 0;
    
    sprintf(sMessage_stop, "SENT_FROM_P2_STOP");
    
    sprintf(sMessage_P2, "SENT_FROM_P1_%d_P2", j);
    SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
    printf("Envoi du message : %s",sMessage);
    
    while (STOP == 0) {

        sprintf(sMessage_rcv_1, "SENT_FROM_P2_%d", j);
        sprintf(sMessage_rcv_2, "SENT_FROM_P3_%d", k);

        if (RECEIVE_QUEUING_MESSAGE(sock, &rMessage) > 0) {            
            
            if (strcmp(rMessage.m_message,sMessage_rcv_1) == 0) {
                
                sprintf(sMessage_P3, "SENT_FROM_P1_%d_P3", k);
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage_P3, sizeof(sMessage_P3));
                printf("Message received from P2: %s\n\n", rMessage.m_message);
                k++;
                
            }
            else if (strcmp(rMessage.m_message,sMessage_rcv_2) == 0) {
                
                j++;
                sprintf(sMessage_P2, "SENT_FROM_P1_%d_P2", j);
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage_P2, sizeof(sMessage_P2));
                printf("Message received from P3: %s\n\n", rMessage.m_message);
            
            }
            else if (strcmp(rMessage.m_message,sMessage_stop) == 0) {
                
                sprintf(sMessage, "SENT_FROM_P1_STOP_OK");
                SEND_QUEUING_MESSAGE(name_machine, portID, sock, myCvector.emetteur, sMessage, sizeof(sMessage));
                
                printf("Fin communication P1\n");
#if 0
                log_close();
                actuators_ardrone_close();
#endif
               // STOP = 1;
                
            }
            else {

                printf("ERROR P1 : %s\n\n", rMessage.m_message);
                //printf("sMessage_rcv : %s\n", sMessage_rcv);
                //printf("sMessage : %s\n", sMessage);
                //printf("rMessage : %s\n", rMessage.m_message);
                //printf("rMessage.m_message : %s\n", rMessage.m_message);
                //printf("strcmp : %d" , strcmp(rMessage.m_message,sMessage_rcv));

            }
        
        } 
	   else {
            //printf("<<< No new message from P2\n");
            //printf("sMessage_rcv : %s\n", sMessage_rcv);
            //printf("sMessage : %s\n", sMessage);
            //printf("rMessage : %s\n", rMessage.m_message);
            //printf("rMessage.m_message : %s\n", rMessage.m_message);
            //printf("strcmp : %d" , strcmp(rMessage.m_message,sMessage_rcv));
		sleep(5);
	}
        
        sleep(1);
    }

	return 0;
}

