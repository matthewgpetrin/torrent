function cov = bouncedCoverage(tx, options)

arguments
    tx

    options.Accuracy (1,1) string = "high"
    options.Reflections (1,1) int16 = 1

    options.TerrainMaterial (1,1) string = "perfect-reflector"
    options.BuildingMaterial (1,1) string = "perfect-reflector"
end

addpath raytracer/utils/;

floor = noiseFloor(295, tx.TransmitterFrequency);

%% Define propagation models -----------------------------------------------

prop_0 = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation",options.Accuracy, ...
    "MaxNumReflections", 0, ...
    "TerrainMaterial", options.TerrainMaterial, ...
    "BuildingsMaterial", options.BuildingMaterial);

prop_1 = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation",options.Accuracy, ...
    "MaxNumReflections", options.Reflections, ...
    "TerrainMaterial", options.TerrainMaterial, ...
    "BuildingsMaterial", options.BuildingMaterial);

%% Coverages macros --------------------------------------------------------
coverage_0 = coverage(tx, prop_0, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.5);

coverage_1 = coverage(tx, prop_1, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.5);

%% Create arrays of coverage data objects and parallel process -------------
table_0 = coverage_0.Data;
table_1 = coverage_1.Data;
table = table_1;

table_0.Power = wattsTodBm(coverage_0.Data.Power);
table_1.Power = wattsTodBm(coverage_1.Data.Power);
table.Power = dBmToWatts(table_1.Power - table_0.Power);

table.Power = real(table.Power);

writetable(table_0,'raytracer/data/coverage_0.txt');
writetable(table_1,'raytracer/data/coverage_1.txt');
writetable(table, 'raytracer/data/coverage');

cov = propagationData(table);

rmpath raytracer/utils/;

end