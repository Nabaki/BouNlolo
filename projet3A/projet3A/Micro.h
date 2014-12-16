#ifndef MICRO_H
#define MICRO_H


#include "stdafx.h"
#include <alc.h>
#include <al.h>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>
#include <vector>

class Micro 
{
public:
	Micro();
	
	int start();
	virtual ~Micro(){};
	bool initOpenAl();
	bool initCapture();
	void shutdownOpenAl();
	void saveSound(const std::string& Filename);
	std::vector<const short> getSamples();
	


	 

private:

	const char* deviceName;
	ALCdevice* device;
	ALCdevice* captureDevice; 
	std::vector<const short> samples;
	ALCint SamplesAvailable;






};
#endif;