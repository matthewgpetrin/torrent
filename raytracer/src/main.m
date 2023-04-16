function angleTable = main %(ang, cpu, minLat, maxLat, minLon, maxLon)

% origin point:
% latitude = 40.745589;
% longitude = -74.024837;
% main 4 4 40.7430 40.7455 -74.0270 -74.0240 0.0005
% main 4 4 40.7455 40.7458 -74.0249 -74.0246 0.0001
% main 12 6 40.7445 40.7455 -74.0255 -74.0235 0.0005

tic;
promptAng = "Angles:";
ang = input(promptAng);
promptCPU = "CPUs:";
cpu = input(promptCPU);
promptMinLat ="Minimum Latitude:";
minLat = input(promptMinLat);
promptMaxLat ="Maximum Latitude:";
maxLat= input(promptMaxLat);
promptMinLon = "Min Longitude:";
minLon = input(promptMinLon);
promptMaxLon = "Max Longitude:";
maxLon = input(promptMaxLon);
promptIncrement= "Increment:";
incrementValue = input(promptIncrement);
numberOfAngles = (ang);
    disp('Number Of Angles: ');
    disp(numberOfAngles);
numberOfCPUs = (cpu);
    disp(numberOfCPUs);
    disp('Number Of CPUs: ');
minimumLatitude = (minLat);
    disp(minimumLatitude);
maximumLatitude = (maxLat);
    disp(maximumLatitude);
minimumLongitude = (minLon);
    disp(minimumLongitude);
maximumLongitude = (maxLon);
    disp(maximumLongitude);
incrementAmount = 1/incrementValue;
latitudedifference = minus(maximumLatitude,minimumLatitude);    
longitudedifference = minus(maximumLongitude,minimumLongitude);
lengthLatitude = times(latitudedifference,incrementAmount)-1;
lengthLatitude = round(lengthLatitude);
disp('lengthLatitude: ');
    disp(lengthLatitude);
lengthLongitude = times(longitudedifference,incrementAmount)-1;
lengthLongitude = round(lengthLongitude);
disp('lengthLongitude: ');
    disp(lengthLongitude);
totalPoints = lengthLatitude*lengthLongitude; %total points within area
%totalPoints = round(totalPoints);
%% checks if points can be divided evenly between cores -------------------
%modValue = mod(numberOfAngles,numberOfCPUs);
%if modValue < 0.00001 %turn very small decimal mod value (ex: 1e-12) to 0
%    modValue = 0;
%end
% check if points can be evenly divided
%if modValue ~=0
%    disp('unequal points to divide between cores');
%    return;
%end
%% create 3D array to stores coords with bestAngle per core ---------------
%original coord values:
%x=40.745589;
%y=-74.024837;
%
%numberOfPointsPerCPU = totalPoints/numberOfCPUs; %total points per core
%numberOfPointsPerCPU = round(numberOfPointsPerCPU); %ensures numberOfPointsPerCPU is an integer
%% create a 2D array for all the points in the area -----------------------

coordinatePointsAllIndex = 1;
coordinatePointsAll2D = zeros(totalPoints,2);
for a=1:lengthLatitude
    for b=1:lengthLongitude
        x = minimumLatitude + incrementValue*a;
        y = minimumLongitude + incrementValue*b;
        coordinatePointsAll2D(coordinatePointsAllIndex,:) = [x,y];
        coordinatePointsAllIndex = coordinatePointsAllIndex + 1;
    end
end
disp(coordinatePointsAll2D);
coordinatePointsAll2DX = coordinatePointsAll2D(:,1);
coordinatePointsAll2DY = coordinatePointsAll2D(:,2);
disp('Total number of coordinate points: ')
    disp(totalPoints);
%% turns 2D array into 3D array to divide coords for each core ------------
%disp('Number of points per core: ');
%disp(numberOfPointsPerCPU);
%coordinatePointsAll3D = zeros(numberOfPointsPerCPU,2,numberOfCPUs);
%counter = 1;
%for i=1:numberOfCPUs
%    while counter <= numberOfPointsPerCPU
%        coordinatePointsAll3D(counter,:,i) = coordinatePointsAll2D(counter+(numberOfPointsPerCPU.*(i-1)),:);
%        counter = counter +1 ;
%    end
%    counter = 1;
%end
%disp(coordinatePointsAll3D);
%% bestAngle parfor loop --------------------------------------------------
%
%coordinatePoints3D = zeros(numberOfPointsPerCPU,4,numberOfCPUs);
%
%for a=1:numberOfCPUs
%    for b=1:numberOfPointsPerCPU
%       
%    
%            m = bestAngle(numberOfAngles, numberOfCPUs, minimumLatitude,maximumLatitude, minimumLongitude,maximumLongitude,...
%                "nElements", 8, ...
%                "Power", 1, ...
%                "Frequency",  2.4e9, ...
%                "Latitude", coordinatePointsAll3D(b,1,a), ...
%                "Longitude", coordinatePointsAll3D(b,2,a), ...
%                "Elevation", 2, ...
%                "Inaccuracy",  "high", ...
%                "Reflections",  1, ...
%                "TerrainMaterial",  "perfect-reflector", ...
%                "BuildingMateria" + ...
%                "l",  "perfect-reflector");
%
%            coordinatePoints3D(b,:,a) = [x,y,m];
%            
%            
%    end
%    
%end
%disp(coordinatePoints3D);
%% bestAngle parfor loop ver2 ---------------------------------------------

coordinateAngle = [];



parfor a = 1:totalPoints

            m = chosenAngle(numberOfAngles, numberOfCPUs, minimumLatitude,maximumLatitude, minimumLongitude,maximumLongitude,...
                "nElements", 8, ...
                "Power", 1, ...
                "Frequency",  2.4e9, ...
                "Latitude", coordinatePointsAll2DX(a,1), ...
                "Longitude", coordinatePointsAll2DY(a,1), ...
                "Elevation", 2, ...
                "Inaccuracy",  "high", ...
                "Reflections",  1, ...
                "TerrainMaterial",  "perfect-reflector", ...
                "BuildingMateria" + ...
                "l",  "perfect-reflector");
            
            
            coordinateAngle(a,:) = m;

  
end

coordAngleArray = horzcat(coordinatePointsAll2D,coordinateAngle);

%% Putting all cell arrays into a single array
%angleArray = [];
%
%for m = 1:numberOfCPUs
%    angleArray = vertcat(angleArray,coordinatePoints3D(:,:,m));
%end
%% Translate index to angle value and replaces index with angle values ----
%
% converts index to angle for each points
%angleTranslateArray = [];
%for angleArrayRowIndex = 1:totalPoints
%    A = coordAngleArray(angleArrayRowIndex,3);
%    B = coordAngleArray(angleArrayRowIndex,4);
%    angleRowIncrement = 360/(numberOfAngles/numberOfCPUs);
%    angleColumnIncrement = 360/numberOfAngles;
%    angleTranslate = (angleRowIncrement .* (A-1)) + (angleColumnIncrement) .* (B-1);
%    angleTranslateArray(angleArrayRowIndex,:) = angleTranslate;
%    angleArrayRowIndex = angleArrayRowIndex + 1;
%end
%
% replaces index with angles
%coordAngleArray(:,4) = [];
%coordAngleArray(:,3) = [];
%angleArray = horzcat(coordAngleArray, angleTranslateArray);
%% Outputs array of angle for all points ----------------------------------
disp('table for bestAngle of all coordinates: ');

latitudeValues = coordAngleArray(:,1);
longitudeValues = coordAngleArray(:,2);
bestAngle = coordAngleArray(:,3);

angleTable = table;
angleTable.Latitude = latitudeValues;
angleTable.Longitude = longitudeValues;
angleTable.Angle = bestAngle;
writetable(angleTable, 'angleTable.txt');
disp(angleTable);
toc;

