ARINC 653 simulator improvements to control AR.Drone2
==================
This is the Integrated Team Project of AM EMS2015/2016 in ISAE Supaero and ENSEEIHT, supervised by Jerome Ermont and Jean-Luc Scharbarg, developped by Charlotte SUCHET, Benjamin GAILLARD, Romain SERVIERES and CAI Dongcai.  

Finally we developped two solutions to control the drone.  
In branch solution1, the partitions are in directory sources/UseCases/Test_Drone/, f1 is based on the work of Nicolas KNIEBIHLI.  
In branch master, the partitions are in directory sources/UseCases/DronePartitions/, f1 is based on paparazzi source code.  

Based on the following projects:  
https://github.com/lwenisch/ardrone  Project of Ilyasse TOUMI, Laure WENISCH, Jie YANG at 2015;  
https://github.com/makrin/ARISS  ARINC653 kernel simulator;  
https://github.com/paparazzi/paparazzi  Paparazzi UAV Project


# Change Log:
## 2016.3.28
Commit TestFailed  
Sometimes when we run apAr653.out in drone, there is error '[navdata] sync error, startbyte not found, resetting...' the drone is out of control in this condition, but it's quite stable if there is no error.  
And the function  navdata_height() I wrote in navdata.c seems not good because sometimes the height was not updated, which will cause severe problems, maybe we should delete the if condition in this function.
## 2016.3.27
change DronePartitions usecase to takeoff and land and communicate with other partitions.
## 2016.3.26
use paparazzi code as partition 1;
change makefile
## 2016.3.24
takeoffTestGood commit  
altSetPt = 12.5*(1<<6);   a bit high  
altSetPt = 12*(1<<6);  too low  
Todo:  
Control the altitude for takeoff using ultrasound sensor;
Control altitude with commands;
maybe change control methods;
## 2016.3.21
test hold position
## 2016.3.17
add Usecase Test_Drone
## 2016.3.16
fix the undefined symbol problem, the use case queueing communication can work in the drone now.
