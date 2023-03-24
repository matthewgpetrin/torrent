% This file calculates different coverage data for the same antenna angled
% in different directions at the same location. Its speed should be 
% compared to VectorizedTest in order to determine which solution will be 
% more efficient. Note that the comparative speed of the files will likely 
% change on different machines and for different nAngles values because 
% any given computer can only run these loops on as many cores as it has.
%
% Alter nAngles to increase or decrease how many angles are compared.
% 
% Note that the code may return an error when attempting to plot the data. 
% I don't know why this is happening. The code returns properly formatted 
% data, but refuses to plot it.

addpath ../torrent/raytracer/src/;
addpath ../torrent/raytracer/utils/;

tic;

% Input variables ---------------------------------------------------------

antenna = YagiAntenna;
frequency = 2.4e9;
power = 0.1;

latitude = 40.745589;
longitude = -74.024837;

elevation = 1;

reflections = 1;

terrainMaterial = "perfect-reflector";
buildingMaterial = "perfect-reflector";


nAngles = 12;
nCores = 6;

nAnglesPerCore = nAngles / nCores;

floor = noiseFloor(295, 2.4e9);

% Define propagation models -----------------------------------------------

prop_0 = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", 0, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

prop_1 = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", reflections, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

% Coverages macros --------------------------------------------------------
cCoverage_0 = @(y) coverage(y, prop_0, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);

cCoverage_1 = @(y) coverage(y, prop_1, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);

% Create Angles Array -----------------------------------------------------
angles = cell(nCores, nAnglesPerCore);

q = 0;
for n = 1:nCores
    for m = 1:nAnglesPerCore
        angle = 360 / nAngles * (q);
        angles{n, m} = angle;
        q = q + 1;
    end
    disp("created angle group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " angles for " + nCores + " cores")

disp(angles);

% Create tx sites array ---------------------------------------------------
txs = cell(nCores, nAnglesPerCore);

for n = 1:nCores
    for m = 1:nAnglesPerCore
        txs{n,m} = txsite(...
            "Antenna", antenna, ...
            "AntennaAngle", angles{n,m}, ...
            "AntennaHeight", elevation, ...
            "Latitude", latitude, ...
            "Longitude", longitude, ...
            "Name", "Transmitter", ...
            "TransmitterFrequency", frequency, ...
            "TransmitterPower", power);
    end
    disp("created txsite group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " txs for " + nCores + " cores")

% Create arrays of coverage data objects and parallel process -------------
coverages_0 = cell(nCores, nAnglesPerCore);
coverages_1 = cell(nCores, nAnglesPerCore);

parfor n = 1:nCores
    coverages_0(n,:) = cellfun(cCoverage_0, txs(n,:), 'uniformoutput',false);
    disp("created coverage group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " coverages for " + nCores + " cores")

parfor n = 1:nCores
    coverages_0(n,:) = cellfun(cCoverage_1, txs(n,:), 'uniformoutput',false);
    disp("created rt coverage group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " rt coverages for " + nCores + " cores")

% Create array of reflected coverage data ---------------------------------
tables = cell(nCores, nAnglesPerCore);

for n = 1:nCores
    for m = 1:nAnglesPerCore
        coverage_0 = coverages_0{n, m};
        coverage_1 = coverages_1{n, m};

        table_0 = coverage_0.Data;
        table_1 = coverage_1.Data;
        table = table_1;

        table_0.Power = dBmToWatts(table_0.Power);
        table_1.Power = dBmToWatts(table_1.Power);
        table.Power = table_1.Power - table_0.Power;

        table_0.Power = wattsTodBm(table_0.Power);
        table_1.Power = wattsTodBm(table_1.Power);
        table.Power = wattsTodBm(data.Power);

        tables{n,m} = table;
    end
    disp("created tables group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " tables for " + nCores + " cores")


% Create array of scores --------------------------------------------------
scores = zeros(nCores, nAnglesPerCore);

for n = 1:nCores
    for m = 1:nAnglesPerCore
        table = tables{n, m};
        score = 0;
        for o = 1:length(table.Power)
            if o > floor
                score = score + 1;
            end
        end
        scores(n,m) = score;
    end
    disp("created scores group of size " + nAnglesPerCore + " for core " + n);
end
disp("created parallel array of " + nAngles + " scores for " + nCores + " cores")

% Find best score and its angle -------------------------------------------
[s,i] = max(scores(:));

[i_row, i_col] = ind2sub(size(scores),i);

optimal_angle = angles{i_row, i_col};

toc;

% View coverage data ------------------------------------------------------
siteviewer("Buildings","stevens.osm","Basemap","topographic");
plot(coverages{i_row, i_col});

rmpath ../torrent/raytracer/utils/;
rmpath ../torrent/raytracer/src/;
