% % This file calculates different coverage data for the same antenna angled
% in different directions at the same location. Its speed should be 
% compared to VectorizedTest in order to determine which solution will be 
% more efficient. Note that the comparative speed of the files will likely 
% change on different machines and for different numAngles values because 
% any given computer can only run these loops on as many cores as it has.
%
% Alter numAngles to increase or decrease how many angles are compared.
% 
% Note that the code may return an error when attempting to plot the data. 
% I don't know why this is happening. The code returns properly formatted 
% data, but refuses to plot it.

addpath ../../include/;
addpath ../../source/;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;
power = 0.1;

latitude = 40.745589;
longitude = -74.024837;

elevation = 1;

reflections = 1;

terrainMaterial = "concrete";
buildingMaterial = "concrete";

numAngles = 12;
numCores = 4;

prop = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", reflections, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

% Loop simulation ---------------------------------------------------------
tic;

wrapTxs = @(x) txsite(...
        "Antenna", antenna, ...
        "AntennaAngle", x, ...
        "AntennaHeight", elevation, ...
        "Latitude", latitude, ...
        "Longitude", longitude, ...
        "Name", "Transmitter", ...
        "TransmitterFrequency", frequency, ...
        "TransmitterPower", power);

floor = noiseFloor(295, frequency);

wrapCoverages = @(y) coverage(y, prop, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);    
    
numAnglesPerCore = numAngles / numCores;


txs = cell(numCores, numAnglesPerCore);
angles = cell(numCores, numAnglesPerCore);
coverages = cell(numCores, numAnglesPerCore);


q = 0;

for o = 1:numCores
    for p = 1:numAnglesPerCore
        angle = 360 / numAngles * (q);
        disp(angle);
        angles{o, p} = angle;
        q = q + 1;
    end
end

parfor n = 1:numCores
    txs(n,:) = cellfun(wrapTxs, angles(n,:), 'uniformoutput',false);
    disp("created tx object gorup");
end

parfor m = 1:numCores
    coverages(m,:) = cellfun(wrapCoverages, txs(m,:), 'uniformoutput',false);
    disp("created coverage object group");
end

toc;

% View coverage data ------------------------------------------------------
siteviewer("Buildings","stevens.osm","Basemap","topographic");
plot(coverages{1,1});
