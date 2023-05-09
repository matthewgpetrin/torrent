function [score] = evalTxAngle(coverage, min_lat, max_lat, min_lon, max_lon, floor)

arguments
    coverage
    min_lat
    max_lat
    min_lon
    max_lon
    floor
end

table = coverage{n,m}.Data;
coverages = table2array(table);

score = 0;
for row = 1:size(coverage_array,1)
    if coverages(row,3) > floor && ...
        coverages(row,1) > min_lat && ...
        coverages(row,1) < max_lat && ...
        coverages(row,2) > min_lon && ...
        coverages(row,2) < max_lon
                        score = score + 1;
    end
end

end
    
    
    