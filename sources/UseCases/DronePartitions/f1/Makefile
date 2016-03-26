#Makefile.arm-linux-toolchain
PREFIX ?= arm-linux-gnueabi
##cai change from gcc to g++, we need g++ for linking simulator library
CC   = $(PREFIX)-g++
LD   = $(PREFIX)-g++


#Makefile.arm-linux
FLOAT_ABI ?= -mfloat-abi=softfp -mfpu=vfp
ARCH_CFLAGS ?= -mtune=cortex-a8 -march=armv7-a

# add ARM specifc flags to CFLAGS, LDFLAGS
CFLAGS += $(FLOAT_ABI) $(ARCH_CFLAGS)
LDFLAGS += $(FLOAT_ABI)

#Makefile.linux
#CC  ?= gcc
#LD  ?= $(CC)

# Launch with "make Q=''" to get full command display
Q=@

OPT ?= 3
# Slightly bigger .elf files but gains the ability to decode macros

CSTANDARD ?= -std=gnu99

#
# Common compiler flags.
# add then to arch specific CFLAGS already defined in e.g. Makefile.arm-linux
#

CFLAGS += -O$(OPT) -fPIC

CFLAGS += -fno-short-enums
CFLAGS += -Wall -Wcast-qual -Wimplicit -Wcast-align
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused


# flags only for C
CFLAGS += -Wstrict-prototypes -Wmissing-declarations
CFLAGS += -Wmissing-prototypes -Wnested-externs
CFLAGS += $(CSTANDARD)
#CFLAGS += $($(TARGET).CFLAGS)
#CFLAGS += $(USER_CFLAGS)


#LDFLAGS	+= $($(TARGET).LDFLAGS) -lm -pthread
LDFLAGS	+=  -lm -pthread

#cdc defined flags
BOARD_CFG=\"boards/ardrone2.h\"
## include dir
CFLAGS += -I./ -I./arch/linux -I./include -I./varinclude -I./modules \
		-I./mcu_periph -I./boards/ardrone -I./firmwares/rotorcraft/
## MACROS
CFLAGS += -DBOARD_CONFIG=$(BOARD_CFG) -DRADIO_CONTROL \
			-DSTABILIZATION_ATTITUDE_TYPE_INT -DPERIPHERALS_AUTO_INIT \
			-DUSE_SONAR -DUSE_VFF_EXTENDED  \
			 -DFAILSAFE_GROUND_DETECT
CFLAGS += -DPERIODIC_FREQUENCY=512


#IMU and AHRS
CFLAGS += -DUSE_IMU -DUSE_AHRS -DUSE_AHRS_ALIGNER -DUSE_MAGNETOMETER
#USE_MAGNETOMETER  # this macro seems not so important
# Set the AHRS propegation frequencies
AHRS_PROPAGATE_FREQUENCY ?= 200
AHRS_CORRECT_FREQUENCY ?= 200
ifdef AHRS_PROPAGATE_FREQUENCY
CFLAGS += -DAHRS_PROPAGATE_FREQUENCY=$(AHRS_PROPAGATE_FREQUENCY)
endif
ifdef AHRS_CORRECT_FREQUENCY
CFLAGS += -DAHRS_CORRECT_FREQUENCY=$(AHRS_CORRECT_FREQUENCY)
endif



# seems that we need to link against librt for glibc < 2.17
LDFLAGS += -lrt

#
# Electrical subsystem / Analog Backend
#
CFLAGS += -DUSE_ADC

# handle linux signals by hand
CFLAGS += -DUSE_LINUX_SIGNAL -D_GNU_SOURCE
# Link static (Done for GLIBC)
CFLAGS += -DLINUX_LINK_STATIC
LDFLAGS += -static


EXE = apAr653 \
	  clean

all:	apAr653

apAr653:	boards/ardrone/board.o        \
			mcu.o        \
			arch/linux/mcu_arch.o        \
			mcu_periph/sys_time.o        \
			arch/linux/mcu_periph/sys_time_arch.o        \
			math/pprz_geodetic_int.o        \
			math/pprz_geodetic_float.o        \
			math/pprz_geodetic_double.o        \
			math/pprz_trig_int.o        \
			math/pprz_orientation_conversion.o        \
			math/pprz_algebra_int.o        \
			math/pprz_algebra_float.o        \
			math/pprz_algebra_double.o        \
			subsystems/settings.o        \
			arch/linux/subsystems/settings_arch.o        \
			subsystems/actuators.o        \
			state.o        \
			subsystems/commands.o        \
			boards/ardrone/baro_board.o        \
			firmwares/rotorcraft/stabilization.o        \
			firmwares/rotorcraft/stabilization/stabilization_none.o        \
			firmwares/rotorcraft/guidance/guidance_h.o        \
			firmwares/rotorcraft/guidance/guidance_h_ref.o        \
			firmwares/rotorcraft/guidance/guidance_v.o        \
			firmwares/rotorcraft/guidance/guidance_v_ref.o        \
			firmwares/rotorcraft/guidance/guidance_v_adapt.o        \
			firmwares/rotorcraft/guidance/guidance_flip.o        \
			firmwares/rotorcraft/navigation.o        \
			subsystems/navigation/waypoints.o        \
			subsystems/navigation/common_flight_plan.o        \
			subsystems/navigation/traffic_info.o        \
			firmwares/rotorcraft/main.o        \
			firmwares/rotorcraft/autopilot.o        \
			mcu_periph/i2c.o        \
			arch/linux/mcu_periph/i2c_arch.o        \
			mcu_periph/uart.o        \
			arch/linux/mcu_periph/uart_arch.o        \
			arch/linux/serial_port.o        \
			arch/linux/mcu_periph/adc_arch.o        \
			subsystems/electrical.o        \
			boards/ardrone/gpio_ardrone.o        \
			modules/sensors/bat_voltage_ardrone2.o        \
			modules/calibration/send_imu_mag_current.o        \
			modules/air_data/air_data.o        \
			modules/geo_mag/geo_mag.o        \
			math/pprz_geodetic_wmm2015.o        \
			mcu_periph/udp.o        \
			arch/linux/mcu_periph/udp_arch.o        \
			arch/linux/udp_socket.o        \
			firmwares/rotorcraft/rotorcraft_telemetry.o        \
			subsystems/radio_control.o        \
			subsystems/radio_control/rc_datalink.o        \
			subsystems/actuators/motor_mixing.o        \
			boards/ardrone/actuators.o        \
			subsystems/imu.o        \
			subsystems/imu/imu_ardrone2.o        \
			boards/ardrone/navdata.o        \
			firmwares/rotorcraft/stabilization/stabilization_attitude_ref_quat_int.o        \
			firmwares/rotorcraft/stabilization/stabilization_attitude_quat_int.o        \
			firmwares/rotorcraft/stabilization/stabilization_attitude_quat_transformations.o        \
			firmwares/rotorcraft/stabilization/stabilization_attitude_rc_setpoint.o        \
			subsystems/ahrs.o        \
			subsystems/ahrs/ahrs_int_cmpl_quat.o        \
			subsystems/ahrs/ahrs_int_cmpl_quat_wrapper.o        \
			subsystems/ahrs/ahrs_aligner.o        \
			subsystems/ins.o        \
			subsystems/ins/ins_int.o        \
			subsystems/ins/vf_extended_float.o    \
			firmwares/rotorcraft/Com_AR653Msg.o    \
			../../../../build/libApexArinc653/CBasefunction.o   \
			../../../../build/libApexArinc653/CArgument.o       \
			../../../../build/libApexArinc653/CQueuing.o        \
			../../../../build/libApexArinc653/CSampling.o       \
			../../../../build/libApexArinc653/Vector.o
	$(CC)  $(CFLAGS) -o $@.out $^ $(LDFLAGS)


.PHONY: depend clean realclean all tests

depend:
	makedepend -Y -r *.c

clean:
	 $(RM) *.elf
	-$(RM) *.o
	-$(RM) */*.o
	-$(RM) */*/*.o
	-$(RM) */*/*/*.o
	-$(RM) *.out
	-$(RM) *.bak core Makefile.bak

realclean: clean

# DO NOT DELETE

#main.o: mcu.h
# log.o: log.h
# state.o: state.h math/pprz_algebra_int.h std.h math/pprz_algebra.h
# state.o: math/pprz_trig_int.h math/pprz_algebra_int.h
# state.o: math/pprz_algebra_float.h math/pprz_geodetic_int.h
# state.o: math/pprz_geodetic.h math/pprz_geodetic_float.h
# state.o: math/pprz_algebra_float.h math/pprz_orientation_conversion.h std.h
# sys_time_arch.o: sys_time.h std.h sys_time_arch.h
# sys_time.o: sys_time.h std.h sys_time_arch.h
# #stabilization_attitude_quat_int.o: stabilization_attitude_quat_int.h
# communication_test_P1.o: sys_time.h std.h sys_time_arch.h log.h state.h
# communication_test_P1.o: math/pprz_algebra_int.h std.h math/pprz_algebra.h
# communication_test_P1.o: math/pprz_trig_int.h math/pprz_algebra_int.h
# communication_test_P1.o: math/pprz_algebra_float.h math/pprz_geodetic_int.h
# communication_test_P1.o: math/pprz_geodetic.h math/pprz_geodetic_float.h
# communication_test_P1.o: math/pprz_algebra_float.h math/pprz_orientation_conversion.h
# communication_test_P1.o: commands.h conf.h capteur/baro_board.h capteur/navdata.h
# communication_test_P1.o: capteur/imu.h math/pprz_algebra_int.h
# communication_test_P1.o: math/pprz_algebra_float.h capteur/conf_capteur.h
# communication_test_P1.o: capteur/ahrs.h state.h capteur/ahrs/ahrs_int_cmpl_quat.h
# communication_test_P1.o: capteur/ahrs.h std.h math/pprz_algebra_int.h
# communication_test_P1.o: capteur/ahrs/ahrs_aligner.h capteur/ins.h
# communication_test_P1.o: math/pprz_geodetic_int.h capteur/ins_int.h capteur/ins.h
# communication_test_P1.o: stabilization/stabilization_attitude_quat_int.h
# communication_test_P1.o: stabilization/stabilization_attitude_common_int.h
# communication_test_P1.o: stabilization/conf_asser.h
# communication_test_P1.o: stabilization/stabilization_attitude_ref_quat_int.h
# communication_test_P1.o: stabilization/stabilization_attitude_ref_int.h
# communication_test_P1.o: stabilization/guidance_v.h stabilization/guidance_v_ref.h
# communication_test_P1.o: math/pprz_algebra.h stabilization/guidance_v_adpt.h
# communication_test_P1.o: moteur/actuators.h moteur/conf_moteur.h
#communication_test_P1.o: ../../../../include/libApexArinc653/CBasefunction.h
