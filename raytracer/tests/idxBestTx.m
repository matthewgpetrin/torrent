function idx = idxBestTx(txs, floor, options)

arguments
    txs
    floor

    options.Reflections (1,1) int16 = 1

    options.TerrainMaterial (1,1) string = "perfect-reflector"
    options.BuildingMaterial (1,1) string = "perfect-reflector"
end

addpath raytracer/utils/;

reflections = options.Reflections;

terrainMaterial = options.TerrainMaterial;
buildingMaterial = options.BuildingMaterial;

nRows = size(txs, 1);
nCols = size(txs, 2);

%% Define propagation models -----------------------------------------------

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

%% Coverages macros --------------------------------------------------------
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


%% Create arrays of coverage data objects and parallel process -------------
coverages_0 = cell(nRows, nCols);
coverages_1 = cell(nRows, nCols);

parfor n = 1:nRows
    coverages_0(n,:) = cellfun(cCoverage_0, txs(n,:), 'uniformoutput',false);
    disp("created coverage group of size " + nCols + " for core " + n);
end
disp("created parallel array of " + nCols * nRows + " coverages for " + nRows + " cores")

parfor n = 1:nRows
    coverages_1(n,:) = cellfun(cCoverage_1, txs(n,:), 'uniformoutput',false);
    disp("created rt coverage group of size " + nCols + " for core " + n);
end
disp("created parallel array of " + nCols * nRows + " rt coverages for " + nRows + " cores")

%% Create array of reflected coverage data ---------------------------------
tables = cell(nRows, nCols);

for n = 1:nRows
    for m = 1:nCols
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
        table.Power = wattsTodBm(table.Power);

        tables{n,m} = table;
    end
    disp("created tables group of size " + nCols + " for core " + n);
end
disp("created parallel array of " + nCols * nRows + " tables for " + nRows + " cores")


%% Create array of scores --------------------------------------------------
scores = zeros(nRows, nCols);

for n = 1:nRows
    for m = 1:nCols
        table = tables{n, m};
        score = 0;
        for o = 1:length(table.Power)
            if o > floor
                score = score + 1;
            end
        end
        scores(n,m) = score;
    end
    disp("created scores group of size " + nCols + " for core " + n);
end
disp("created parallel array of " + nCols * nRows + " scores for " + nRows + " cores")

%% Find best score and its index ------------------------------------------
[~,i] = max(scores(:));

[i_row, i_col] = ind2sub(size(scores),i);

idx = [i_row, i_col];


rmpath raytracer/utils/;

end