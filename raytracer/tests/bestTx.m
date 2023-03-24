% This file calculates different coverage data for the same antenna angled
% in different directions at the same location. Its speed should be 
% compared to VectorizedTest in order to determine which solution will be 
% more efficient. Note that the comparative speed of the files will likely 
% change on different machines and for different nAngs values because 
% any given computer can only run these loops on as many cores as it has.
%
% Alter nAngs to increase or decrease how many angles are compared.
% 
% Note that the code may return an error when attempting to plot the data. 
% I don't know why this is happening. The code returns properly formatted 
% data, but refuses to plot it.

addpath raytracer/src/;
addpath raytracer/utils/;

%% Input variables ---------------------------------------------------------
latitude = 40.745589;
longitude = -74.024837;
elevation = 2;

power = 1;
frequency = 2.4e9;

nElements = 8;

reflections= 1;

terrainMaterial = "perfect-reflector";
buildingMaterial = "perfect-reflector";

nCPUs = 4;
nAngs = 4;

tic;

%% Define parameters ------------------------------------------------------
nAngPerCpu = nAngs/ nCPUs;
floor = noiseFloor(295, frequency);

%% Create angles Array -----------------------------------------------------
angles = cell(nCPUs, nAngPerCpu);

q = 0;
for n = 1:nCPUs
    for m = 1:nAngPerCpu
        angle = 360 / nAngs * (q);
        angles{n, m} = angle;
        q = q + 1;
    end
    disp("created angle group of size " + nAngPerCpu + " for core " + n);
end
disp("created parallel array of " + nAngs + " angles for " + nCPUs + " cores")

disp(angles);

%% Create antenna array ---------------------------------------------------
antennas = cell(nCPUs, nAngPerCpu);

for n = 1: nCPUs
    for m = 1: nAngPerCpu
        antenna = UCA(frequency, angles{n,m}, nElements);
        antennas{n,m} = antenna;
    end
    disp("created antenna group of size " + nAngPerCpu + " for core " + n);
end
disp("created parallel array of " + nAngs + " antennas for " + nCPUs + " cores")

%% Create tx sites array ---------------------------------------------------
txs = cell(nCPUs, nAngPerCpu);

for n = 1:nCPUs
    for m = 1:nAngPerCpu
        txs{n,m} = txsite(...
            "Antenna", antennas{n,m}, ...
            "Latitude", latitude, ...
            "Longitude", longitude, ...
            "AntennaHeight", elevation, ...
            "TransmitterFrequency", frequency, ...
            "TransmitterPower", power);
    end
    disp("created txsite group of size " + nAngPerCpu + " for core " + n);
end
disp("created parallel array of " + nAngs + " txs for " + nCPUs + " cores")

%% Find best angle --------------------------------------------------------
index = idxBestTx(txs, floor, ...
    Reflections = reflections, ...
    TerrainMaterial = terrainMaterial, ...
    BuildingMaterial = buildingMaterial);

disp("optimal transmission angle of " + angles(index));

toc;

rmpath raytracer/utils/;
rmpath raytracer/src/;