//
//  VBO.h
//  OpenGlTest
//
//  Created by Charlie Close on 23/03/2023.
//

#ifndef VBO_h
#define VBO_h

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>
#include<ostream>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 colour;
};

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(std::vector<Vertex>& vertices);
    VBO();
    void setVertex(std::vector<Vertex>& vertices);

    // Binds the VBO
    void Bind();
    // Unbinds the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};

#endif
