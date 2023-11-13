//
//  Texture.h
//  OpenGlTest
//
//  Created by Charlie Close on 24/03/2023.
//

#ifndef Texture_h
#define Texture_h


#include<glad/glad.h>
#include"stb_image.h"
#include"shaderClass.h"

class Texture
{
public:
    GLuint ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    // Assigns a texture unit to a texture
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    // Binds a texture
    void Bind();
    // Unbinds a texture
    void Unbind();
    // Deletes a texture
    void Delete();
};

#endif
