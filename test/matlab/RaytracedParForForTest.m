% This file calculates different coverage data for the same antenna angled
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

terrainMaterial = "perfect-reflector";
buildingMaterial = "perfect-reflector";


numAngles = 12;
numCores = 4;

% Define propagation model using raytracing -------------------------------
prop = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", reflections, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

% Loop simulation ---------------------------------------------------------
tic;

% Creates anonymous function for coverages. This decreases verbosity when
% using cellfun with functions with multiple inputs. It is basically a
% macro
wrapCoverages = @(y) coverage(y, prop, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);    

% Determines how many angles will be run on each core
numAnglesPerCore = numAngles / numCores;

% Define cell arrays with #core columns. The angles to be processed by
% each core are found in the rows.
txs = cell(numCores, numAnglesPerCore);
angles = cell(numCores, numAnglesPerCore);
coverages = cell(numCores, numAnglesPerCore);


q = 0;
% Populates the angles cell array with angles
for o = 1:numCores
    for p = 1:numAnglesPerCore
        angle = 360 / numAngles * (q);
        angles{o, p} = angle;
        q = q + 1;
    end
end

disp(angles);

% Loop populates angle array with angles
for n = 1:numCores
    for m = 1:numAnglesPerCore
        txs{n,m} = txsite(...
            "Antenna", antenna, ...
            "AntennaAngle", angles{n,m}, ...
            "AntennaHeight", elevation, ...
            "Latitude", latitude, ...
            "Longitude", longitude, ...
            "Name", "Transmitter", ...
            "TransmitterFrequency", frequency, ...
            "TransmitterPower", power);
        disp("created tx object group");
    end
end

% Parallel for loop in which each core is assigned an array of txsite
% objects and populates the coverages cell array with propagationData
% objects
parfor m = 1:numCores
    for n = 1:numAnglesPerCore
        coverage(txs{m, n}, prop, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);    
        disp("created coverage object");
    end
    disp("created coverage object group");
end

toc;

% View coverage data ------------------------------------------------------
siteviewer("Buildings","stevens.osm","Basemap","topographic");
plot(coverages{1,1});

rmpath ../../source/;
rmpath ../../include/;
