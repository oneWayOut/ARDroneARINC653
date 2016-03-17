#CC=arm-none-linux-gnueabi-gcc
CC=arm-none-linux-gnueabi-g++
LS=$(CC)
CFLAGS= -march=armv7-a -lm

EXE = communication_test_P1 \
	  clean

all:	$(EXE)

communication_test_P1: 	communication_test_P1.o 			    					\
			log.o					     						\
			math/pprz_trig_int.o 								\
			math/pprz_orientation_conversion.o					\
			math/pprz_geodetic_double.o							\
			math/pprz_geodetic_float.o							\
			math/pprz_geodetic_int.o							\
			state.o												\
			sys_time.o											\
			sys_time_arch.o										\
			capteur/ahrs/ahrs_int_cmpl_quat.o					\
			capteur/ahrs/ahrs_aligner.o 						\
			capteur/ahrs.o 										\
			capteur/baro_board.o 								\
			capteur/navdata.o  									\
			capteur/imu.o 										\
			capteur/ins.o										\
			capteur/ins_int.o									\
			capteur/vf_float.o									\
			stabilization/stabilization_attitude_ref_quat_int.o \
			stabilization/stabilization_attitude_quat_int.o 	\
			stabilization/guidance_v.o							\
			stabilization/guidance_v_ref.o						\
			moteur/gpio_ardrone.o								\
			moteur/actuators_ardrone2_raw.o						\
			moteur/actuators.o									\
			moteur/motor_mixing.o								\
			commands.o 									        \
			../../../../build/libApexArinc653/CBasefunction.o   \
			../../../../build/libApexArinc653/CArgument.o       \
			../../../../build/libApexArinc653/CQueuing.o        \
			../../../../build/libApexArinc653/CSampling.o       \
			../../../../build/libApexArinc653/Vector.o
#			CBasefunction.o

	$(CC) $(CFLAGS) -o $@.out $^


.PHONY: depend clean realclean all tests

depend:
	makedepend -Y -r *.c

clean:
	 $(RM) *.elf
	-$(RM) *.o
	-$(RM) */*.o
	-$(RM) */*/*.o
	-$(RM) gmon.out
	-$(RM) *.bak core Makefile.bak
	-$(RM) $(EXE) a.out

realclean: clean

# DO NOT DELETE

commands.o: commands.h conf.h
log.o: log.h
state.o: state.h math/pprz_algebra_int.h std.h math/pprz_algebra.h
state.o: math/pprz_trig_int.h math/pprz_algebra_int.h
state.o: math/pprz_algebra_float.h math/pprz_geodetic_int.h
state.o: math/pprz_geodetic.h math/pprz_geodetic_float.h
state.o: math/pprz_algebra_float.h math/pprz_orientation_conversion.h std.h
sys_time_arch.o: sys_time.h std.h sys_time_arch.h
sys_time.o: sys_time.h std.h sys_time_arch.h
#stabilization_attitude_quat_int.o: stabilization_attitude_quat_int.h
communication_test_P1.o: sys_time.h std.h sys_time_arch.h log.h state.h
communication_test_P1.o: math/pprz_algebra_int.h std.h math/pprz_algebra.h
communication_test_P1.o: math/pprz_trig_int.h math/pprz_algebra_int.h
communication_test_P1.o: math/pprz_algebra_float.h math/pprz_geodetic_int.h
communication_test_P1.o: math/pprz_geodetic.h math/pprz_geodetic_float.h
communication_test_P1.o: math/pprz_algebra_float.h math/pprz_orientation_conversion.h
communication_test_P1.o: commands.h conf.h capteur/baro_board.h capteur/navdata.h
communication_test_P1.o: capteur/imu.h math/pprz_algebra_int.h
communication_test_P1.o: math/pprz_algebra_float.h capteur/conf_capteur.h
communication_test_P1.o: capteur/ahrs.h state.h capteur/ahrs/ahrs_int_cmpl_quat.h
communication_test_P1.o: capteur/ahrs.h std.h math/pprz_algebra_int.h
communication_test_P1.o: capteur/ahrs/ahrs_aligner.h capteur/ins.h
communication_test_P1.o: math/pprz_geodetic_int.h capteur/ins_int.h capteur/ins.h
communication_test_P1.o: stabilization/stabilization_attitude_quat_int.h
communication_test_P1.o: stabilization/stabilization_attitude_common_int.h
communication_test_P1.o: stabilization/conf_asser.h
communication_test_P1.o: stabilization/stabilization_attitude_ref_quat_int.h
communication_test_P1.o: stabilization/stabilization_attitude_ref_int.h
communication_test_P1.o: stabilization/guidance_v.h stabilization/guidance_v_ref.h
communication_test_P1.o: math/pprz_algebra.h stabilization/guidance_v_adpt.h
communication_test_P1.o: moteur/actuators.h moteur/conf_moteur.h
communication_test_P1.o: ../../../../include/libApexArinc653/CBasefunction.h
