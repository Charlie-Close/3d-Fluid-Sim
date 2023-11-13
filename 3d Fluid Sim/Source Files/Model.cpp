//
//  Model.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#include"Model.h"

void maxmin(float* max, float* min, float val)
{
    if (val < *min) {
        *min = val;
    }
    if (*max < val) {
        *max = val;
    }
}

Model::Model(std::string filepath, float rotSize, glm::vec3 rotVec, glm::vec3 colour, std::vector<Cell*> Cells)
{
    glm::mat4 transform = glm::rotate(glm::mat4(1), rotSize, rotVec);
    
    try {
      stl_reader::StlMesh <float, unsigned int> mesh (filepath);
    
      float maxx = -10000000000;
      float maxy = -10000000000;
      float maxz = -10000000000;
      float minx = 10000000000;
      float miny = 10000000000;
      float minz = 10000000000;
      for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
            const float* c0 = mesh.tri_corner_coords (itri, 0);
            const float* c1 = mesh.tri_corner_coords (itri, 1);
            const float* c2 = mesh.tri_corner_coords (itri, 2);
          
            glm::vec3 v0 = glm::vec3(glm::vec4(c0[0], c0[1], c0[2], 1) * transform);
            glm::vec3 v1 = glm::vec3(glm::vec4(c1[0], c1[1], c1[2], 1) * transform);
            glm::vec3 v2 = glm::vec3(glm::vec4(c2[0], c2[1], c2[2], 1) * transform);
          
            maxmin(&maxx, &minx, v0[0]);
            maxmin(&maxy, &miny, v0[1]);
            maxmin(&maxz, &minz, v0[2]);
            maxmin(&maxx, &minx, v1[0]);
            maxmin(&maxy, &miny, v1[1]);
            maxmin(&maxz, &minz, v1[2]);
            maxmin(&maxx, &minx, v2[0]);
            maxmin(&maxy, &miny, v2[1]);
            maxmin(&maxz, &minz, v2[2]);
      }
    
      float scalex = 2 * ObjectBox[0] / (maxx - minx);
      float scaley = 2 * ObjectBox[1] / (maxy - miny);
      float scalez = 2 * ObjectBox[2] / (maxz - minz);
      float scale = fmin(fmin(scalex, scaley), scalez);
        
      transform = glm::rotate(glm::mat4(scale), rotSize, rotVec);
      glm::vec3 position = glm::vec3(-scale * (minx + maxx) / 2, 0.05 - walls[1] - miny * scale, -scale * (minz + maxz) / 2);
        
      std::cout << "SCALE: " << scale << std::endl;
      std::cout << miny << std::endl;

      for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
          const float* c0 = mesh.tri_corner_coords (itri, 0);
          const float* c1 = mesh.tri_corner_coords (itri, 1);
          const float* c2 = mesh.tri_corner_coords (itri, 2);
          
          glm::vec3 v0(c0[0], c0[1], c0[2]);
          glm::vec3 v1(c1[0], c1[1], c1[2]);
          glm::vec3 v2(c2[0], c2[1], c2[2]);
          
          glm::vec3 norm = glm::normalize(glm::vec4(glm::cross(v1 - v0, v2 - v0), 1) * transform);
          
          glm::vec3 V1 = glm::vec3(glm::vec4(c0[0], c0[1], c0[2], 1) * transform) + position;
          glm::vec3 V2 = glm::vec3(glm::vec4(c1[0], c1[1], c1[2], 1) * transform) + position;
          glm::vec3 V3 = glm::vec3(glm::vec4(c2[0], c2[1], c2[2], 1) * transform) + position;
          
          verts.push_back(Vertex{glm::vec3(glm::vec4(c0[0], c0[1], c0[2], 1) * transform) + position, norm, colour});
          verts.push_back(Vertex{glm::vec3(glm::vec4(c1[0], c1[1], c1[2], 1) * transform) + position, norm, colour});
          verts.push_back(Vertex{glm::vec3(glm::vec4(c2[0], c2[1], c2[2], 1) * transform) + position, norm, colour});
          
          ind.push_back(GLuint(itri * 3));
          ind.push_back(GLuint(itri * 3 + 1));
          ind.push_back(GLuint(itri * 3 + 2));
        
          glm::vec3 verts[] = {V1, V2, V3};
          Collider* col = new Collider(verts, norm, int(itri));
          
          std::vector <int> intCells = col->FindAllCells();
          
          for (int i = 0; i < intCells.size(); i++)
          {
              Cells[intCells[i]]->colliders.push_back(col);
              cols.push_back(col);
          }
      }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    std::cout << verts.size() << std::endl;
    
    ModelMesh = Mesh(verts, ind);
    
    AverageForce = glm::vec3(0, 0, 0);
    FrameCount = 0;
}

void Model::addAverages()
{
    glm::vec3 total(0, 0, 0);
    for (int i = 0; i < cols.size(); i++)
    {
        total += cols[i]->Force;
        cols[i]->Force = glm::vec3(0, 0, 0);
    }
    AverageForce = (AverageForce * glm::mat3(FrameCount) + total) * glm::mat3(1 / float(FrameCount + 1));
    FrameCount += 1;
}
