//
//  Mesh.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO.setVertex(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO.setIndices(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

Mesh::Mesh()
{
    Mesh::vertices = {};
    Mesh::indices = {};

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO.setVertex(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO.setIndices(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();    
}

void Mesh::Draw
(
    Shader& shader,
    Camera& camera
)
{
    // Bind shader to be able to access uniforms
    shader.Activate();
    VAO.Bind();
    
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
    VAO.Delete();
    EBO.Delete();
    VBO.Delete();
}
