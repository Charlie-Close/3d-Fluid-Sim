//
//  main.cpp
//  3d fluid sim
//
//  Created by Charlie Close on 23/03/2023.
//

#include"stl_reader.h"

#include<iostream>
#include"Mesh.h"
#include"Model.h"
#include"FluidClasses.h"
#include <thread>
#include <chrono>

const unsigned int width = 1200;
const unsigned int height = 800;

std::vector <Cell*> Cells;
int ThreadBreaks[NumberOfThreads + 1];
int numObjs = 0;

void ThreadForceCells(int i, bool noCollide, bool noAverage)
{
    int start = ThreadBreaks[i];
    int end = ThreadBreaks[i + 1];

    for (int j = start; j < end; j++) {
        Cells[j]->CheckCollisions(noCollide, noAverage);
    }
}


int main()
{
    std::vector <Vertex> verts;
    std::vector <GLuint> inds;
    verts.push_back(Vertex({glm::vec3(-walls[0], -walls[1], -walls[2]), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)}));
    verts.push_back(Vertex({glm::vec3(-walls[0], -walls[1], walls[2]), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)}));
    verts.push_back(Vertex({glm::vec3(walls[0], -walls[1], walls[2]), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)}));
    verts.push_back(Vertex({glm::vec3(walls[0], -walls[1], -walls[2]), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)}));
    inds.push_back(0);
    inds.push_back(1);
    inds.push_back(2);
    inds.push_back(0);
    inds.push_back(2);
    inds.push_back(3);
    
    int count = 0;
    for (float z = -walls[2]; z < walls[2]; z += CellDistance)
    {
        for (float y = -walls[1]; y < walls[1]; y += CellDistance)
        {
            for (float x = -walls[0]; x < walls[0]; x += CellDistance)
            {
                Cells.push_back(new Cell(glm::vec3(x, y, z)));
                count += 1;
            }
        }
    }
        
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
    GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    //Load GLAD so it configures OpenGL
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, 2 * width, 2 * height);
    
    
    
    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
 
    // Create floor mesh
//    Mesh floor(verts, ind);
    Model Object("StlFiles/Ferrari F1 2021.stl", (2.0 / 3.0) * 3.1415926, glm::vec3(1, -1, -1), glm::vec3(0.8, 0.8, 0.8), Cells);
    
    for (int i = 0; i < NumberOfCells; i++)
    {
        numObjs += Cells[i]->colliders.size() + 4 * MeanDensity;
    }
    
    int curPos = 0;
    int curNum = 0;
    for (int i = 0; i < NumberOfCells; i++) {
        curNum += Cells[i]->colliders.size() + 4 * MeanDensity;
        if (floor((NumberOfThreads) * float(curNum) / float(numObjs)) > curPos)
        {
            curPos += 1;
            ThreadBreaks[curPos] = i;
            
        }
    }
    
    
    Mesh Floor(verts, inds);
    
    
    shaderProgram.Activate();

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    
    
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 60.0f));
    
//    Particle* p = new Particle(glm::vec3(10, 0, 0), glm::vec3(-1, 0, 0));
    Particle* Particles[NumberOfParticles];
    float averageV = 0;
    for (int i = 0; i < NumberOfParticles; i++)
    {
        Particles[i] = new Particle(i);
        averageV += pow(glm::length(Particles[i]->Velocity), 2);
    }
    averageV /= NumberOfParticles;
    averageV = pow(averageV, 0.5);
    std::cout << averageV << std::endl;
    
    bool pPressed = false;
    bool pause = false;
    
    std::vector <std::thread> Threads;
    
    // Main while loop
    count = 0;
    bool noCollide = true;
    bool noAverage = true;
    
    Mesh FlowLines = Mesh();
    bool FlowLine = false;
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);
        
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            if (!pPressed) {
                pPressed = true;
                pause = !pause;
                if (pause) {
//                    FlowLines = FluidLines(Cells);
                    FlowLine = true;
                }
            }
        } else {
            pPressed = false;
        }


        // Draws different meshes
        Object.ModelMesh.Draw(shaderProgram, camera);
        if (!noCollide and !pause) {
            DrawAllCells(Cells, shaderProgram, camera, Object.AverageForce);
        }
        Floor.Draw(shaderProgram, camera);
        if (pause and FlowLine) {
            DrawAllCells(Cells, shaderProgram, camera, glm::vec3(0, 0, 0));
//            FlowLines.Draw(shaderProgram, camera);
        }
        if (!pause) {
            count += 1;
            if (noCollide or noAverage) {
                if (count == 500) {
                    noCollide = false;
                    std::cout << "COLLIDE" << std::endl;
                }
                if (count == 1000) {
                    noAverage = false;
                    std::cout << "AVERAGE" << std::endl;
                }
                std::cout << count << std::endl;
            } else {
                if (count % 100 == 0) {
                    Object.addAverages();
                    std::cout << Object.AverageForce[0] << ", " << Object.AverageForce[1] << ", " << Object.AverageForce[2] << std::endl;
                    count = 1;
                }
            }
            
            for (int i = 0; i < NumberOfParticles; i++)
            {
                std::vector <int> indCells = FindCellsIn(Particles[i]->Coordinates, Particles[i]->Coordinates + Particles[i]->Velocity * glm::mat3(dt));
                for (int j = 0; j < indCells.size(); j++)
                {
                    if (indCells[j] < NumberOfCells and indCells[j] >= 0) {
                        Cells[indCells[j]]->particles.push_back(Particles[i]);
                    }
                }
                Particles[i]->Move();
//                Particles[i]->Draw(shaderProgram, camera);
            }
            for (int i = 0; i < NumberOfThreads; i++) {
                Threads.push_back(std::thread(ThreadForceCells, i, noCollide, noAverage));
            }
            for (int i = 0; i < NumberOfThreads; i++) {
                Threads[i].join();
            }
            Threads.clear();
        }

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
