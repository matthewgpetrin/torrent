addpath ../include/;


% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;

% Show antenna properties as figures --------------------------------------
figure("Name", "Physical Design")
show(antenna); 

figure("Name", "Exciter Design")
show(antenna.Exciter); 

figure("Name", "Emission Pattern")
pattern(antenna, frequency);

figure("Name", "Azimuth Emission Pattern")
patternAzimuth(antenna, frequency); 

figure("Name", "Elevation Emission Pattern")
patternElevation(antenna, frequency); 

% Show power spectrum on map ----------------------------------------------
tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 40.745589, ...
            "Longitude", -74.024837, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 5, ...
            "TransmitterFrequency", 2.4e9);
show(tx)

coverage(tx, ...
        "SignalStrengths", -120:-5, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "Transparency", 0.6);

rmpath ../include/;