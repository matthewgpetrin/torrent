%This file calculates different coverage data for the same antenna angled
% in different directions at the same location. It is to be compared to
% ParForTest to test for speed improvements when using parfor.
% 
% Alter numAngles to increase or decrease how many angles are compared.
% 
% Note that the code may return an error when attempting to plot the data. 
% I don't know why this is happening. The code returns properly formatted 
% data, but refuses to plot it.

addpath ../torrent/raytracer/src/;
addpath ../torrent/raytracer/utils/;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;
power = 0.1;

latitude = 37.338380;
longitude = -115.924490;

elevation = 1;

reflections = 1;

terrainMaterial = "concrete";
buildingMaterial = "concrete";

numAngles = 4;

% Loop simulation ---------------------------------------------------------
tic;
    
txs = cell(1, numAngles);
coverages = cell(1, numAngles);

for n = 1:numAngles
    angle = 360 / numAngles * (n - 1);
    txs{1, n} = txsite(...
        "Antenna", antenna, ...
        "AntennaAngle", angle, ...
        "AntennaHeight", elevation, ...
        "Latitude", latitude, ...
        "Longitude", longitude, ...
        "Name", "Transmitter", ...
        "TransmitterFrequency", frequency, ...
        "TransmitterPower", power);
    disp("txsite object created");
end

floor = noiseFloor(295, frequency);

for m = 1:nAngles
    coverages{1, m} = coverage(txs{1, m}, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);    
    disp("coverage object created");
end

toc;

% View coverage data ------------------------------------------------------
% siteviewer("Basemap","topographic");
% show(txs{1, 1});
% plot(coverages1{1, 1});

rmpath ../torrent/raytracer/utils/;
rmpath ../torrent/raytracer/src/;
