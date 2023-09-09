#include <iostream>
#include <string.h>
#include <getopt.h>

#include "Utility/stb_image.h"
#include <wiHelper.h>

bool DumpBin(const char* filepath, std::string header, const char* dataName, int channels)
{
	int height, width, bpp;
	unsigned char* rgba = stbi_load(filepath, &width, &height, &bpp, channels);
	return wi::helper::Bin2H(rgba, width * height * channels, header, dataName);
};

int main( int argc, char *argv[])
{
    std::string help = "Not enough arguments\nusage: Bin2H [Filepath] [Header filepath] [Dataname for byte array] [Amount of channels]";

	if ((argc >= 4)){
	std::string arg1, arg2, arg3, arg4;
	int chan;

		arg1 = argv[1];
		arg2 = argv[2];
		arg3 = argv[3];
		arg4 = argv[4];

		if(argc = 3){
			chan = 4; //default for these headers is 4 channels.
		}
		else{
			chan = atoi(arg4.c_str());
		}

		DumpBin(arg1.c_str(), arg2, arg3.c_str(), chan);
	}
	else
	{
		std::cout << help;
	}

}
