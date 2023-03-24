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

addpath raytracer/utils/;

%% Input variables ---------------------------------------------------------
latitude = 40.745589;
longitude = -74.024837;
elevation = 2;

power = 1;
frequency = 2.4e9;

nElements = 8;

reflections= 0;

terrainMaterial = "perfect-reflector";
buildingMaterial = "perfect-reflector";

nCPUs = 4;
nAngs = 4;

tic;

%% Define parameters ------------------------------------------------------
nAngPerCPU = nAngs/ nCPUs;
floor = noiseFloor(295, frequency);

%% Create angles Array -----------------------------------------------------
angles = cell(nCPUs, nAngPerCPU);

q = 0;
for n = 1:nCPUs
    for m = 1:nAngPerCPU
        angle = 360 / nAngs * (q);
        angles{n, m} = angle;
        q = q + 1;
    end
    disp("created angle group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngs + " angles for " + nCPUs + " cores")

disp(angles);

%% Create antenna array ---------------------------------------------------
antennas = cell(nCPUs, nAngPerCPU);

for n = 1: nCPUs
    for m = 1: nAngPerCPU
        antenna = UCA(frequency, angles{n,m}, nElements);
        antennas{n,m} = antenna;
    end
    disp("created antenna group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngs + " antennas for " + nCPUs + " cores")

%% Create tx sites array ---------------------------------------------------
txs = cell(nCPUs, nAngPerCPU);

for n = 1:nCPUs
    for m = 1:nAngPerCPU
        txs{n,m} = txsite(...
            "Antenna", antennas{n,m}, ...
            "Latitude", latitude, ...
            "Longitude", longitude, ...
            "AntennaHeight", elevation, ...
            "TransmitterFrequency", frequency, ...
            "TransmitterPower", power);
    end
    disp("created txsite group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngs + " txs for " + nCPUs + " cores")

%% Reflected coverage macro for cellfun ------------------------------------
rCoverage = @(y) bouncedCoverage(y, ...
        "Accuracy", "high", ...
        "Reflections", reflections, ...
        "TerrainMaterial", terrainMaterial, ...
        "BuildingMaterial", buildingMaterial);

%% Create coverages array -------------------------------------------------
coverages = cell(nCPUs, nAngPerCPU);

parfor n = 1:nCPUs
    coverages(n,:) = cellfun(rCoverage, txs(n,:), 'uniformoutput',false);
    disp("created coverage group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngPerCPU * nCPUs + " coverages for " + nCPUs + " cores")

%% Create array of scores -------------------------------------------------
scores = zeros(nCPUs, nAngPerCPU);

for n = 1:nCPUs
    for m = 1:nAngPerCPU
        score = transmissionScore(coverages{n,m}, floor);
        scores(n,m) = score;
    end
    disp("created scores group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngPerCPU * nCPUs + " scores for " + nCPUs + " cores")

%% Find best score and its index ------------------------------------------
[~,i] = max(scores(:));

[i_row, i_col] = ind2sub(size(scores),i);

idx = [i_row, i_col];

disp("optimal transmission angle of " + angles(idx));

toc;

rmpath raytracer/utils/;