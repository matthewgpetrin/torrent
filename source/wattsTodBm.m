function [dBmMatrix] = wattsTodBm(wattsMatrix)
dBmMatrix = 10.*log10(wattsMatrix)+30;
end

