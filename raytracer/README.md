# Torrent Raytracer:
## Overview:
This directory contains MATLAB code intended to read a .osm file and output a table of ideal transmission angles corresponding to different latitude and longitude coordinates within the .osm's range. This data may be referenced by the system at a later date.

Ideal transmission angles are determined by running MATLAB raytraces at different angles in parallel at one location and selecting the run with the greatest coverage. Similarly, angle determinations are run in parallel at evenly spaced points within a .osm file's range.

Note that individual raytraces are computationally taxing, and MATLAB is incapable of running them on the GPU. Thus a supercomputer is required to practically use this subsystem. 

## Structure:
| Directory   | Contains                                              |
| :-----------|:------------------------------------------------------|
| **/data/**  | Program output tables, figures, and files             |
| **/maps/**  | Maps to be used by the program                        | 
| **/src/**   | Project critical and specialized functions and scripts| 
| **/tests/** | Tests of currently relevant files and functions       |
| **/trash/** | Old code for reference (may or may not work)          | 
| **/utils/** | Frequently-used general-use functions                 |  
  
## Functions:
**dBmToWatts(dBmMatrix)**  
Accepts a matrix of values and returns their dBm equivalent.  
  
**wattsTodBm(wattsMatrix)**  
Accepts a matrix of values and returns their Watt equivalent.  
 
**noiseFloor(temperature, bandwidth)**  
Accepts a temperature in K and a bandwidth in Hz and returns the corresponding noise floor in dBm.  

**UCA(frequency, angle, nElements)**  
Accepts frequency in Hz, an azimuth angle in degrees, and a number of Elements and returns a circularArray object with nElements elements from the Antenna Toolbox.   

The Antenna Toolbox does not accept broadcast angles as an input, so this function constructs a UCA object from the Phased Array Toolbox, calculates the required delays, and then constructs an Antenna Toolbox object, which is generally more useful.  
