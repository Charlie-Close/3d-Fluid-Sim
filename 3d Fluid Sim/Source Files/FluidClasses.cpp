//
//  FluidClasses.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 27/03/2023.
//

#include"FluidClasses.h"
#include <random>
#include <chrono>


unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
std::default_random_engine generator(seed);

// setup the Maxwell distribution, i.e. gamma distribution with alpha = 3/2
std::gamma_distribution<float> maxwell(3./2., k_T);
//pow(maxwell(generator), 2);

glm::vec3 randomMaxwell()
{
    return glm::sphericalRand(maxwell(generator));
}

Particle::Particle()
{
    Respawn();
    number = 0;
}

Particle::Particle(int numb)
{
    Respawn();
    number = numb;
}


void Particle::Draw(Shader& shader, Camera& camera)
{
    ModelMesh = Circle(Coordinates, ParticleSize, 1);
    ModelMesh.Draw(shader, camera);
}

void Particle::Move()
{
    Coordinates += Velocity * glm::mat3(dt);
    Velocity += Acceleration * glm::mat3(dt);
    if (Coordinates[0] < -walls[0] or Coordinates[0] > walls[0])
    {
        Respawn();
    }
    
    if (Coordinates[1] < -walls[1])
    {
        Coordinates[1] = -walls[1] + (-walls[1] - Coordinates[1]);
        Velocity[1] *= -wallCores;
    }
    if (Coordinates[1] > walls[1])
    {
        Coordinates[1] = walls[1] + (walls[1] - Coordinates[1]);
        Velocity[1] *= -wallCores;
    }
    if (Coordinates[2] < -walls[2])
    {
        Coordinates[2] = -walls[2] + (-walls[2] - Coordinates[2]);
        Velocity[2] *= -wallCores;
    }
    if (Coordinates[2] > walls[2])
    {
        Coordinates[2] = walls[2] + (walls[2] - Coordinates[2]);
        Velocity[2] *= -wallCores;
    }
    Interacted.clear();
    Bounced.clear();
}

void Particle::Respawn()
{
    Velocity = randomMaxwell() + AverageVelocity;
    if (Velocity[0] < 0) {
        Coordinates = glm::vec3(walls[0], (float(rand() % 1001) - 500) / (500 / walls[1]), (float(rand() % 1001) - 500) / (500 / walls[2]));
    } else {
        Coordinates = glm::vec3(-walls[0], (float(rand() % 1001) - 500) / (500 / walls[1]), (float(rand() % 1001) - 500) / (500 / walls[2]));
    }
}


Cell::Cell()
{
    Coordinates = glm::vec3(0, 0, 0);
    AverageVelocity = glm::vec3(1, 0, 0);
    Colour[0] = 0.8f;
    FrameCounter = 0;
}

Cell::Cell(glm::vec3 Position)
{
    Coordinates = Position;
    AverageVelocity = glm::vec3(0, 0, 0);
    FrameCounter = 0;
    Colour[0] = 0.8f;
}


void Cell::CheckCollisions(bool noCollide, bool noAverage)
{
    glm::vec3 AV(0, 0, 0);
    float AD = 0;
    if (particles.size() != 0) {
        for (int i = 0; i < particles.size(); i++)
        {
            float minT = 1;
            glm::vec3 particleNewVec = particles[i]->Velocity * glm::mat3(dt);
            int pos = 0;
            
            for (int j = 0; j < colliders.size(); j++)
            {
                float check = colliders[j]->CheckAndCollide(particles[i]->Coordinates, particleNewVec, minT);
                if (check < minT) {
                    minT = check;
                    pos = j;
                }
            }
            if (minT < 1) {
                glm::vec3 Impulse = colliders[pos]->Normal * glm::mat3((1 + surfaceCores) * glm::dot(colliders[pos]->Normal, particles[i]->Velocity));
                particles[i]->Velocity -= Impulse;
                if (!noAverage) {
                    colliders[pos]->Force += Impulse;
                }
            }

            if (!noCollide) {
                for (int j = 0; j < particles.size(); j++)
                {
                    if (j != i and !std::count(particles[i]->Interacted.begin(), particles[i]->Interacted.end(), particles[j]->number))
                    {
                        glm::vec3 direc = particles[i]->Coordinates - particles[j]->Coordinates;
                        float length = glm::length(direc);
                        glm::vec3 norm = glm::normalize(direc);
                        if (length < ParticleSize and length > 0) {
                            glm::vec3 RelV = particles[i]->Velocity - particles[j]->Velocity;
                            glm::vec3 Impulse = norm * glm::mat3(glm::dot(RelV, norm));
                            particles[i]->Velocity += Impulse;
                            particles[i]->Coordinates += norm * glm::mat3(2 * ParticleSize - length);
                            particles[j]->Velocity -= Impulse;
                            particles[j]->Coordinates -= norm * glm::mat3(2 * ParticleSize - length);
                            particles[i]->Interacted.push_back(particles[j]->number);
                            particles[j]->Interacted.push_back(particles[i]->number);
                        }
                    }
                }
            }
            AV += particles[i]->Velocity;
        }
        AV /= particles.size();
    }
    if (!noAverage) {
        AD = particles.size();
        AverageVelocity = (glm::mat3(FrameCounter) * AverageVelocity + AV) * glm::mat3(1 / (FrameCounter+1));
        Density = (FrameCounter * Density + AD) / (FrameCounter + 1);
        FrameCounter += 1;
    }
    
    particles.clear();
}

void Cell::SetColour() {
    float x = Density - MeanDensity;
    Colour[0] = 1 / (exp(-((x / cSpread) - 1)) + 1);
    Colour[1] = 2 / (exp(x / cSpread) + exp(-x / cSpread));
    Colour[2] = 1 / (exp((x / cSpread) + 1) + 1);
}


void DrawAllCells(std::vector <Cell*> Cells, Shader& shader, Camera& camera, glm::vec3 Force)
{
    std::vector <GLuint> Indices;
    std::vector <Vertex> Verticies;
    int offset = 0;
    for (int i = 0; i < NumberOfCells; i++)
    {
        if (Cells[i]->colliders.size() > 0) {
            Cells[i]->SetColour();
            Arrow(Cells[i]->Coordinates, Cells[i]->AverageVelocity * glm::mat3(ArrowScale), Cells[i]->Colour, Indices, Verticies, &offset);
        }
    }
    
    float Colour[3] = {1, 0, 0};
    Arrow(glm::vec3(0, 5, 0), Force * glm::mat3(ForceScale), Colour, Indices, Verticies, &offset);
    
    Mesh Mesh1(Verticies, Indices);
    Mesh1.Draw(shader, camera);
    Mesh1.Delete();
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float k)
{
    return (a + glm::mat3(k) * (b - a));
}

float flerp(float a, float b, float k)
{
    return (a + k * (b - a));
}

Mesh FluidLines(std::vector <Cell*> Cells)
{
    std::vector <GLuint> Indices;
    std::vector <Vertex> Verticies;
    int offset = 0;
    for (float y = -walls[1] + 0.1; y < walls[1] - 4; y += 0.5)
    {
        for (float z = -walls[2] + 1; z < walls[2] - 1; z += 0.5)
        {
            std::cout << "Z: " << z << std::endl;
            glm::vec3 pos(walls[0] - 0.5, y, z);
            int count = 0;
            while (abs(pos[0]) < walls[0] - 0.1 and abs(pos[1]) < walls[1] - 0.1 and abs(pos[2]) < walls[2] - 0.1 and count < 5000)
            {
                count += 1;
                double spx = (pos[0] + walls[0]) / CellDistance - 0.5;
                double spy = (pos[1] + walls[1]) / CellDistance - 0.5;
                double spz = (pos[2] + walls[2]) / CellDistance - 0.5;
                
                int p0 = scaledToIndex(floor(spx), floor(spy), floor(spz));
                int p1 = scaledToIndex(ceil(spx), floor(spy), floor(spz));
                int p2 = scaledToIndex(floor(spx), ceil(spy), floor(spz));
                int p3 = scaledToIndex(ceil(spx), ceil(spy), floor(spz));
                int p4 = scaledToIndex(floor(spx), floor(spy), ceil(spz));
                int p5 = scaledToIndex(ceil(spx), floor(spy), ceil(spz));
                int p6 = scaledToIndex(floor(spx), ceil(spy), ceil(spz));
                int p7 = scaledToIndex(ceil(spx), ceil(spy), ceil(spz));
                
                float xdiff = spx - floor(spx);
                glm::vec3 vel1 = lerp(Cells[p0]->AverageVelocity, Cells[p1]->AverageVelocity, xdiff);
                float dens1 = flerp(Cells[p0]->Density, Cells[p1]->Density, spx - floor(spx));
                glm::vec3 vel2 = lerp(Cells[p2]->AverageVelocity, Cells[p3]->AverageVelocity, xdiff);
                float dens2 = flerp(Cells[p3]->Density, Cells[p3]->Density, spx - floor(spx));
                glm::vec3 vel3 = lerp(Cells[p4]->AverageVelocity, Cells[p5]->AverageVelocity, xdiff);
                float dens3 = flerp(Cells[p4]->Density, Cells[p5]->Density, spx - floor(spx));
                glm::vec3 vel4 = lerp(Cells[p6]->AverageVelocity, Cells[p7]->AverageVelocity, xdiff);
                float dens4 = flerp(Cells[p6]->Density, Cells[p7]->Density, spx - floor(spx));
                
                float ydiff = spy - floor(spy);
                glm::vec3 vel11 = lerp(vel1, vel2, ydiff);
                float dens11 = flerp(dens1, dens2, ydiff);
                glm::vec3 vel12 = lerp(vel3, vel4, ydiff);
                float dens12 = flerp(dens3, dens4, ydiff);
                
                float zdiff = spz - floor(spz);
                glm::vec3 Velocity = lerp(vel11, vel12, zdiff);
                float Density = flerp(dens11, dens12, zdiff);
                
                float x = Density - MeanDensity;
                float Colour[] = {1 / (exp(-((x / cSpread) - 1)) + 1),
                    2 / (exp(x / cSpread) + exp(-x / cSpread)),
                    1 / (exp((x / cSpread) + 1) + 1)};
                
                float dist = 0.8;
                SmallArrow(pos, glm::normalize(Velocity) * glm::mat3(dist), Colour, Indices, Verticies, &offset);
                pos += glm::normalize(Velocity) * glm::mat3(dist);
            }
        }
    }
    
    return Mesh(Verticies, Indices);
}
