addpath ../../include/;
addpath ../../source/;

%% Input variables --------------------------------------------------------
frequency = 2.4e9;
antenna = UCA(frequency);

disp(noiseFloor(295, frequency));

%% Show antenna properties as figures -------------------------------------
figure("Name", "Physical Design")
viewArray(antenna, 'ShowNormal', true); 

figure("Name", "Exciter Design")
show(antenna.Exciter); 

% %figure("Name", "Emission Pattern")
% %pattern(antenna, frequency);
% 
% figure("Name", "Azimuth Emission Pattern")
% patternAzimuth(antenna, frequency); 
% 
% %figure("Name", "Elevation Emission Pattern")
% %patternElevation(antenna, frequency); 

% Show power spectrum on map ----------------------------------------------
tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 37.338380, ...
            "Longitude", -115.924490, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 5, ...
            "TransmitterFrequency", 2.4e9);
show(tx)

coverage(tx, ...
        "SignalStrengths", -120:-5, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "Transparency", 0.6);

rmpath ../../include/;

rmpath ../../source/;