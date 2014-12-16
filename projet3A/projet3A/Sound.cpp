
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>

#include "Sound.h"
#include "Sounds.h"




Sound::Sound(const std::string _path) :
	path(_path)

{
	start();
}


void Sound ::start()
{
	sound = sf_open(path.c_str(), SFM_READ, &info);
	channel=info.channels;
	rate=info.samplerate;
	frame=info.frames;
	buff = new double[frame];
	double *buffin= new double[(channel*frame)];
	double data=sf_read_double(sound,buffin,channel*frame);
	sf_close(sound);
	std::ofstream VectorSong;
	VectorSong.open("VectorSong.txt");
	
	for (int i = 0; i*channel < data; i++)
	{
		
		
		VectorSong <<buffin[i*channel] << " ";
		buff[i] =buffin[i*channel] ;
	}
	

}

int Sound::getChannel() const
	{
		return channel;
	}


int Sound::getRate() const
{
	return rate;
}

int Sound::getFrame() const
{
	return frame;
}
double *Sound::getbuff() const
{
	return buff;
}

/*
std::vector<double> Sound::getSamples() 
{

	return samples;
}
*/



 



