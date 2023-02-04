addpath ../include/;
addpath ../source/;

tic;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;

reflections = 1;
terrainMaterial = "concrete";
buildingMaterial = "concrete";

viewer = siteviewer("Buildings","stevens.osm","Basemap","topographic");

% Define transmission site ------------------------------------------------
tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 40.745589, ...
            "Longitude", -74.024837, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 0.1, ...
            "TransmitterFrequency", 2.4e9);

% Define ray tracing models, one with reflections and one without ---------
rt1 = propagationModel("raytracing", ...
                        "Method", "sbr", ...
                        "MaxNumReflections", 0, ...
                        "BuildingsMaterial",buildingMaterial, ...
                        "TerrainMaterial",terrainMaterial);

rt2 = propagationModel("raytracing", ...
                        "Method", "sbr", ...
                        "MaxNumReflections", reflections, ...
                        "BuildingsMaterial",buildingMaterial, ...
                        "TerrainMaterial",terrainMaterial);

% Compute coverage data for both models -----------------------------------
coverageData1 = coverage(tx, rt1, ...
        "SignalStrengths", -120:-5, ...
        "Transparency", 0.6, ...
        "Resolution", 3, ...
        "MaxRange", 500);

disp('Non-reflected coverage data computed');
toc;

coverageData2 = coverage(tx, rt2, ...
        "SignalStrengths", -120:-5, ...
        "Transparency", 0.6, ...
        "Resolution", 3, ...
        "MaxRange", 500);

disp('Reflected coverage data computed');
toc;

% Create tables containing coverage data ----------------------------------
T1 = coverageData1.Data;
T2 = coverageData2.Data;
T = T2;

% Convert powers from dBm to Watts ----------------------------------------
T1.Power = dBmToWatts(T1.Power);
T2.Power = dBmToWatts(T2.Power);

% Subtract powers and convert back to dBm ---------------------------------
T.Power = WattsTodBm(T2.Power - T1.Power);

% Limit to real #s. Not sure if this has any downsides.
% Required as of 2022b because destructive interference can return
% negative W which gives imaginary dBms which coverage doesn't like
T.Power = real(T.Power);

writetable(T, 'coverage.txt');

plot(propagationData(T));

disp('Coverage data adjusted');
toc;

rmpath ../source/;
rmpath ../include/;