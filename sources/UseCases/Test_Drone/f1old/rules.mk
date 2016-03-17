sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/f1.out)
TARGETS 				+= $(call SRC_2_BIN, $(TARGET))
OBJS_$(d)				:= $(call SRC_2_OBJ, \
						$(d)/communication_test_P1.o 							\
						$(d)/../paparazzi/log.o 							\
						$(d)/../paparazzi/math/pprz_trig_int.o 					\
						$(d)/../paparazzi/math/pprz_orientation_conversion.o 			\
						$(d)/../paparazzi/math/pprz_geodetic_double.o	 				\
						$(d)/../paparazzi/math/pprz_geodetic_float.o					\
						$(d)/../paparazzi/math/pprz_geodetic_int.o					\
						$(d)/../paparazzi/state.o 							\
						$(d)/../paparazzi/sys_time.o 							\
						$(d)/../paparazzi/sys_time_arch.o 						\
						$(d)/../paparazzi/capteur/ahrs/ahrs_int_cmpl_quat.o 				\
						$(d)/../paparazzi/capteur/ahrs/ahrs_aligner.o 				\
						$(d)/../paparazzi/capteur/ahrs.o						\
						$(d)/../paparazzi/capteur/baro_board.o  					\
						$(d)/../paparazzi/capteur/navdata.o  						\
						$(d)/../paparazzi/capteur/imu.o  						\
						$(d)/../paparazzi/capteur/ins.o 						\
						$(d)/../paparazzi/capteur/ins_int.o 						\
						$(d)/../paparazzi/capteur/vf_float.o 						\
						$(d)/../paparazzi/stabilization/stabilization_attitude_ref_quat_int.o	\
                        $(d)/../paparazzi/stabilization/stabilization_attitude_quat_int.o  		\
                        $(d)/../paparazzi/stabilization/guidance_v.o					\
						$(d)/../paparazzi/stabilization/guidance_v_ref.o	  			\
						$(d)/../paparazzi/moteur/gpio_ardrone.o	 				\
						$(d)/../paparazzi/moteur/actuators_ardrone2_raw.o  				\
						$(d)/../paparazzi/moteur/actuators.o  					\
						$(d)/../paparazzi/moteur/motor_mixing.o  					\
						$(d)/../paparazzi/commands.o							\
						$(d)/../../../build/libApexArinc653/*.o
					)

OBJECTS 	+= $(OBJS_$(d))
$(OBJS_$(d))	:  CC_FLAGS_SPEC	:= -lpthread -march=armv7-a -lm
$(TARGET)	:  $(OBJS_$(d))

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
