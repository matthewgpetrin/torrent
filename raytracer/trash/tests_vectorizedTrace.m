% This file calculates different coverage data for the same antenna angled
% in different directions at the same location. Its speed should be 
% compared to ParForTest in order to determine which solution will be 
% more efficient. Note that the comparative speed of the files will likely 
% change on different machines and for different numAngles values because 
% any given computer can only run these loops on as many cores as it has.
% 
% Alter numAngles to increase or decrease how many angles are compared.
% 
% NOTE: I don't believe I've fully properly vectorized this code since 
% most stuff online sayd vectorization is faster than parfor, BUT says
% parfor is faster than cellfun. Please correct it if possible

addpath ../torrent/raytracer/src/;
addpath ../torrent/raytracer/utils/;

% Input variables ---------------------------------------------------------
antenna = YagiAntenna;
frequency = 2.4e9;
power = 0.1;

latitude = 37.338380;
longitude = -115.924490;

elevation = 1;

reflections = 1;

terrainMaterial = "concrete";
buildingMaterial = "concrete";

numAngles = 4;

% Anonymous functions to decrease cellfun verbosity -----------------------
tic; 

wrapTxs = @(x) txsite(...
        "Antenna", antenna, ...
        "AntennaAngle", x, ...
        "AntennaHeight", elevation, ...
        "Latitude", latitude, ...
        "Longitude", longitude, ...
        "Name", "Transmitter", ...
        "TransmitterFrequency", frequency, ...
        "TransmitterPower", power);

floor = noiseFloor(295, frequency);

wrapCoverages = @(y) coverage(y, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);    

angles = cell(3:1);

% Create cell arrays of data ----------------------------------------------
for n = 1:numAngles
    angles{n, 1} = (360 / numAngles) * (n - 1);
end

txs = cellfun(wrapTxs, angles, 'uniformoutput',false);
coverages = cellfun(wrapCoverages, txs, 'uniformoutput',false);

toc;    

% View data on map --------------------------------------------------------
% viewer = siteviewer("Basemap","topographic");
% show(txs{2, 1});
% plot(coverages{1, 2});

rmpath ../torrent/raytracer/utils/;
rmpath ../torrent/raytracer/src/;