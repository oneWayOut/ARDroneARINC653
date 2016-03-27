#Summary
This project is based on paparazzi code, It's modified to be run in AR.Drone2 with an Arinc653 simulator.
from this repository: https://github.com/oneWayOut/ArDronePPZ

##Todo:
1 improve the altitude control
2 implement better communication 

##ChangeLog
###2016.3.27
change makefile;
add communication feature, but need to change the macro in Com_AR653Msg.h;
change flight_plan.h to make a takeoff and land flight plan.

###2016.3.26
change makefile to fix the GLIBC_2.17 not found error
can fly now

add Arinc653 simulator library, can communicate now

###GLIBC_2.17 not found  2016.3.25
change flight plan
add navdata_height() function

### init
This project is based on the paparazzi repository
commited at 2016-02-28 00:02:50; SHA1 ID =  eba0a75a3a0c21cdcd5cbf18a19f346820407345
