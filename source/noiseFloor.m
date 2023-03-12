% Calculate the noise floor given a temperature in K and a frequency

function [noiseFloor] = noiseFloor(temperature, bandwidth)
noiseFloor = 10*log10(physconst('Boltzmann') * temperature * bandwidth * 1000);
end