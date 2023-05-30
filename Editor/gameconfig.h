#pragma once
#include "WickedEngine.h"
#include <string>
#include <map>
#include <filesystem>

class Gameconfig
{
public:

	static Gameconfig& getInstance() {
		static Gameconfig instance;  // This instance is created lazily and only once
		return instance;
	}
	wi::config::File config;



	void get_game_directories(const std::string& s)
	{
		if (!std::filesystem::exists(s))
			return;

		for (auto& p : std::filesystem::directory_iterator(s))
			if (p.is_directory())
				games.emplace(p.path().stem().string(), p.path().string());
	}

	std::map<std::string, std::string> games;

	std::string currentgame = "default";

	~Gameconfig()
	{
		config.Commit();
	}

private:
	Gameconfig() {}  // Private constructor to prevent external instantiation
};

