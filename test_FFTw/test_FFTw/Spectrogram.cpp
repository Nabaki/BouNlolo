#include "Spectrogram.h"
#include <iostream>
#include <fftw3.h>


Spectrogram::Spectrogram():
nbChunk(0), windowLength(0)
{
}

Spectrogram::Spectrogram(double* signal, int signalLength, int windowSize, int hopSize):
nbChunk(0), windowLength(0)
{
	STFT(signal, signalLength, windowSize, hopSize);
}

Spectrogram::~Spectrogram()
{
}

double** Spectrogram::getSpectro() const
{
	return spectro;
}

int Spectrogram::getNbChunk() const
{
	return nbChunk;
}

int Spectrogram::getWindowLength() const
{
	return windowLength;
}

//Cr�e une 'hamming window', c'est un cosinus de p�riode windowlenght, partant de 0 allant jusque 1 et retournant � 0
void Spectrogram::hamming(int windowSize, double *buffer) {
	double pi = 3.14159265358979323846;
	for (int i = 0; i < windowSize; i++) {

		// M�me hamming window que matlab
		buffer[i] = 0.54 - (0.46 * cos(2 * pi * ((double)i / ((double)windowLength - 1))));
	}

}

//Short Time Fourier Transform, ou transform�e de Fourier glissante
//windowSize doit etre sup�rieure � hopSize (quand on ne sait pas on met la moiti� de windowSize)
void Spectrogram::STFT(double* signal, int signalLength, int windowSize, int hopSize) {

	//Taille du spectrogramme
		// On ne prendra que les (windowSize/2 + 1) premiers points de la fft.
		// We do this because the FFT output is mirrored about the nyquist 
		// frequency, so the second half of the data is redundant.
		windowLength = windowSize/2+1;

		//Nombre de chunk trait�s
		if (signalLength % hopSize != 0)
			spectro = new double*[signalLength / hopSize + 1];
		else
			spectro = new double*[signalLength / hopSize];
	
	// Initialisation des donn�es n�cessaires � la STFT
		fftw_complex* data = new fftw_complex[windowSize];
		fftw_complex* fft_result = new fftw_complex[windowSize];

		fftw_plan	plan;
		plan = fftw_plan_dft_1d(windowSize, data, fft_result, FFTW_FORWARD, FFTW_ESTIMATE);

		//ChunkPosition est la position du premier �l�ment des morceaux de musiques qui seront analys�s
		int chunkPosition = 0;
		int readIndex;

		// Should we stop reading in chunks? 
		int bStop = 0;
		int numChunks = 0;


	// Cr�ation du hammingWindow
	double* hammingWindow = new double[windowSize];
	hamming(windowSize, hammingWindow);


	// Process each chunk of the signal
	while (chunkPosition < signalLength && !bStop) {

		// Copy the chunk into our buffer
		for (int i = 0; i < windowSize; i++) {


			readIndex = chunkPosition + i;

			if (readIndex < signalLength) {

				// On remplie la data en multipliant les valeurs par le hamming window
				data[i][0] = signal[readIndex] * hammingWindow[i];
				data[i][1] = 0.0;

			}
			else {

				//ici nous avons d�pas� la longueur du signal donc on compl�te avec des z�ro
				data[i][0] = 0.0;
				data[i][1] = 0.0;

				bStop = 1; //dans ce cas on arr�te de boucler puiqu'on est � la fin du signal (� am�liorer)

			}
		}
		
		// On fait la fft sur notre morceau de musique
		fftw_execute(plan);

		//on copie les donn�es dans le tableau de sortie
		spectro[numChunks] = normalizeFft(fft_result, windowSize, hammingWindow);
		
		//on met � jour les valeurs et on recommence
		numChunks++;
		chunkPosition += hopSize; 

	} // Fin du while

	nbChunk = numChunks;

	fftw_destroy_plan(plan);

	//fftw_free(data); // � refaire
	//fftw_free(fft_result; //� refaire
}

double* Spectrogram::normalizeFft(fftw_complex* sp, int windowSize, double* hammingWindow)
{
	double* data = new double[windowLength];

	//Calcul de la valeur moyenne par laquelle le hammingWindow a multipli� la fft
	double moyHamming = 0;
	for (int j = 0; j < windowSize; j++)
		moyHamming += hammingWindow[j];
	moyHamming /= (double)windowSize;

	//Diff�rentes op�rations sur le signal complex de sortie :
	for (int i = 0; i < windowLength; i++){

		//On fait le module des valeurs
		data[i] = sqrt(sp[i][0] * sp[i][0] + sp[i][1] * sp[i][1]);
		//puis on normalise par rapport � la taille des window
		data[i] = data[i] / (double)windowSize;
		//puis par rapport � la moyenne de la hamming window
		data[i] = data[i] / moyHamming;
	}


	return data;
}