#ifndef SOUND_H
#define COMPLEX_H


#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>

class Sound 
{
		public: 
				Sound(const char *path);
				virtual ~Sound(){};
				void Open();
				int getRate() const;
				int getFrame() const;
				double *getbuff() const;
				int getChannel() const;



		private:
				double *buff;
				int frame; 
				int channel;
				double rate;
				SNDFILE *sound;
				SF_INFO info;
				const char *path;
		
	
};
#endif