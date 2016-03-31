# Summary
This project is based on paparazzi code, It's modified to be run in AR.Drone2 with an Arinc653 simulator.

## TODO List  (for future students)
### Need to solve the navdata error
when we run apAr653.out in drone (without simulator), there is error '[navdata] sync error, startbyte not found, resetting...', This error may came from the compiler settings in the makefile. We changed the compiler from arm-linux-gnueabi-gcc to arm-linux-gnueabi-g++ in order to have compatibility with the ARINC653 simulator, maybe we could use arm-linux-gnueabi-gcc as compiler, and use arm-linux-gnueabi-g++ as LD tool. or change the compiler and test this program alone without linking any simulator libraries(comment the lines related to libApexArinc653 in the makefile).
### Add new flight modes to have horizontal control


## ChangeLog

### 2016.3.27
change makefile;
add communication feature, but need to change the macro in Com_AR653Msg.h;
change flight_plan.h to make a takeoff and land flight plan.

### 2016.3.26
change makefile to fix the GLIBC_2.17 not found error
can fly now

add Arinc653 simulator library, can communicate now

### GLIBC_2.17 not found  2016.3.25
change flight plan
add navdata_height() function

### init
This project is based on the paparazzi repository
commited at 2016-02-28 00:02:50; SHA1 ID =  eba0a75a3a0c21cdcd5cbf18a19f346820407345
