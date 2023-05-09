function antenna = YagiAntenna()

% Variable definitions ----------------------------------------------------
frequency = 2.45e9;
    
exciterSpace = 25.4;
exciterLength = 63.5;
exciterDiameter = 25.4;
    
directorCount = 18;
    
% directorLength = [38.1 38.1 38.1 38.1 38.1  ...
%                   38.1 38.1 38.1 38.1 38.1  ...
%                   38.1 38.1 38.1 38.1 38.1  ...
%                   38.1 38.1 38.1];
% directorSpace = [12.2 28.6 41.3 41.3 41.3  ...
%                  44.5 44.5 44.5 44.5 44.5  ...
%                  47.6 44.5 47.6 47.6 47.6  ...
%                  47.6 41.3 44.5];
    
directorSpace = [ 12, 27, 38.5, 37, 38.5, 40.5, 48.15, 43.5, 42.5, 44.8, 45.33, 46.9, 47, 49.1, 51, 49.85, 33.15, 38.1 ];
    
directorLength = [ 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 49.3 ];
    
%reflectorLength = 82.3;
%reflectorSpace = 15.9;
reflectorLength = 82.5;
reflectorSpace = 13.7;
    
% Create exciter element and antenna --------------------------------------
exciter = design(dipoleFolded, frequency); % Auto generate dipole
    
% We should create a dipole manually using real dimensions. I have not been
% able to do so without getting errors
    
antenna = yagiUda("Exciter", exciter, 'NumDirectors', 18, ...
                      "DirectorLength", directorLength.*10^-3, ...
                      "DirectorSpacing", directorSpace.*10^-3, ...
                      "ReflectorLength", reflectorLength*10^-3, ...
                      "ReflectorSpacing", reflectorSpace*10^-3);
    
antenna.Tilt = 90;
antenna.TiltAxis = [0 1 0];