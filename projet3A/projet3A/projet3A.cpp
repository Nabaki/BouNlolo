// projet3A.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "stdafx.h"
#include "Sound.h"
#include "Micro.h"
#include <stdio.h>
#include <stdlib.h>
#include<vector>
#include <sndfile.h>
#include <iostream>
#include <fstream>
#include<string>







int main()
{		//std::string path="ocean.wav";
		//Sound s1 = Sound(path);
	//s1.Open();
	//std::cout << s1.getChannel();
	//std::ofstream files;
	//files.open ("files.txt");
	//files<<s1.getbuff();
	
	//std::vector<double> buff= s1.getSamples();
	

	
	Micro m = Micro();
	m.start();
	m.saveSound("bbbb.wav");
	std::vector<const short> buff= m.getSamples();
	for (int i = 0; i < 100; i++)
	{
		std::cout << buff[i];
	}
	


}
