addpath ../include/;

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

coverageData = coverage(tx, ...
        "SignalStrengths", -60, ...
        "Transparency", 0.6, ...
        "MaxRange", 500);

% Display data on map -----------------------------------------------------
show(tx);
plot(coverageData);
pattern(tx);


rmpath ../include/;