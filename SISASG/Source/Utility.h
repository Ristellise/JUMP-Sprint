#ifndef UTILITY_H
#define UTILITY_H

#include "Mtx44.h"
#include "Vertex.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
// Windows
#include <direct.h>
#define GetCurrentDir _getcwd
#else
// Linux.
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


Position operator*(const Mtx44& lhs, const Position& rhs);
std::string CurrentDirectory();
#endif
