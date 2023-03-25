function idx = bestAngle(nAngles, nCPUs, options)

arguments
    nAngles
    nCPUs

    options.nElements (1,1) double = 8
    options.Power (1,1) {mustBeNumeric} = 1
    options.Frequency (1,1) {mustBeNumeric} = 2.4e9
    options.Floor (1,1) {mustBeNumeric} = noiseFloor(295, 2.4e9)

    options.Latitude (1,1) {mustBeNumeric} = 40.745589
    options.Longitude (1,1) {mustBeNumeric} = -74.024837
    options.Elevation (1,1) {mustBeNumeric} = 2

    options.Inaccuracy (1,1) string = "high"
    options.Reflections (1,1) int16 = 1

    options.TerrainMaterial (1,1) string = "perfect-reflector"
    options.BuildingMaterial (1,1) string = "perfect-reflector"
end

addpath ../utils/;

tic;

nAngPerCPU = nAngles/ nCPUs;

%% Create angles Array ----------------------------------------------------
angles = cell(nCPUs, nAngPerCPU);

q = 0;
for n = 1:nCPUs
    for m = 1:nAngPerCPU
        angle = 360 / nAngles * (q);
        angles{n, m} = angle;
        q = q + 1;
    end
    disp("created angle group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngles + " angles for " + nCPUs + " cores")

disp(angles);

%% Create antenna array ---------------------------------------------------
antennas = cell(nCPUs, nAngPerCPU);

for n = 1: nCPUs
    for m = 1: nAngPerCPU
        antenna = UCA(options.Frequency, angles{n,m}, options.nElements);
        antennas{n,m} = antenna;
    end
    disp("created antenna group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngles + " antennas for " + nCPUs + " cores")

%% Create tx sites array ---------------------------------------------------
txs = cell(nCPUs, nAngPerCPU);

for n = 1:nCPUs
    for m = 1:nAngPerCPU
        txs{n,m} = txsite(...
            "Antenna", antennas{n,m}, ...
            "Latitude", options.Latitude, ...
            "Longitude", options.Longitude, ...
            "AntennaHeight", options.Elevation, ...
            "TransmitterFrequency", options.Frequency, ...
            "TransmitterPower", options.Power);
    end
    disp("created txsite group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngles + " txs for " + nCPUs + " cores")

%% Reflected coverage macro for cellfun ------------------------------------
rCoverage = @(y) bouncedCoverage(y, ...
        "Accuracy", options.Inaccuracy, ...
        "Reflections", options.Reflections, ...
        "TerrainMaterial", options.TerrainMaterial, ...
        "BuildingMaterial", options.BuildingMaterial);

%% Create coverages array -------------------------------------------------
coverages = cell(nCPUs, nAngPerCPU);

parfor n = 1:nCPUs
    coverages(n,:) = cellfun(rCoverage, txs(n,:), 'uniformoutput',false);
    disp("created coverage group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngPerCPU * nCPUs + " coverages for " + nCPUs + " cores")

%% Create array of scores -------------------------------------------------
scores = zeros(nCPUs, nAngPerCPU);

for n = 1:nCPUs
    for m = 1:nAngPerCPU
        score = transmissionScore(coverages{n,m}, options.Floor);
        scores(n,m) = score;
    end
    disp("created scores group of size " + nAngPerCPU + " for core " + n);
end
disp("created parallel array of " + nAngPerCPU * nCPUs + " scores for " + nCPUs + " cores")

%% Find best score and its index ------------------------------------------
[~,i] = max(scores(:));

[i_row, i_col] = ind2sub(size(scores),i);

idx = [i_row, i_col];

disp("optimal transmission angle of " + angles(idx));

cov = coverages{i_row, i_col};

plot(cov);

toc;

rmpath ../utils/;

end