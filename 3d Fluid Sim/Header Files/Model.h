//
//  Model.h
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#ifndef Model_h
#define Model_h

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Mesh.h"
#include"stl_reader.h"
#include"Triangle.h"
#include<string>
#include"triangleCollider.h"
#include"FluidClasses.h"
#include"Settings.h"

class Model
{
public:
    std::vector <Vertex> verts;
    std::vector <GLuint> ind;
    std::vector <Collider*> cols;
    glm::vec3 AverageForce;
    int FrameCount;

    Mesh ModelMesh;
    
    Model(std::string filepath, float rotSize, glm::vec3 rotVec, glm::vec3 colour, std::vector<Cell*> Cells);
    void addAverages();
    void ShowForce();
};


#endif /* Model_h */
