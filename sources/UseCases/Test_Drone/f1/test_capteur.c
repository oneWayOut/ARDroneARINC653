#include <stdio.h>   /* Standard input/output definitions */
#include <stdbool.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <float.h>

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

int main(){
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
			if(!fin){
				altitude +=512;
				if(altitude >= 5000){
					fin = 1;
				}
			}else{
				altitude -=512;
			}
			printf("poussée est de %d\n",altitude);
		}

	}
	printf("Fin acquisition\n");
	log_close();
	actuators_ardrone_close();
}
