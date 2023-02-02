function [wattsMatrix] = dBmToWatts(dBmMatrix)
wattsMatrix = 10.^((dBmMatrix-30)/10);
end

