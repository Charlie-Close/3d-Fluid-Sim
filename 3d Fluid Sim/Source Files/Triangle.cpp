//
//  circle.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 25/03/2023.
//

#include"Triangle.h"

Triangle::Triangle()
{
    glm::vec3 vect1 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 vect2 = glm::vec3(1.0f, -1.0f, 1.0f);
    glm::vec3 vect3 = glm::vec3(-1.0f, -1.0f, 1.0f);
    
    
    Norm = glm::normalize(glm::cross(vect1 - vect2, vect1 - vect3));
    Vectors[0] = vect1;
    Vectors[1] = vect2;
    Vectors[2] = vect3;
    
    float Colour[] = {255, 255, 255};
    
    
    GLfloat verticesToAssign[27] = {
        vect1[0], vect1[1], vect1[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2],
        vect2[0], vect2[1], vect2[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2],
        vect3[0], vect3[1], vect3[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2]
    };
    
    for (int i = 0; i < 27; i++) {
        Vertices[i] = verticesToAssign[i];
    }
}

Triangle::Triangle(glm::vec3 vect1, glm::vec3 vect2, glm::vec3 vect3, float Colour[])
{
    Norm = glm::normalize(glm::cross(vect1 - vect2, vect1 - vect3));
    
    Vectors[0] = vect1;
    Vectors[1] = vect2;
    Vectors[2] = vect3;
    
    GLfloat verticesToAssign[27] = {
        vect1[0], vect1[1], vect1[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2],
        vect2[0], vect2[1], vect2[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2],
        vect3[0], vect3[1], vect3[2], Norm[0], Norm[1], Norm[2], Colour[0], Colour[1], Colour[2]
    };
    
    for (int i = 0; i < 27; i++) {
        Vertices[i] = verticesToAssign[i];
    }
}

Triangle::Triangle(glm::vec3 vect1, glm::vec3 vect2, glm::vec3 vect3, glm::vec3 center, float Colour[])
{
    Norm = glm::normalize(glm::cross(vect1 - vect2, vect1 - vect3));
    glm::vec3 cent = (vect1 + vect2 + vect3) * glm::mat3(0.3333);
    
    Vectors[0] = vect1;
    Vectors[1] = vect2;
    Vectors[2] = vect3;
    
    glm::vec3 norm1 = glm::normalize(vect1 - center);
    glm::vec3 norm2 = glm::normalize(vect2 - center);
    glm::vec3 norm3 = glm::normalize(vect3 - center);
    
    if(glm::dot(Norm, cent - center) > 0)
    {
        Norm = Norm * glm::mat3(-1);
    }
    
    GLfloat verticesToAssign[27] = {
        vect1[0], vect1[1], vect1[2], norm1[0], norm1[1], norm1[2], Colour[0], Colour[1], Colour[2],
        vect2[0], vect2[1], vect2[2], norm2[0], norm2[1], norm2[2], Colour[0], Colour[1], Colour[2],
        vect3[0], vect3[1], vect3[2], norm3[0], norm3[1], norm3[2], Colour[0], Colour[1], Colour[2]
    };
    
    for (int i = 0; i < 27; i++) {
        Vertices[i] = verticesToAssign[i];
    }
}

glm::vec3 Scale(glm::vec3 v1, glm::vec3 v2, float scale)
{
    glm::vec3 dist = v1 - v2;
    return(v1 + dist * glm::mat3(scale / glm::length(dist) - 1));
    
}

void Triangle::Subdivide(Triangle* p1, Triangle* p2, Triangle* p3, Triangle* p4, glm::vec3 center, float distance)
{
    glm::vec3 mid1 = Scale((Vectors[0] + Vectors[1]) * glm::mat3(0.5), center, distance);
    glm::vec3 mid2 = Scale((Vectors[1] + Vectors[2]) * glm::mat3(0.5), center, distance);
    glm::vec3 mid3 = Scale((Vectors[2] + Vectors[0]) * glm::mat3(0.5), center, distance);
    
    float Colour[] = {Vertices[6], Vertices[7], Vertices[8]};
    
    Triangle triangle1(Vectors[0], mid1, mid3, center, Colour);
    Triangle triangle2(Vectors[1], mid1, mid2, center, Colour);
    Triangle triangle3(Vectors[2], mid2, mid3, center, Colour);
    Triangle triangle4(mid1, mid2, mid3, center, Colour);
    
    *p1 = triangle1;
    *p2 = triangle2;
    *p3 = triangle3;
    *p4 = triangle4;
}
