#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture
{
public:

	unsigned int id;
	std::string type; //matDiffuse, matSpecular, matEmissive
	std::string path;
};


#endif