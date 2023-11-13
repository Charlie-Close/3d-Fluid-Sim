//
//  Circle.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 25/03/2023.
//

#include"Shapes.h"
#include <iostream>
#include<cmath>

std::ostream &operator<<(std::ostream &os, Vertex const &m) {
    return os << "Position(" << m.position[0] << ", " << m.position[1] << ", " << m.position[2] << ") - Normal(" << m.normal[0] << ", " << m.normal[1] << ", " << m.normal[2] << ") - Colour(" << m.colour[0] << ", " << m.colour[1] << ", " << m.colour[2] << ")";
}

Mesh Circle(glm::vec3 position, float radius, int subDev)
{
    float s1 = radius / sqrt(1 + pow(((1 + sqrt(5)) / 2), 2));
    float sphi = s1 * (1 + sqrt(5)) / 2;
    
    float Colour[] = {0.8, 0.8, 0.8};
    Triangle StartingTriangles[20] = {
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(sphi, s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-sphi, s1, 0.f) + position, glm::vec3(-s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, s1) + position, glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, position, Colour),
    };
    
    Triangle FinalTris[int(20 * pow(4, subDev))];
    
    for (int i = 0; i < 20; i++) {
        FinalTris[i] = StartingTriangles[i];
    }
    
    for (int j = 0; j < subDev; j++) {        
        Triangle TTris2[int(20 * pow(4, (j + 1)))];
        
        for (int i = 0; i < 20 * pow(4, j); i++) {
            FinalTris[i].Subdivide(&TTris2[i * 4], &TTris2[i * 4 + 1], &TTris2[i * 4 + 2], &TTris2[i * 4 + 3], position, radius);
        }
        for (int i = 0; i < 20 * pow(4, (j + 1)); i++) {
            FinalTris[i] = TTris2[i];
        }
    }
    
    std::vector <GLuint> IndWithReps;
    std::vector <Vertex> VertWithReps;
    
    for (int i = 0; i < 20 * pow(4, subDev); i++){
        IndWithReps.push_back(i * 3);
        IndWithReps.push_back(i * 3 + 1);
        IndWithReps.push_back(i * 3 + 2);
        for (int j = 0; j < 27; j+= 9) {
            VertWithReps.push_back(Vertex({
                glm::vec3(FinalTris[i].Vertices[j], FinalTris[i].Vertices[j + 1], FinalTris[i].Vertices[j + 2]),
                glm::vec3(FinalTris[i].Vertices[j + 3], FinalTris[i].Vertices[j + 4], FinalTris[i].Vertices[j + 5]),
                glm::vec3(FinalTris[i].Vertices[j + 6], FinalTris[i].Vertices[j + 7], FinalTris[i].Vertices[j + 8])}));
        }
    }
    
    Mesh Mesh1(VertWithReps, IndWithReps);
    return Mesh1;
};


void Arrow(glm::vec3 position, glm::vec3 direction, float Colour[], std::vector <GLuint>& Indices, std::vector <Vertex>& Verticies, int* offset)
{
    
    Triangle triangle1(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.05) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.05) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.05) + position,
                       position, Colour);
    Triangle triangle2(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.05) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.05) + position,
                       direction + position,
                       position, Colour);
    Triangle triangle3(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.05) + position,
                       direction + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.05) + position,
                       position, Colour);
    Triangle triangle4(direction + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.05) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.05) + position,
                       position, Colour);

    Triangle TempTriangles[] = { triangle1, triangle2, triangle3, triangle4 };
    
    for (int i = 0; i < 4; i++) {
        Indices.push_back(i * 3 + *offset);
        Indices.push_back(i * 3 + 1 + *offset);
        Indices.push_back(i * 3 + 2 + *offset);
        for (int j = 0; j < 27; j+= 9) {
            Verticies.push_back(Vertex({
                glm::vec3(TempTriangles[i].Vertices[j], TempTriangles[i].Vertices[j + 1], TempTriangles[i].Vertices[j + 2]),
                glm::vec3(TempTriangles[i].Vertices[j + 3], TempTriangles[i].Vertices[j + 4], TempTriangles[i].Vertices[j + 5]),
                glm::vec3(TempTriangles[i].Vertices[j + 6], TempTriangles[i].Vertices[j + 7], TempTriangles[i].Vertices[j + 8])}));
        }
    }
    
    *offset += 12;
}

void SmallArrow(glm::vec3 position, glm::vec3 direction, float Colour[], std::vector <GLuint>& Indices, std::vector <Vertex>& Verticies, int* offset)
{
    
    Triangle triangle1(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.02) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.02) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.02) + position,
                       position, Colour);
    Triangle triangle2(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.02) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.02) + position,
                       direction + position,
                       position, Colour);
    Triangle triangle3(glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), direction)) * glm::mat3(.02) + position,
                       direction + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.02) + position,
                       position, Colour);
    Triangle triangle4(direction + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction)) * glm::mat3(.02) + position,
                       glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), direction)) * glm::mat3(.02) + position,
                       position, Colour);

    Triangle TempTriangles[] = { triangle1, triangle2, triangle3, triangle4 };
    
    for (int i = 0; i < 4; i++) {
        Indices.push_back(i * 3 + *offset);
        Indices.push_back(i * 3 + 1 + *offset);
        Indices.push_back(i * 3 + 2 + *offset);
        for (int j = 0; j < 27; j+= 9) {
            Verticies.push_back(Vertex({
                glm::vec3(TempTriangles[i].Vertices[j], TempTriangles[i].Vertices[j + 1], TempTriangles[i].Vertices[j + 2]),
                glm::vec3(TempTriangles[i].Vertices[j + 3], TempTriangles[i].Vertices[j + 4], TempTriangles[i].Vertices[j + 5]),
                glm::vec3(TempTriangles[i].Vertices[j + 6], TempTriangles[i].Vertices[j + 7], TempTriangles[i].Vertices[j + 8])}));
        }
    }
    
    *offset += 12;
}
