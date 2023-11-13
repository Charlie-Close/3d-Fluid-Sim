//
//  FluidClasses.h
//  OpenGlTest
//
//  Created by Charlie Close on 27/03/2023.
//

#ifndef FluidClasses_h
#define FluidClasses_h

#include"Shapes.h"
#include"Settings.h"
#include<iostream>
#include<vector>
#include"triangleCollider.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>
#include<glm/gtc/random.hpp>



class Particle
{
public:
    glm::vec3 Coordinates;
    glm::vec3 Velocity;
    glm::vec3 Acceleration;
    Mesh ModelMesh;
    std::vector <int> Interacted;
    std::vector <int> Bounced;
    int number;
    
    Particle();
    Particle(int numb);
    void Move();
    void Respawn();
    void Draw(Shader& shader, Camera& camera);
    void FindCell(int Cells[8]);
};

class Cell
{
public:
    glm::vec3 Coordinates;
    glm::vec3 AverageVelocity;
    float Colour[3] = {0, 0, 0};
    float Density = 0;
    float FrameCounter = 0;
    std::vector <Collider*> colliders;
    std::vector <Particle*> particles;
    
    Cell();
    Cell(glm::vec3 Position);
    void AddForces();
    void CheckCollisions(bool noCollide, bool noAverage);
    bool IsParticleInside[NumberOfParticles];
    void SetColour();
};

void DrawAllCells(std::vector <Cell*> Cells, Shader& shader, Camera& camera, glm::vec3 Force);

Mesh FluidLines(std::vector <Cell*> Cells);


#endif /* FluidClasses_h */
