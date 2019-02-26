#include "Utility.h"
#include <random>
#include <functional>

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#endif
Position operator*(const Mtx44& lhs, const Position& rhs)
{
    float b[4];
    for (int i = 0; i < 4; i++)
        b[i] = lhs.a[0 * 4 + i] * rhs.x + lhs.a[1 * 4 + i] * rhs.y + lhs.a[2 * 4 + i] * rhs.z + lhs.a[3 * 4 + i] * 1;
    return Position(b[0], b[1], b[2]);
}

std::string CurrentDirectory()
{

    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return "[Can't find Current Directory]";
    }
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    return std::string(cCurrentPath);
}

int mt19937Rand(int low, int hi)
{
    std::random_device rd;
    auto dice_rand = std::bind(std::uniform_int_distribution<int>(low, hi),
        std::mt19937(rd()));
    return dice_rand();
}

float mt19937Rand(float low, float hi)
{
    std::random_device rd;
    auto dice_rand = std::bind(std::uniform_real_distribution<float>(low, hi),
        std::mt19937(rd()));
    return dice_rand();
}
