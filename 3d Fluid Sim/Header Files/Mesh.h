//
//  Mesh.h
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#ifndef Mesh_h
#define Mesh_h

#include<string>
#include<vector>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    // Store VAO in public so it can be used in the Draw function
    VAO VAO;
    VBO VBO;
    EBO EBO;

    // Initializes the mesh
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
    Mesh();
    void Delete();

    // Draws the mesh
    void Draw
    (
        Shader& shader,
        Camera& camera
    );
};

#endif /* Mesh_h */
