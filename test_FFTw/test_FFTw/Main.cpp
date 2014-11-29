#include <iostream>
#include <fftw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>

#include "Spectrogram.h"

double* recordSong()
{
	SNDFILE *sf;
	SF_INFO info;
	int num_items;
	double *in, *out;
	int f, sr, c;
	int i;

	// Open the WAV file
	info.format = 0;
	sf = sf_open("track.wav", SFM_READ, &info);
	if (sf == NULL)
	{
		std::cout << ("Failed to open the file.\n");
		exit(-1);
	}

	f = info.frames; // nombre d'échantillon
	sr = info.samplerate; // taux d'échantillonage 
	c = info.channels; //nombre de cannaux 
	std::cout << "frames = " << f << std::endl;
	std::cout << "samplerate : " << sr << std::endl;
	std::cout << "channels : " << c << std::endl;
	num_items = f*c;
	in = new double[num_items]; //allocation de la memoire // new [taille] ;
	out = new double[f];
	const int num = sf_read_double(sf, in, num_items);

	std::cout << "num : " << num << std::endl;
	sf_close(sf);

	std::ofstream VectorSong;
	VectorSong.open("VectorSong.txt");
	for (i = 0; i*c < num; i++){
		VectorSong << in[i*c] << " ";
		out[i] = in[i*c];
	}
		
	return out;
}

int main(){

	int N = 106150;
	double *song = recordSong();

	Spectrogram sp(song,N,2048,2048);

	//Créer un fichier qui recueille le spectrogramme
	std::ofstream Spectrogramme;
	Spectrogramme.open("Spectrogramme.txt");
	for (int i = 0; i < sp.getNbChunk(); i++){

		if (i != 0)
			Spectrogramme << std::endl;

		for (int j = 0; j < sp.getWindowLength(); j++)
			Spectrogramme << sp.getSpectro()[i][j] << " ";
	}

	system("pause");
	return 0;
}