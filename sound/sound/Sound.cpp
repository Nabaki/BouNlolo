
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
//#include <fstream>
#include "Sound.h"


Sound::Sound(const char *_path):
	path(_path)

{
	Open();
}


void Sound ::Open()
{
	SNDFILE *fileOpen=sf_open(path,SFM_READ,&info);
	channel=info.channels;
	rate=info.samplerate;
	frame=info.frames;
	buff = new double[frame];
	double *buffin= new double[(channel*frame)];
	int data=sf_read_double(fileOpen,buff,channel*frame);
	std::ofstream VectorSong;
	for (int i = 0; i*channel < data; i++)
	{
		
		VectorSong.open("VectorSong.txt");
		VectorSong << buffin[i*channel] << " ";
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





 



