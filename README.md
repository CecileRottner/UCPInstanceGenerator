# Instance generator for the UCP

## Instance names

UCP instances are generated under a name of type 
```bash
n_T_1_demand_sym_bin_intra_id.txt
```

where :
- n (int) : number of production units
- T (int) : number of time periods
- demand (int) : type of production demand (3 means the classical form "two peak per day")
- sym (int) : level of symmetry in the instance (0 means no symmetry, and x means there are in average n/x identical units)
- bin (bool) : equals 1 if instances are such that Pmin=Pmax for all production units (production levels are then binary), equals 0 otherwise
- intra (bool) : equals 1 if instances are subject to "intra site" constraints (ie units are grouped on geographical sites and there is at most one start up per time period on each site)
- id (int) : unique identifier of the instance


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




# Code description

## Overview

This code generates instances for the Unit Commitment Problem (UCP), producing data files that describe power generation units, their operational constraints, and demand profiles over a time horizon. The generator is configurable, supporting different demand types, unit symmetries, cost correlations, and more.

## Main Components

### 1. Main Program (`main.cpp`)

Entry point for generating UCP instances. Handles parameter parsing, random instance generation, and output formatting.
- **Key Function:** `generation()`
	- Generates all unit and demand data for a single instance.
	- Writes the instance to a text file with a structured format.
- **Parameters:**
	- `n`: Number of units
	- `T`: Number of time periods (with half-hour time periods)
	- `bloc`: Type of instance (correlation/cost structure) => should be set to 1 to get realistic correlations for the units characteristics, based on literature data for thermal units (coal, gas, etc)
	- `demande_type`: Type of demand profile (seasonal, random, etc.)
	- `symetrie`: Symmetry factor for units
	- `cat`: Category (e.g., binary or continuous units)
	- `intra`: Intra-site grouping flag
- **Output:**
	- Text files with arrays for unit parameters (Pmin, Pmax, costs, etc.), demand, and grouping information.

Function `generation()` relies on file `demand.cpp` to generate a randomized 2-peak per day demand curve, and on file `corr.cpp` to generate the parameters of the production units with realistic correlation between its Pmax, Pmin, costs, etc.

### 2. Demand Generation (`demand.cpp`, `demand.h`)

Generates realistic or random demand profiles for the time horizon.

- **Key Functions:**
	- `setParam()`: Sets demand curve parameters based on the season/type.
	- `demand24()`: Simulates a 24-hour demand profile using the parameters.
	- `setDemand()`: Generates a demand profile for the full time horizon (multiple days).
- **Demand Types:**
	- 0: Winter
	- 1: Mid-season
	- 2: Summer
	- 3: Literature/theoretical
	- 4: Random

### 3. Correlation and Cost Generation (`corr.cpp`, `corr.h`)

Generates correlated random variables for unit parameters (typically costs and power capacities) to reflect realistic dependencies for thermal units.

- **Key Functions:**
	- `Chol()`: Computes Cholesky decomposition for correlation matrices.
	- `genCorrChol()`: Generates correlated sequences using Cholesky factors.
	- `genCorrCoef()`, `correle2a2()`: Generate pairs of correlated sequences.
	- `genFirst()`: Generates initial values for unit parameters.

## Output File Structure
Each generated instance file contains:
- `n`, `T`: Number of units and time periods
- `Init`: Initial on/off states for units
- `L`, `l`: Maximum and minimum up/down times
- `Pmin`, `Pmax`: Minimum and maximum power outputs
- `cf`, `c0`, `cp`: Cost values (fixed, startup, production)
- `D`: Demand profile
- `K`, `nk`: Grouping information for units
- `First`, `Last`: Markers for group boundaries
- `nS`, `S`: Site/group information

## How to Use
1. **Compile:** Use the provided `Makefile` or compile manually (e.g., `g++ main.cpp demand.cpp corr.cpp -o ucpgen`).
2. **Run:**
	 ```sh
	 ./ucpgen n T bloc demande_type symetrie cat intra
	 ```
	 - Example: `./ucpgen 10 48 1 0 5 2 1`
3. **Output:**
	 - Generates 20 instance files per run, each with a unique ID in the filename.



