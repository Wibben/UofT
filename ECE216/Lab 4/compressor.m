function [ numDiscardedCoefficients, compressedTestSignal ] = compressor(testSignal, cutoff)
    
    fourierCoefficients = fft2(testSignal); %Obtain the fourier series coefficients.
    
    % Count number of coefficients with abs value less than cutoff
    numDiscardedCoefficients = nnz(abs(fourierCoefficients)<cutoff);
    
    % Set all coefficients with abs value less than cutoff to 0
    fourierCoefficients(abs(fourierCoefficients)<cutoff) = 0;
    
    compressedTestSignal = ifft2(fourierCoefficients);
end
