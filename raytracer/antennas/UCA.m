function [array] = UCA(frequency, angle, num_elements)

    % Create a uniform circular array using phased toolbox. This will be used
    % to calculate the necessary phase delays
    element = phased.IsotropicAntennaElement('FrequencyRange', [0 frequency]);
    
    phasedUCA = phased.UCA('NumElements', num_elements, ...
        'ArrayNormal', 'z', ...
        'Taper', ones(size(num_elements)), ...
        'Radius' , 0.5.*(physconst('LightSpeed')/frequency), ...
        'Element', element);
    
    if angle > 180
        angle = -(360 - angle);
    end
    
    % Double check this
    steeringAngles = angle;
    phaseShiftBits = 6;
    
    % Find the weights
    w = zeros(getNumElements(phasedUCA), length(frequency));
    
    % Calculate steering vector of antenna
    steerVector = phased.SteeringVector('SensorArray', phasedUCA, ...
        'PropagationSpeed', physconst('LightSpeed'), ...
        'NumPhaseShifterBits', phaseShiftBits);
    
    for n = 1:length(frequency)
        w(:, n) = step(steerVector, frequency(n), steeringAngles(:, n));
    end
    
    % Calculate phase shifts from phased toolbox array
    delay = phased.ElementDelay('SensorArray', phasedUCA);
    phaseShifts = delay(angle(1)).*360.*frequency;
    
    % Design array using antenna toolbox for accurate coverage
    antenna = design(dipole, 2.4e9);
    antenna.Length = 0.3937;
    antenna.Width = 0.01016;
    
    antennaUCA = circularArray("AmplitudeTaper", phasedUCA.Taper, ...
        "Element", antenna, ...
        "NumElements", phasedUCA.NumElements, ...
        "PhaseShift", phaseShifts, ...
        "Radius", phasedUCA.Radius, ...
        "TiltAxis", [0 0 1], ...
        "Tilt", 0);
    
    uca = design(antennaUCA, frequency, antenna);
    
    array = uca;