% This file calculates different coverage data for the same antenna angled
% in different directions at the same location. Its speed should be 
% compared to VectorizedTest in order to determine which solution will be 
% more efficient. Note that the comparative speed of the files will likely 
% change on different machines and for different numAngles values because 
% any given computer can only run these loops on as many cores as it has.
%
% Alter numAngles to increase or decrease how many angles are compared.
% 
% Note that the code may return an error when attempting to plot the data. 
% I don't know why this is happening. The code returns properly formatted 
% data, but refuses to plot it.

addpath ../../include/;
addpath ../../source/;

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


numAngles = 12;
numCores = 6;

% Define propagation model using raytracing -------------------------------
% reflected model
prop = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", reflections, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);
% non reflected model
prop1 = propagationModel("raytracing", ...
    "Method", "sbr", ...
    "AngularSeparation","high", ...
    "MaxNumReflections", 0, ...
    "TerrainMaterial",terrainMaterial, ...
    "BuildingsMaterial",buildingMaterial);

% Loop simulation ---------------------------------------------------------
tic;
floor = noiseFloor(295, 2.4e9);
% Creates anonymous function for coverages. This decreases verbosity when
% using cellfun with functions with multiple inputs. It is basically a
% macro
wrapCoverages = @(y) coverage(y, prop, ...
        "MaxRange", 500, ...
        "Resolution", 3, ...
        "ShowLegend", true, ...
        "SignalStrengths", floor:-5, ...
        "Transparency", 0.6);



% Determines how many angles will be run on each core
numAnglesPerCore = numAngles / numCores;

% Define cell arrays with #core columns. The angles to be processed by
% each core are found in the rows.
txs = cell(numCores, numAnglesPerCore);
angles = cell(numCores, numAnglesPerCore);
coverages = cell(numCores, numAnglesPerCore);


q = 0;
% Populates the angles cell array with angles
for o = 1:numCores
    for p = 1:numAnglesPerCore
        angle = 360 / numAngles * (q);
        angles{o, p} = angle;
        q = q + 1;
    end
end

disp(angles);

% Loop populates angle array with angles
for n = 1:numCores
    for m = 1:numAnglesPerCore
        txs{n,m} = txsite(...
            "Antenna", antenna, ...
            "AntennaAngle", angles{n,m}, ...
            "AntennaHeight", elevation, ...
            "Latitude", latitude, ...
            "Longitude", longitude, ...
            "Name", "Transmitter", ...
            "TransmitterFrequency", frequency, ...
            "TransmitterPower", power);
        disp("created tx object group");
    end
end

% Parallel for loop in which each core is assigned an array of txsite
% objects and populates the coverages cell array with propagationData
% objects
parfor n = 1:numCores
    coverages(n,:) = cellfun(wrapCoverages, txs(n,:), 'uniformoutput',false);
    disp("created coverage object group");
end

toc;
tic;
% compute coverage data for the 12 angle arrays with 6 cores

coverageData0 = coverages{1,1};
coverageData30 = coverages{1,2};
coverageData60 = coverages{2,1};
coverageData90 = coverages{2,2};
coverageData120 = coverages{3,1};
coverageData150 = coverages{3,2};
coverageData180 = coverages{4,1};
coverageData210 = coverages{4,2};
coverageData240 = coverages{5,1};
coverageData270 = coverages{5,2};
coverageData300 = coverages{6,1};
coverageData330 = coverages{6,2};


% create 12 tables for each coverage data
T0 = coverageData0.Data;
T30 = coverageData30.Data;
T60 = coverageData60.Data;
T90 = coverageData90.Data;
T120 = coverageData120.Data;
T150 = coverageData150.Data;
T180 = coverageData180.Data;
T210 = coverageData210.Data;
T240 = coverageData240.Data;
T270 = coverageData270.Data;
T300 = coverageData300.Data;
T330 = coverageData330.Data;

% create txt for coverage data table for 12 angles
% (optional: used for checking values )

writetable(T0, 'angle0.txt');
writetable(T30, 'angle30.txt');
writetable(T60, 'angle60.txt');
writetable(T90, 'angle90.txt');
writetable(T120, 'angle120.txt');
writetable(T150, 'angle150.txt');
writetable(T180, 'angle180.txt');
writetable(T210, 'angle210.txt');
writetable(T240, 'angle240.txt');
writetable(T270, 'angle270.txt');
writetable(T300, 'angle300.txt');
writetable(T330, 'angle330.txt');

% more variables and empty array
noiseFloorScoreArray = [];
score = 0;
limit = floor; %noisefloor
dataLength = 88796;
% combines table into 1 table
coverageDataTable = table(T0,T30,T60,T90,T120,T150,T180,T210,T240,T270,T300,T330);
% splitvar function to allow writing nested tables
T1 = splitvars(coverageDataTable);
% turns table into array
coverageDataArray = table2array(T1);
% loop to calculate number of data above threshhold 

for angle = 3:3:numAngles*3 %checks only the power value of each table
    for row = 1:dataLength %checks through all datapoints
        if coverageDataArray(row,angle) >= limit %compares power value to noise floor
            score = score + 1; %if true, increment by 1
        end
    end
    noiseFloorScoreArray(length(noiseFloorScoreArray)+1) = score; %adds result to array
    score = 0; %resets score counter to zero for next angle
end

noiseFloorTotal = table(noiseFloorScoreArray);
% function to find the highest score
[val, idx] = max(noiseFloorScoreArray);
% display the angle with the highest score
disp('The best angle is:')
disp((idx-1)*30);

% organizes and displays score value in a 2x6 array to match the angle arrays
noiseFloorScoreArrayOdd = [noiseFloorScoreArray(1,1);noiseFloorScoreArray(1,3);noiseFloorScoreArray(1,5);noiseFloorScoreArray(1,7);noiseFloorScoreArray(1,9);noiseFloorScoreArray(1,11)];
noiseFloorScoreArrayEven = [noiseFloorScoreArray(1,2);noiseFloorScoreArray(1,4);noiseFloorScoreArray(1,6);noiseFloorScoreArray(1,8);noiseFloorScoreArray(1,10);noiseFloorScoreArray(1,12)];
noiseFloorScoreArray2D = [noiseFloorScoreArrayOdd,noiseFloorScoreArrayEven];    
disp(noiseFloorScoreArray2D);

toc;
tic;
% switch case function to plot the best angle found
switch idx
    case 12
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{6,2});
    case 11
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{6,1});
    case 10
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{5,2});
    case 9
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{5,1});
    case 8
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{4,2});
    case 7
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{4,1});
    case 6
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{3,2});
    case 5
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{3,1});
    case 4
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{2,2});
    case 3
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{2,1});
    case 2
    siteviewer("Buildings","stevens.osm","Basemap","topographic");
    plot(coverages{1,2});
    otherwise
% View coverage data ------------------------------------------------------
siteviewer("Buildings","stevens.osm","Basemap","topographic");
plot(coverages{1,1});
end
toc;
rmpath ../../source/;
rmpath ../../include/;
