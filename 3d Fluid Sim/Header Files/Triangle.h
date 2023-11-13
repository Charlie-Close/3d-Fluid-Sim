//
//  circle.h
//  OpenGlTest
//
//  Created by Charlie Close on 25/03/2023.
//

#ifndef Triangle_h
#define Triangle_h

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>


class Triangle
{
public:
    GLfloat Vertices[27];
    glm::vec3 Vectors[3];
    glm::vec3 Norm;
    
    Triangle(glm::vec3 vect1, glm::vec3 vect2, glm::vec3 vect3, glm::vec3 center, float Colour[]);
    Triangle(glm::vec3 vect1, glm::vec3 vect2, glm::vec3 vect3, float Colour[]);
    Triangle();
    
    void Subdivide(Triangle* p1, Triangle* p2, Triangle* p3, Triangle* p4, glm::vec3 center, float distance);
};


glm::vec3 Scale(glm::vec3 v1, glm::vec3 v2, float scale);
#endif /* circle_h */
