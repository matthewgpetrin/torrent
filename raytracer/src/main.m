function coord_angle_array = main %(ang, cpu, min_lat, max_lat, min_lon, max_lon)

% origin point:
% 2 meter = 0.000025
% main 12 12 40.743975 40.744600 -74.026025 -74.025375 0.000025

%% Prompt user for inputs--------------------------------------------------
num_angs = input("# Angles: ");

num_cpus = input("# CPUs: ");

min_lat = input("Minimum Latitude: ");
max_lat = input("Maximum Latitude: ");

min_lon = input("Minimum Longitude: ");
max_lon = input("Maximum Longitude: ");

accuracy = input("GPS Accuracy: ");

map_name = input("Map Name: ", 's');

num_angs = (num_angs);
num_cpus = (num_cpus);

min_lat = (min_lat);
max_lat = (max_lat);

min_lon = (min_lon);
max_lon = (max_lon);

disp("GPS Range: [" + min_lat + "," + min_lon ...
            + "] [" + max_lat + "," + max_lon + "]");

addpath("..\util\");
addpath("..\antennas\");

%% Compute points to be analyzed ------------------------------------------
x = 1 / accuracy;

lat_range = minus(max_lat, min_lat);    
lon_range = minus(max_lon, min_lon);

length_latitude = times(lat_range, x)-1;
length_latitude = round(length_latitude);
disp('length_latitude: ');
    disp(length_latitude);

length_longitude = times(lon_range, x) -1;
length_longitude = round(length_longitude);
disp('length_longitude: ');
    disp(length_longitude);

num_points = length_latitude * length_longitude; %total points within area

%% Check if points can be divided evenly between cores --------------------
%mod_val = mod(num_angs, num_cpus);
%
%if mod_val < 0.00001 %turn very small decimal mod value (ex: 1e-12) to 0
%    mod_val = 0;
%end
% 
%check if points can be evenly divided
%if mod_val ~= 0
%    disp('Cannot evenly divide points between cores');
%    return;
%end
%
%% Create 3D array to store coords with bestAngle per core ----------------
%points_per_cpu = num_points / num_cpus; %total points per core
%
%points_per_cpu = round(points_per_cpu); %ensures points_per_cpu is an integer
%
%% Create a 2D array for all the points in the area -----------------------
coord_points = zeros(num_points, 2);

idx = 1;
for a = 1:length_latitude
    for b = 1:length_longitude
        x = min_lat + accuracy * a;
        y = min_lon + accuracy * b;
        coord_points(idx,:) = [x,y];
        idx = idx + 1;
    end
end

%% bestAngle parfor loop ver2 ---------------------------------------------
coord_angles = [];

parfor idx = 1:num_points
%             m = bestTxAngle(num_angs, num_cpus, min_lat, max_lat, min_lon, max_lon,...
%                 "num_elements", 8, ...
%                 "tx_power", 1, ...
%                 "tx_freq",  2.4e9, ...
%                 "floor", noiseFloor(295, 2.4e9), ...
%                 "lat", coord_points_x(idx, 1), ...
%                 "lon", coord_points_y(idx, 1), ...
%                 "el", 2, ...
%                 "inaccuracy",  "high", ...
%                 "num_reflects",  2, ...
%                 "terrain_material", "concrete", ...
%                 "building_material", "concrete");
            m = rand * 360
            coord_angles(idx,:) = m;
end

coord_angle_array = horzcat(coord_points, coord_angles);
toc;

rmpath("..\util\");
rmpath("..\antennas\");

writematrix(coord_angle_array, "../data/" + map_name + ".trnt", 'FileType', 'text');