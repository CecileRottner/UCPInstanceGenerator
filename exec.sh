#!/bin/bash
n=10
T=24
bloc=1 #instance type
demand=3 #demand type. 3 --> 2-peaks per day demand
sym=5 #level of symmetry. 0--> no symmetry ; n--> all units are identical ; otherwise, generates n/sym units identical in average
binary=1 #Pmin=Pmax
intra=1 #generates sites
./main $n $T $bloc $demand $sym $binary $intra 