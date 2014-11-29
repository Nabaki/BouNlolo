// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <alc.h>
#include <al.h>
#include <iostream>
#include <vector>
#include <sndfile.h>
#include <ctime>
#include <iomanip>
#include<fstream>

ALCdevice* Device = NULL;
ALCdevice* CaptureDevice = NULL;





bool InitOpenAL(const char* DeviceName = NULL)
{
	// Ouverture du device
	Device = alcOpenDevice(DeviceName);
	if (!Device)
	{
		std::cerr << "Impossible d'ouvrir le device par défaut" << std::endl;
		return false;
	}

	// Création du contexte
	ALCcontext* Context = alcCreateContext(Device, NULL);
	if (!Context)
	{
		std::cerr << "Impossible de créer un contexte audio" << std::endl;
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


bool InitCapture(const char* DeviceName = NULL)
{
	// On commence par vérifier que la capture audio est supportée
	if (alcIsExtensionPresent(Device, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		std::cerr << "La capture audio n'est pas supportée par votre systeme" << std::endl;
		return false;
	}

	// Ouverture du device
	CaptureDevice = alcCaptureOpenDevice(DeviceName, 44100, AL_FORMAT_MONO16, 44100);
	if (!CaptureDevice)
	{
		std::cerr << "Impossible d'ouvrir le device de capture" << std::endl;
		return false;
	}

	return true;
}


void SaveSound(const std::string& Filename, const std::vector<ALshort>& Samples)
{
	// On renseigne les paramètres du fichier à créer
	SF_INFO FileInfos;
	FileInfos.channels = 1;
	FileInfos.samplerate = 44100;
	FileInfos.format = SF_FORMAT_PCM_16 | SF_FORMAT_WAV;

	// On ouvre le fichier en écriture
	SNDFILE* File = sf_open(Filename.c_str(), SFM_WRITE, &FileInfos);
	if (!File)
	{
		std::cerr << "Impossible de créer le fichier audio" << std::endl;
		return;
	}

	// Ecriture des échantillons audio
	sf_write_short(File, &Samples[0], Samples.size());

	// Fermeture du fichier
	sf_close(File);
}


void ShutdownCapture()
{
	// Fermeture du device de capture
	alcCaptureCloseDevice(CaptureDevice);
}


////////////////////////////////////////////////////////////
/// Ferme proprement OpenAL
///
////////////////////////////////////////////////////////////
void ShutdownOpenAL()
{
	// Récupération du contexte
	ALCcontext* Context = alcGetCurrentContext();

	// Désactivation du contexte
	alcMakeContextCurrent(NULL);

	// Destruction du contexte
	alcDestroyContext(Context);

	// Fermeture du device
	alcCloseDevice(Device);
}

int _tmain(int argc, _TCHAR* argv[])
{


	if (!InitOpenAL(NULL))
		return EXIT_FAILURE;



	
	// On commence par vérifier que la capture audio est supportée
	if (alcIsExtensionPresent(Device, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		std::cerr << "La capture audio n'est pas supportée par votre systeme" << std::endl;
		return EXIT_FAILURE;
	}

	// Ouverture du device
	CaptureDevice = alcCaptureOpenDevice(NULL, 44100, AL_FORMAT_MONO16, 44100);// le device a choisir et frequence d'échantiollonage et format
	// the size of capture buffer 
	if (!CaptureDevice)
	{
		std::cerr << "Impossible d'ouvrir le device de capture" << std::endl;
		return EXIT_FAILURE;
	}


	/*
	// Vidage de la liste
	std::vector<std::string> Devices;
	Devices.clear();

	// Récupération des devices disponibles
	const ALCchar* DeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
	const ALCchar *actualDeviceName;
	ALCdevice *pMyDevice;
	pMyDevice = alcOpenDevice(NULL);
	actualDeviceName = alcGetString(pMyDevice, ALC_DEVICE_SPECIFIER);
	std::cout << actualDeviceName;


	*/

	/*
	// Récupération des devices de capture disponibles
	std::vector<std::string> devices;

	const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

	if (deviceList)
	{
		while (strlen(deviceList) > 0)
		{
			devices.push_back(deviceList);
			deviceList = deviceList + 1;
		}

	}

	//getCaptureDevice(devices);

	// On affiche la liste à l'utilisateur
	std::cout << "Veuillez choisir un device pour la capture :" << std::endl;
	for (int i = 0; i < devices.size(); ++i)
		std::cout << i+1 << "-" << devices[i] << std::endl;

	// Et on le laisse choisir
	int Choice;
	std::cin >> Choice;


	// Initialisation de la capture
	if (!InitCapture(actualDeviceName))
	return EXIT_FAILURE;
	*/

	// Lancement de la capture
	alcCaptureStart(CaptureDevice);

	// On va stocker les échantillons capturés dans un tableau d'entiers signés 16 bits
	std::vector<ALshort> Samples;

	// On attend que l'utilisateur appuie sur entrée...
	std::cout << "Appuyez sur entree pour commencer 5 secondes de capture..." << std::endl;
//	std::cin.ignore(10000, '\n');
//	std::cin.ignore(10000, '\n');

	// ...Et c'est parti pour 5 secondes de capture
	time_t Start = time(NULL);
	while (time(NULL) - Start < 5)
	{
		// On récupère le nombre d'échantillons disponibles
		ALCint SamplesAvailable;
		alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);

		// On lit les échantillons et on les ajoute au tableau
		if (SamplesAvailable > 0)
		{
			std::size_t Start = Samples.size();
			Samples.resize(Start + SamplesAvailable);
			alcCaptureSamples(CaptureDevice, &Samples[Start], SamplesAvailable);
		}
	}

	// On stoppe la capture
	alcCaptureStop(CaptureDevice);

	// On n'oublie pas les éventuels échantillons qu'il reste à récupérer
	ALCint SamplesAvailable;
	alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
	if (SamplesAvailable > 0)
	{
		std::size_t Start = Samples.size();
		Samples.resize(Start + SamplesAvailable);
		alcCaptureSamples(CaptureDevice, &Samples[Start], SamplesAvailable);
	}
	
	//écrire dans un fichier
	std::ofstream myfile;
	myfile.open("samples.txt");
	for (int i = 0; i<Samples.size(); i++)
	{
		myfile << Samples[i] << " ";
	}
	myfile.close();
	

	// On sauvegarde les échantillons capturés dans un fichier
	SaveSound("bou3ou3.wav", Samples);

	// Fermeture de la capture
	ShutdownCapture();

	// Fermeture d'OpenAL
	ShutdownOpenAL();

	// On attend que l'utilisateur appuie sur entrée...
	std::cout << "Appuyez sur entree pour terminer..." << std::endl;
	std::cin.ignore(10000, '\n');

	return EXIT_SUCCESS;


	return 0;
}

