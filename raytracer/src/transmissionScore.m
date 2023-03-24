function score = transmissionScore(coverage, floor)

    arguments
        coverage
        floor
    end
    
    table = coverage.Data;
        score = 0;
        for o = 1:length(table.Power)
            if o > floor
                score = score + 1;
            end
        end
    end