addpath ../include/;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;

reflections = 2;
terrainMaterial = "concrete";
buildingMaterial = "concrete";

viewer = siteviewer("Buildings","stevens.osm","Basemap","topographic");

tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 40.745589, ...
            "Longitude", -74.024837, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 0.1, ...
            "TransmitterFrequency", 2.4e9);

rt = propagationModel("raytracing", ...
                        "Method", "sbr", ...
                        "MaxNumReflections", reflections, ...
                        "BuildingsMaterial",buildingMaterial, ...
                        "TerrainMaterial",terrainMaterial);

coverageData = coverage(tx, ...
        "SignalStrengths", -120:-5, ...
        "Transparency", 0.6, ...
        "MaxRange", 500);

coverageDataRaytraced = coverage(tx, rt, ...
        "SignalStrengths", -120:-5, ...
        "Transparency", 0.6, ...
        "MaxRange", 500);

show(tx);

plot(coverageDataRaytraced - coverageData);

