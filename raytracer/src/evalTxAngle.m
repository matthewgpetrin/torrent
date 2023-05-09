function score = transmissionScore(coverage, floor, minLatitude, maxLatitude, minLongitude, maxLongitude)

    arguments
        coverage
        floor
        minLatitude
        maxLatitude
        minLongitude
        maxLongitude
    end
    
    table = coverage.Data;
    coverageArray = table2array(table);
        score = 0;
        for row = 1:size(coverageArray,1)
            if coverageArray(row,1) > minLatitude && coverageArray(row,1) < maxLatitude && coverageArray(row,2) > minLongitude && coverageArray(row,2) < maxLongitude
                if coverageArray(row,3) > floor
                    score = score + 1;
                end
            end
        end
    end
