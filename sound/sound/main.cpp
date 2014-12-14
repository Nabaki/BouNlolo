// sound.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "Sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>





int main()
    {
		const char* path="ocean.wav";
		Sound s1=Sound(path);
		s1.Open();
		std::cout<<s1.getRate();
		//std::ofstream files;
		//files.open ("files.txt");
		//files<<s1.getbuff();

		

	}



