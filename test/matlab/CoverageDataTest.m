addpath ../../include/;
addpath ../../source/;

clearMap(siteviewer);

% Input variables ---------------------------------------------------------
frequency = 2.4e9;
antenna = YagiAntenna;
    
% Define transmitter site  ------------------------------------------------
tx = txsite("Name","Transmitter", ...
            "Antenna", antenna, ...
            "Latitude", 37.338380, ...
            "Longitude", -115.924490, ...
            "AntennaHeight", 1, ...
            "TransmitterPower", 0.1, ...
            "TransmitterFrequency", frequency);

% Define coverage data ----------------------------------------------------

coverageData = coverage(tx1, ...
        "SignalStrengths", -60, ...
        "Transparency", 0.6, ...
        "MaxRange", 500);

% Display data on map -----------------------------------------------------
show(tx);
pattern(tx);
plot(coverageData);

% Convert to table and write to txt file ----------------------------------
table(coverageData.Data);
writetable(coverageData.Data, 'coverage.txt');

rmpath ../include/;