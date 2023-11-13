//
//  EBO.h
//  OpenGlTest
//
//  Created by Charlie Close on 23/03/2023.
//

#ifndef EBO_h
#define EBO_h

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
    // ID reference of Elements Buffer Object
    GLuint ID;
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(std::vector<GLuint>& indices);
    EBO();
    void setIndices(std::vector<GLuint>& indices);

    // Binds the EBO
    void Bind();
    // Unbinds the EBO
    void Unbind();
    // Deletes the EBO
    void Delete();
};

#endif
