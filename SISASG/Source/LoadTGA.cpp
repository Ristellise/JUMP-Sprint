
#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "LoadTGA.h"

GLuint LoadTGA(const char *file_path,GLint MagMinFilter,GLint Clamping) // load TGA file to memory
{
    std::cout << "Loading " << file_path << "... ";
    std::ifstream fileStream(file_path, std::ios::binary);
    if(!fileStream.is_open()) {
        std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?]\n";
        return 0;
    }

    GLubyte        header[ 18 ];                                    // first 6 useful header bytes
    GLuint        bytesPerPixel;                                    // number of bytes per pixel in TGA gile
    GLuint        imageSize;                                        // for setting memory
    GLubyte *    data;
    GLuint        texture = 0;
    unsigned    width, height;

    fileStream.read((char*)header, 18);
    width = header[12] + header[13] * 256;
    height = header[14] + header[15] * 256;

    if(    width <= 0 ||                                // is width <= 0
        height <= 0 ||                                // is height <=0
        (header[16] != 24 && header[16] != 32))        // is TGA 24 or 32 Bit
    {
        fileStream.close();                            // close file on failure
        std::cout << "File header error.\n";
        return 0;                                        
    }

    bytesPerPixel    = header[16] / 8;                        //divide by 8 to get bytes per pixel
    imageSize        = width * height * bytesPerPixel;    // calculate memory required for TGA data
    
    data = new GLubyte[ imageSize ];
    fileStream.seekg(18, std::ios::beg);
    fileStream.read((char *)data, imageSize);
    fileStream.close();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if(bytesPerPixel == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    else //bytesPerPixel == 4
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

    // !!
    // Generate Mipmaps first. then set params.
    // Mistake Ref: https://www.khronos.org/opengl/wiki/Common_Mistakes#Automatic_mipmap_generation
    // !!
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MagMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Clamping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Clamping);

    

    float maxAnisotropy = 1.f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
        (GLint)maxAnisotropy);
    //end of modifiable code
    std::cout << "Loaded TGA.\n";
    delete []data;

    return texture;                        
}