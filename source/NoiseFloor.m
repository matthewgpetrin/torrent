function [noiseFloor] = NoiseFloor(temperature, bandwidth)
noiseFloor = 10*log10(physconst('Boltzmann') * temperature * bandwidth * 1000);
end