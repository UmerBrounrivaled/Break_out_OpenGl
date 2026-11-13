#pragma once 

#include <glad/gl.h>

class Texture2D {   //THIS CLASS HANDLES TEXTURE CONFIGURATION
public:
    unsigned int ID;  //Open Gl texture identifier

    unsigned int width;  //texture dimensions in pixels 
    unsigned int height;

    unsigned int internalFormat; //format stored by opengl
    unsigned int imageFormat; //format of source image 

    unsigned int wrapS;  //behaviour outside U/V coordinates
    unsigned int wrapT;
    unsigned int filterMin; //tells how pixels are sampled 
    unsigned int filterMax;

    Texture2D(); //initializes default texture settings

    void Generate(unsigned char* data, unsigned int width,
                  unsigned int height);
    void Bind() const;
};