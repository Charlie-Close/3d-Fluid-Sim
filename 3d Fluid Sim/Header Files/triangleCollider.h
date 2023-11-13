//
//  triangleCollider.h
//  OpenGlTest
//
//  Created by Charlie Close on 30/03/2023.
//

#ifndef triangleCollider_h
#define triangleCollider_h

#include <glm/glm.hpp>
#include <vector>
#include"Settings.h"

class Collider
{
public:
    Collider(glm::vec3 verts[3], glm::vec3 norm, int numb);
    std::vector <int> FindAllCells();
    float CheckAndCollide(glm::vec3 rayOrigin, glm::vec3 rayVector, float maxT);
    glm::vec3 Verticies[3];
    glm::vec3 Normal;
    glm::vec3 Force;
    int number;
};

std::vector <int> FindCellsIn(glm::vec3 Coord1, glm::vec3 Coord2);
int scaledToIndex(float x, float y, float z);


#endif /* triangleCollider_h */
