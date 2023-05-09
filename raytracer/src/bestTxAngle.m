function angle_idx = bestTxAngle(num_angs, num_cpus, min_lat,max_lat,min_lon,max_lon, options)

arguments
    num_angs
    num_cpus
    min_lat
    max_lat
    min_lon
    max_lon

    options.num_elements (1,1) double = 8
    options.tx_power (1,1) {mustBeNumeric} = 1
    options.tx_freq (1,1) {mustBeNumeric} = 2.4e9
    options.floor (1,1) {mustBeNumeric} = noiseFloor(295, 2.4e9)

    options.lat (1,1) {mustBeNumeric} = x
    options.lon (1,1) {mustBeNumeric} = y
    options.el (1,1) {mustBeNumeric} = 2

    options.inaccuracy (1,1) string = "high"
    options.num_reflects (1,1) int16 = 1

    options.terrain_material (1,1) string = "concrete"
    options.building_material (1,1) string = "concrete"
end

addpath("..\util\");
addpath("..\antennas\");

tic;
num_ang_per_cpu = num_angs / num_cpus;

%% Create angles Array ----------------------------------------------------
angles = cell(num_cpus, num_ang_per_cpu);

q = 0;
for n = 1:num_cpus
    for m = 1:num_ang_per_cpu
        angle = 360 / num_angs * (q);
        angles{n, m} = angle;
        q = q + 1;
    end
    disp("created angle group of size " + num_ang_per_cpu + " for core " + n);
end
disp("created parallel array of " + num_angs + " angles for " + num_cpus + " cores")

%% Create antenna array ---------------------------------------------------
antennas = cell(num_cpus, num_ang_per_cpu);

for n = 1: num_cpus
    for m = 1: num_ang_per_cpu
        antenna = UCA(options.tx_freq, angles{n,m}, options.num_elements);
        antennas{n,m} = antenna;
    end
    disp("created antenna group of size " + num_ang_per_cpu + " for core " + n);
end
disp("created parallel array of " + num_angs + " antennas for " + num_cpus + " cores")

%% Create tx sites array ---------------------------------------------------
txs = cell(num_cpus, num_ang_per_cpu);

for n = 1:num_cpus
    for m = 1:num_ang_per_cpu
        txs{n,m} = txsite(...
            "Antenna", antennas{n,m}, ...
            "Latitude", options.lat, ...
            "Longitude", options.lon, ...
            "AntennaHeight", options.el, ...
            "TransmitterFrequency", options.tx_freq, ...
            "TransmitterPower", options.tx_power);
    end
    disp("created txsite group of size " + num_ang_per_cpu + " for core " + n);
end
disp("created parallel array of " + num_angs + " txs for " + num_cpus + " cores")

%% Reflected coverage macro for cellfun ------------------------------------
ref_coverage = @(y) bouncedCoverage(y, ...
        "Accuracy", options.inaccuracy, ...
        "num_reflects", options.num_reflects, ...
        "terrain_material", options.terrain_material, ...
        "building_material", options.building_material);

%% Create coverages array -------------------------------------------------
coverages = cell(num_cpus, num_ang_per_cpu);

parfor n = 1:num_cpus
    coverages(n,:) = cellfun(ref_coverage, txs(n,:), 'uniformoutput',false);
    disp("created coverage group of size " + num_ang_per_cpu + " for core " + n);
end
disp("created parallel array of " + num_ang_per_cpu * num_cpus + " coverages for " + num_cpus + " cores")

%% Create array of scores -------------------------------------------------
scores = zeros(num_cpus, num_ang_per_cpu);

for n = 1:num_cpus
    for m = 1:num_ang_per_cpu
        score = evalTxAngle(coverages{n,m}, min_lat, max_lat, min_lon, max_lon, options.floor);
%         table = coverages{n,m}.Data;
%         coverageArray = table2array(table);
% 
%         score = 0;
%         for row = 1:size(coverageArray,1)
%             if coverageArray(row,3) > options.floor && ...
%                coverageArray(row,1) > min_lat && ...
%                coverageArray(row,1) < max_lat && ...
%                coverageArray(row,2) > min_lon && ...
%                coverageArray(row,2) < max_lon
%                         score = score + 1;
%             end
%         end
    end
        scores(n,m) = score;
end
    disp("created scores group of size " + num_ang_per_cpu + " for core " + n);

disp("created parallel array of " + num_ang_per_cpu * num_cpus + " scores for " + num_cpus + " cores");

%% Find best score and its index ------------------------------------------
[~,i] = max(scores(:));

[i_row, i_col] = ind2sub(size(scores),i);

angle_idx = angles{i_row, i_col};

idx = [i_row, i_col];

disp("optimal transmission angle of " + options.lat + ", "+ options.lon + ": " + idx);

%cov = coverages{i_row, i_col};

%plot(cov);

toc;

rmpath("..\util\");
rmpath("..\antennas\");

end
