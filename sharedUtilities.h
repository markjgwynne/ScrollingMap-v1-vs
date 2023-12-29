#pragma once

#ifndef SHAREDUTILITIES_H_INCLUDED
#define SHAREDUTILITIES_H_INCLUDED

#include "olcPixelGameEngine.h"


namespace ScrollingMap
{

	enum tiletype
	{
		Grass,
		Tree,
		Sand,
		Path
	};

	std::map<tiletype, olc::Pixel> tileMap {
		{ tiletype::Grass, olc::GREEN },
		{ tiletype::Tree, olc::DARK_GREEN },
		{ tiletype::Sand, olc::DARK_YELLOW },
		{ tiletype::Path, olc::GREY }
	};

	std::map<tiletype, std::string> tileTypeName {
		{ tiletype::Grass, "Grass" },
		{ tiletype::Tree, "Tree" },
		{ tiletype::Sand, "Sand" },
		{ tiletype::Path, "Path" }
	};

	enum xDirection {
		NONE_X,
		EAST,
		WEST
	};

	enum yDirection {
		NONE_Y,
		NORTH,
		SOUTH
	};

};

#endif