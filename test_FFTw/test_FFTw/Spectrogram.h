#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <fftw3.h>

class Spectrogram
{
	public:
		Spectrogram();
		Spectrogram(double* signal, int signalLength, int windowSize, int hopSize);
		~Spectrogram();

		double** getSpectro() const;
		int getNbChunk() const;
		int getWindowLength() const;
		
		double* normalizeFft(fftw_complex* sp, int windowSize, double* hammingWindow);
		void hamming(int windowSize, double *buffer);
		void STFT(double* signal, int signalLength, int windowSize, int hopSize);

	private:
		double** spectro;
		int nbChunk;
		int windowLength;
};

#endif
