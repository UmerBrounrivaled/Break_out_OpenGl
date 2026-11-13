#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

#include <iostream>

Texture2D::Texture2D()  //constructor 
    : ID(0),
    //int variableName(0) direct initializes ID to 0
      width(0),
      height(0),
      internalFormat(GL_RGBA),
      imageFormat(GL_RGBA),
      wrapS(GL_REPEAT),
      wrapT(GL_REPEAT),
      filterMin(GL_LINEAR),
      filterMax(GL_LINEAR) {
}


void Texture2D::Generate(  //setting the configuration 
    unsigned char* data,
    unsigned int textureWidth,
    unsigned int textureHeight
) {
    width = textureWidth;
    height = textureHeight;

    glGenTextures(1, &ID);  //creates texture ID
    glBindTexture(GL_TEXTURE_2D, ID); //Selects the texture for configuration

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    glTexImage2D(  //uploads pixels from CPU memory to GPU memory.
        GL_TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        imageFormat,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);  //After Configuration it unbinds 
}


void Texture2D::Bind() const {  // selects the texture for drawing
    glBindTexture(GL_TEXTURE_2D, ID);
}   