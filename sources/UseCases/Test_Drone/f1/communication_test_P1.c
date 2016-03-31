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

#include "droneMsgHeader.h"

#define messageSize 256

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

static tid_t main_tid;
static tid_t main_periodic_tid;
static tid_t print_tid;
static tid_t land_tid;
static tid_t takeoff_tid;
static int fin=0;
static int inc =0;
static int32_t droneCMD = 0;  /*0 nothing, 1 takeoff 2 land*/
static int32_t altSetPt = 0;    /*fixed point representation: Q23.8. in m*/
static int32_t altReal = 0;  /*in cm*/

int main(int argc, char *argv[])
{
	//The simulator isn't able to find the machine name alone, we have to hardcode it
	char * name_machine = "127.0.0.1";
	int nbarg = argc;
	// char **argument = new char*[argc];
    //char **argument= malloc( argc *sizeof(char));
	int i = 0;
    Type_Message rMessage[2];
    int msgIndex[2] = {0,0};

    int16_t height=0;

    int count=0;


	for (i = 0; i <= nbarg; i++) {
		printf("Argument[%d] : %s\n", i, argv[i]);
	}

    printf("Initialisation of Paparazzi...\n");
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

    main_tid = sys_time_register_timer(60,NULL);
    main_periodic_tid = sys_time_register_timer((1./PERIODIC_FREQUENCY),NULL);
    print_tid = sys_time_register_timer((1./500.),NULL);
    land_tid = sys_time_register_timer(1./15.,NULL);
    takeoff_tid = sys_time_register_timer(1./5.,NULL);


    printf("Update IMU...\n");
    actuators_led_set(GREEN,GREEN,GREEN,GREEN);

    printf("Initialisation ARINC653...\n");

    //Communication initialization
    COMMUNICATION_VECTOR myCvector;

    //myCvector = init_communication(argument, NULL);
    myCvector = init_communication(argv, NULL);

    int portID[10];
    int sock[10];

    int32_t msgID = 0;
    int32_t msgID_sent = 0;

    char sMessage[256];

	int t=0;

    memset(sMessage, 0, 256);
	
    printf("port number: %d\n", myCvector.vqueuing_port.size);
    for(i=0; i<myCvector.vqueuing_port.size; i++) {
        vector_get(&(myCvector.vqueuing_port), i, &portID[i]);
        vector_get(&(myCvector.vqueuing_socket), i, &sock[i]);
    }
    
    sprintf(sMessage, "P1 INIT_DONE");
    SEND_QUEUING_MESSAGE(name_machine, portID[0], sock[0], myCvector.emetteur, sMessage, sizeof(sMessage)); 

    while(1) {

        //period
        if(sys_time_check_and_ack_timer(main_periodic_tid)) {
            imu_periodic();
        }

        //event
        imu_event(on_gyro_event, on_accel_event, on_mag_event);
        if(ahrs.status && sys_time_check_and_ack_timer(print_tid)) {
       	    
            height = navdata_getHeight();

            sleep(1);

            msgID_sent = DRONE_MSG_ALT;
            altReal = (int)height;

            memcpy(sMessage, &msgID_sent, 4);
            memcpy(sMessage+4, &altReal, 4);
            printf("Height = %d\n", altReal);
            SEND_QUEUING_MESSAGE(name_machine, portID[0], sock[0], myCvector.emetteur, sMessage, sizeof(sMessage));

            //printf("droneCMD = %d and msgID_sent = %d\n", droneCMD, msgID_sent);

            if (droneCMD!=2 && RECEIVE_QUEUING_MESSAGE(sock[0], &rMessage[0]) > 0) {
                memcpy(&msgID, rMessage[0].m_message, 4);
                printf("P1 received msgID = %d\n", msgID);
                switch(msgID) {

                case DRONE_MSG_ALT:  //we intended to use this ID msg as a setpoint altitude of the drone
                    memcpy(&altSetPt, rMessage[0].m_message+4, 4);
                    altSetPt *= 2.56;  /*1cm: 0.01m*2^8=2.56 because of fixed point representation: Q23.8*/
                    break;
                case DRONE_MSG_CMD: // Control commands from the second partition
                    memcpy(&droneCMD, rMessage[0].m_message+4, 4);
                    printf("droneCMD = %d\n", droneCMD);
                    break;
                default:

                    break;
                }
            }

            if (droneCMD==1 && msgID==DRONE_MSG_CMD) { //takeoff            
                // count = 100 and +=1 too much amplitude but works
                // 400 - 778 works well
                if(count>300) {  //use this counter to control the frequency of the acuator cmds. 
                                //however this is only based on experience.
                    count=0;

                    if (altSetPt <= 704) {
               	     	altSetPt += 2*(1<<6);
				        printf("Increasing thrust : %d\n", altSetPt);
                    }
                    else if (altReal < 150) {
               	     	altSetPt += 1;
                        printf("Increasing thrust : %d\n", altSetPt);
                    }
                	else {
                    	altSetPt -= 1;
				        printf("Decreasing thrust : %d\n", altSetPt);
                    }

               	guidance_v_run(true,altSetPt);

                }

            count++;

            //droneCMD = 0;
            }
            else if (droneCMD==2 && msgID==DRONE_MSG_CMD) { //land
            
                //printf("Landing\n");

                if(ahrs.status && sys_time_check_and_ack_timer(land_tid)) {
                    
                    printf("altSetPt : %d\n",altSetPt); 

                    if(altReal<1) {
                        
                        if (altSetPt > 64)
                            altSetPt -= 64; 
                        else {
                            stabilization_cmd[0] = 0;
                            stabilization_cmd[1] = 0;
                            stabilization_cmd[2] = 0;
                            stabilization_cmd[3] = 0;
                            altSetPt = 0;
                        }
                    }
                    else {
                        if (altReal > 2) {
				            if (altSetPt > 785)
                                altSetPt -= 2; 
                            else 
                                altSetPt = 781;  
                        }                  
                        else
                           altSetPt = 0;
                        
                    guidance_v_run(true,altSetPt);

                    }
                }
            }
            else
                ;

            stabilization_attitude_run(true);

            commands[0]=stabilization_cmd[0];
            commands[1]=stabilization_cmd[1];
            commands[2]=stabilization_cmd[2];
            commands[3]=stabilization_cmd[3];
            //printf("%ld\n",stateGetAccelNed_i()->z);
            actuators_set(commands);
            actuators_commit();
        }        
    }

    printf("End of main program...\n");

    log_close();
    actuators_ardrone_close();

	return 0;
}
