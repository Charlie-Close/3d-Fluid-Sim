//
//  triangleCollider.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 30/03/2023.
//

#include<iostream>
#include"triangleCollider.h"

float fract(float a) {
    return a - floor(a);
}

int scaledToIndex(float x, float y, float z) {
    x = fmin(walls[0] * 2 / CellDistance - 1, fmax(0, x));
    y = fmin(walls[1] * 2 / CellDistance - 1, fmax(0, y));
    z = fmin(walls[2] * 2 / CellDistance - 1, fmax(0, z));
    return round(x) + round(y) * ceil(2 * walls[0] / CellDistance) + round(z) * ceil(2 * walls[0] / CellDistance) * ceil(2 * walls[1] / CellDistance);
}

void FindCells(glm::vec3 Coordinates, int Cells[8])
{
    double scaledPosx = (Coordinates[0] + walls[0]) / CellDistance;
    double scaledPosy = (Coordinates[1] + walls[1]) / CellDistance;
    double scaledPosz = (Coordinates[2] + walls[2]) / CellDistance;

    int position = scaledToIndex(scaledPosx, scaledPosy, scaledPosz);
    Cells[0] = position;


    int dx = 0;
    int dy = 0;

    if(fract(scaledPosx + 0.5) > CellDistance / CellWidth and scaledPosx + 1 < NumberOfCells / ceil(4 * walls[1] * walls[2] / pow(CellDistance, 2))){
        Cells[1] = scaledToIndex(scaledPosx + 1, scaledPosy, scaledPosz);
        dx = 1;
    } else if((fract(scaledPosx + 0.5) < 1 - CellDistance / CellWidth) and scaledPosx - 1 > 0) {
        Cells[1] = scaledToIndex(scaledPosx - 1, scaledPosy, scaledPosz);
        dx = -1;
    } else {
        Cells[1] = position;
    }


    if(fract(scaledPosy + 0.5) > CellDistance / CellWidth and scaledPosy + 1 < NumberOfCells / ceil(4 * walls[0] * walls[2] / pow(CellDistance, 2))){
        Cells[2] = scaledToIndex(scaledPosx, scaledPosy + 1, scaledPosz);
        dy = 1;
        Cells[3] = scaledToIndex(scaledPosx + dx, scaledPosy + 1, scaledPosz);
    } else if((fract(scaledPosy + 0.5) < 1 - CellDistance / CellWidth) and scaledPosy - 2 > 0) {
        Cells[2] = scaledToIndex(scaledPosx, scaledPosy - 1, scaledPosz);
        dy = -1;
        Cells[3] = scaledToIndex(scaledPosx + dx, scaledPosy - 1, scaledPosz);
    } else {
        Cells[2] = position;
        Cells[3] = position;
    }


    if(fract(scaledPosz + 0.5) > CellDistance / CellWidth and scaledPosz + 1 < NumberOfCells / ceil(4 * walls[0] * walls[1] / pow(CellDistance, 2))){
        Cells[4] = scaledToIndex(scaledPosx, scaledPosy, scaledPosz + 1);
        dy = 1;
        Cells[5] = scaledToIndex(scaledPosx + dx, scaledPosy, scaledPosz + 1);
        Cells[6] = scaledToIndex(scaledPosx, scaledPosy + dy, scaledPosz + 1);
        Cells[7] = scaledToIndex(scaledPosx + dx, scaledPosy + dy, scaledPosz + 1);
    } else if((fract(scaledPosz + 0.5) < 1 - CellDistance / CellWidth) and scaledPosz - 1 > 0) {
        Cells[4] = scaledToIndex(scaledPosx, scaledPosy, scaledPosz - 1);
        dy = -1;
        Cells[5] = scaledToIndex(scaledPosx + dx, scaledPosy, scaledPosz - 1);
        Cells[6] = scaledToIndex(scaledPosx, scaledPosy + dy, scaledPosz - 1);
        Cells[7] = scaledToIndex(scaledPosx + dx, scaledPosy + dy, scaledPosz - 1);
    } else {
        Cells[4] = position;
        Cells[5] = position;
        Cells[6] = position;
        Cells[7] = position;
    }
}

std::vector <int> FindCellsIn(glm::vec3 Coord1, glm::vec3 Coord2)
{
    std::vector <int> Cells;
    glm::vec3 step = glm::normalize(Coord2 - Coord1) * glm::mat3(CellDistance);
    for (glm::vec3 check = Coord1; dot(Coord2 - check, Coord2 - Coord1) > 0; check += step)
    {
        int indCells[8];
        FindCells(check, indCells);
        for (int i = 0; i < 8; i++) {
            if (!std::count(Cells.begin(), Cells.end(), indCells[i]))
            {
                Cells.push_back(indCells[i]);
            }
        }
    }

    int indCells[8];
    FindCells(Coord2, indCells);
    for (int i = 0; i < 8; i++) {
        if (!std::count(Cells.begin(), Cells.end(), indCells[i]))
        {
            Cells.push_back(indCells[i]);
        }
    }
    return Cells;
}

Collider::Collider(glm::vec3 verts[3], glm::vec3 norm, int numb)
{
    Verticies[0] = verts[0];
    Verticies[1] = verts[1];
    Verticies[2] = verts[2];
    Force = glm::vec3(0, 0, 0);
    Normal = norm;
    number = numb;
}

std::vector <int> Collider::FindAllCells()
{
    std::vector <int> Cells = FindCellsIn(Verticies[0], Verticies[1]);
    std::vector <int> Cells2 = FindCellsIn(Verticies[1], Verticies[2]);
    std::vector <int> Cells3 = FindCellsIn(Verticies[2], Verticies[0]);
    Cells.insert(Cells.end(), Cells2.begin(), Cells2.end());
    Cells.insert(Cells.end(), Cells3.begin(), Cells3.end());
    
    return Cells;
}

float Collider::CheckAndCollide(glm::vec3 rayOrigin, glm::vec3 rayVector, float maxT)
{
    const float EPSILON = 0.0000001;
    glm::vec3 vertex0 = Verticies[0];
    glm::vec3 vertex1 = Verticies[1];
    glm::vec3 vertex2 = Verticies[2];
    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = glm::cross(rayVector, edge2);
    a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return 1;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * glm::dot(s, h);
    if (u < 0.0 || u > 1.0)
        return 1;
    q = glm::cross(s, edge1);
    v = f * glm::dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return 1;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON and t <= maxT + EPSILON) // ray intersection
    {
        return t;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return 1;
}
