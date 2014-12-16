#ifndef SOUND_H
#define SOUND_H


#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <alc.h>
#include <al.h>
#include <fstream>
#include <vector>
#include <string>


class Sound 
{
		public: 
			Sound(const std::string path);
				virtual ~Sound(){};
				void start();
				int getRate() const;
				int getFrame() const;
				double *getbuff() const;
				int getChannel() const;
				//std::vector<double> getSamples()  ;



		private:
				double *buff;
				int frame; 
				int channel;
				int rate;
				SNDFILE *sound;
				SF_INFO info;
				const std::string path;
				//xstd::vector<double> samples;
				
		
	
};
#endif