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

tic;

tx = txsite(...
        "Antenna", antenna, ...
        "AntennaAngle", angle, ...
        "AntennaHeight", elevation, ...
        "Latitude", latitude, ...
        "Longitude", longitude, ...
        "Name", "Transmitter", ...
        "TransmitterFrequency", frequency, ...
        "TransmitterPower", power);

prop = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", reflections, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

cv = coverage(tx, prop, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);   

toc;

siteviewer("Buildings","stevens.osm","Basemap","topographic");
plot(cv);