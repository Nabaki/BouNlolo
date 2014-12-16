
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>
#include "Micro.h"


Micro::Micro():
	deviceName(NULL),device(NULL),captureDevice(NULL)

{
}

int Micro::start()


{
	if(!initOpenAl())
		return EXIT_FAILURE;
//On commence par v�rifier que la capture audio est support�e
	if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		std::cerr << "La capture audio n'est pas support�e par votre systeme" << std::endl;
		return EXIT_FAILURE;
	}
// Ouverture du device
	//captureDevice = alcCaptureOpenDevice(NULL, 44100, AL_FORMAT_MONO16, 44100);// le device a choisir et frequence d'�chantiollonage et format
	// the size of capture buffer 
	if (!initCapture())
	{
		std::cerr << "Impossible d'ouvrir le device de capture" << std::endl;
		return EXIT_FAILURE;
	}


	// Lancement de la capture
	alcCaptureStart(captureDevice);
// On va stocker les �chantillons captur�s dans un tableau d'entiers sign�s 16 bits

	// On attend que l'utilisateur appuie sur entr�e...
	std::cout << "Appuyez sur entree pour commencer 5 secondes de capture..." << std::endl;

	// ...Et c'est parti pour 5 secondes de capture
	time_t Start = time(NULL);
	while (time(NULL) - Start < 5)
	{
		// On r�cup�re le nombre d'�chantillons disponibles
		alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);

		// On lit les �chantillons et on les ajoute au tableau
		if (SamplesAvailable > 0)
		{
			std::size_t Start = samples.size();
			samples.resize(Start + SamplesAvailable);
			alcCaptureSamples(captureDevice, &samples[Start], SamplesAvailable);
		}
	}

	// On stoppe la capture
	alcCaptureStop(captureDevice);

	// On n'oublie pas les �ventuels �chantillons qu'il reste � r�cup�rer
	ALCint SamplesAvailable;
	alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
	if (SamplesAvailable > 0)
	{
		std::size_t Start = samples.size();
		samples.resize(Start + SamplesAvailable);
		alcCaptureSamples(captureDevice, &samples[Start], SamplesAvailable);
	}

	// On sauvegarde les �chantillons captur�s dans un fichier

	std::ofstream VectorSong;
	VectorSong.open("VectorSong.txt");
	for (int i = 0; i < samples.size(); i++)
	{
		VectorSong << samples[i] << std::endl;
	}

	// Fermeture du device de capture
	alcCaptureCloseDevice(captureDevice);

	// Fermeture d'OpenAL
	shutdownOpenAl();

	// On attend que l'utilisateur appuie sur entr�e...
	std::cout << "Appuyez sur entree pour terminer..." << std::endl;


	return EXIT_SUCCESS;


	return 0;

}

bool Micro::initOpenAl()
{
		device = alcOpenDevice(deviceName);
	if (!device)
	{
		std::cerr << "Impossible d'ouvrir le device par d�faut" << std::endl;
		return false;
	}

	// Cr�ation du contexte
	ALCcontext* Context = alcCreateContext(device, NULL);
	if (!Context)
	{
		std::cerr << "Impossible de cr�er un contexte audio" << std::endl;
		return false;
	}

	// Activation du contexte
	if (!alcMakeContextCurrent(Context))
	{
		std::cerr << "Impossible d'activer le contexte audio" << std::endl;
		return false;
	}

	return true;
}

bool Micro::initCapture()
{
// On commence par v�rifier que la capture audio est support�e
	if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		std::cerr << "La capture audio n'est pas support�e par votre systeme" << std::endl;
		return false;
	}
	// Ouverture du device
	captureDevice = alcCaptureOpenDevice(deviceName, 44100, AL_FORMAT_MONO16, 44100);
	if (!captureDevice)
	{
		std::cerr << "Impossible d'ouvrir le device de capture" << std::endl;
		return false;
	}

	return true;

}


void Micro:: shutdownOpenAl()
{
	// R�cup�ration du contexte
	ALCcontext* Context = alcGetCurrentContext();

	// D�sactivation du contexte
	alcMakeContextCurrent(NULL);

	// Destruction du contexte
	alcDestroyContext(Context);

	// Fermeture du device
	alcCloseDevice(device);
}


void Micro:: saveSound(const std::string& Filename)
{
	// On renseigne les param�tres du fichier � cr�er
	SF_INFO FileInfos;
	FileInfos.channels = 1;
	FileInfos.samplerate = 44100;
	FileInfos.format = SF_FORMAT_PCM_16 | SF_FORMAT_WAV;

	// On ouvre le fichier en �criture
	SNDFILE* File = sf_open(Filename.c_str(), SFM_WRITE, &FileInfos);
	if (!File)
	{
		std::cerr << "Impossible de cr�er le fichier audio" << std::endl;
		return;
	}

	// Ecriture des �chantillons audio
	sf_write_short(File, &samples[0], samples.size());

	// Fermeture du fichier
	sf_close(File);
}
std::vector<const short> Micro::getSamples()
{
	return samples;
}