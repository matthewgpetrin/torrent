addpath ../../include/;
addpath ../../source/;

% Input variables ---------------------------------------------------------
frequency = 2.4e9;
antenna = UCA(2.4e9, 190, 9);
    
% Define transmitter site  ------------------------------------------------
tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 37.338380, ...
            "Longitude", -115.924490, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 0.1, ...
            "TransmitterFrequency", frequency);

% Define coverage data ----------------------------------------------------

coverageData = coverage(tx, ...
        "SignalStrengths", noiseFloor(295, frequency), ...
        "Transparency", 0.6, ...
        "MaxRange", 1000);

% Display data on map -----------------------------------------------------
show(tx);
pattern(tx);
plot(coverageData);

% Convert to table and write to txt file ----------------------------------
table(coverageData.Data);
writetable(coverageData.Data, 'coverage.txt');

addpath ../../source/;
rmpath ../../include/;