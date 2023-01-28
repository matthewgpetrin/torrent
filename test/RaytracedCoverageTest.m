addpath ../include/;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;


antenna.TiltAxis = 'X';
antenna.Tilt = 0;

reflections = 2;
terrainMaterial = "concrete";
buildingMaterial = "concrete";

viewer = siteviewer("Buildings","stevens.osm","Basemap","topographic");

tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 40.745589, ...
            "Longitude", -74.024837, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 5, ...
            "TransmitterFrequency", 2.4e9);
show(tx)

rt = propagationModel("raytracing", ...
                        "Method", "sbr", ...
                        "MaxNumReflections", reflections, ...
                        "BuildingsMaterial",buildingMaterial, ...
                        "TerrainMaterial",terrainMaterial);

coverage(tx, rt, ...
        "SignalStrengths", -120:-5, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "Transparency", 0.6);