# IoT-Passive-Shunt-Balancer
This repository is designed to monitor battery cells voltages and to balance them by selective charging and emptying.

The project is made using 4 lithium ion battery cells arranged in series to imitate a 16 Volts battery pack
The System uses 4 current sensors and 4 voltage sensors

The main algorithm for this project is to lower all the battery voltages to it's empty state by shunting it through some resistors, and then charge them together 
to it's full state.

The purpose is to achieve a balanced final voltage of every battery cells and to reach the full stage at an equal time for each cells.
