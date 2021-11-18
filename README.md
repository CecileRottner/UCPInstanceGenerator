# Instance generator for the UCP

## Instance names

UCP instances are generated under a name of type 
```bash
n_T_1_demand_sym_bin_intra_id.txt
```

where :
- n : number of production units
- T : number of time periods
- demand : type of production demand (3 means the classical form "two peak per day")
- sym : level of symmetry in the instance (0 means no symmetry, and x means there are in average n/x identical units)
- bin : equals 1 if instances are such that Pmin=Pmax for all production units (production levels are then binary), equals 0 otherwise
- intra : equals 1 if instances are subject to "intra site" constraints (ie units are grouped on geographical sites and there is at most one start up per time period on each site)
- id : unique identifier of the instance


## Instance format

### General data

* **n**: number of units
* **T**: number of time periods
* **D**: power demand at each time step

* **K**: number of unit type (= number of different units, useful for symmetrical instances)
* **ns**: number of sites


### Units data

* **Init**: initial status of each unit (up = 1 or down = 0)
* **L**: minimum up time of each unit
* **l**: minimum down time of each unit
* **Pmin**: minimum power output for each up unit
* **Pmax**: maximum power output for each up unit
* **cf**: fixed cost of each unit
* **c0**: start-up cost of each unit
* **cp**: proportional cost of each unit
* **nk**: type of each unit (ie: if 2 units have the same type, then they are identical)
* **S**: site index of each unit (ie: nk[i] is the index of the site of unit i)

