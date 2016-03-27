#Change Log:

##2016.3.27
change DronePartitions usecase to takeoff and land and communicate with other partitions.

##2016.3.26
use paparazzi code as partition 1;
change makefile

##2016.3.24
takeoffTestGood commit
altSetPt = 12.5*(1<<6);   a bit high
altSetPt = 12*(1<<6);  too low
Todo:
Control the altitude for takeoff using ultrasound sensor
Control altitude with commands
maybe change control methods 

##2016.3.21
test hold position


##2016.3.17
add Usecase Test_Drone


##2016.3.16
fix the undefined symbol problem, the use case queueing communication can work in the drone now.



ARINC653 Standard kernel Simulator for ARDrone
==================
Copied from https://github.com/lwenisch/ardrone;
Project of Ilyasse TOUMI, Laure WENISCH, Jie YANG at 2015.

An ARINC653 kernel simulator implementing the Time and Space Partitioning concept (TSP) has been developed on Linux.

This version has been adapted to work on a Parrot ARDrone 2.0. 

This source code is distributed under MIT and CeCILL-B licences, available at http://opensource.org/licenses/MIT and http://www.cecill.info/licences/ respectively.

You can find the original source code of the ARINC 653 simulator at https://github.com/makrin/ARISS

This project was initially developed at LAAS-CNRS ( https://www.laas.fr/public/en/tsf) under the supervision of Prof. Jean-Charles Fabre (INPT-ENSEEIHT) as a tool for education and research. The first version was developed by Gabriel BUSTAMENTE, Rémi PALUSTRAN and Martin CRONEL, former students of INPT-ENSEEIHT dept GEA. The current version has been developed and is maintained by Martin CRONEL.

You can find details on the software architecture and on its use in the ReadMe directory.
