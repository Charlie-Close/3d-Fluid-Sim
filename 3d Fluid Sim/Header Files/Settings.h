//
//  Settings.h
//  OpenGlTest
//
//  Created by Charlie Close on 30/03/2023.
//

#ifndef Settings_h
#define Settings_h

const int ParticleDetail = 0;
const float CellWidth = 0.5;
const float CellDistance = 0.45;
const glm::vec3 AverageVelocity(-100, 0, 0);
const int VariationVelocity = 20;
const float walls[] = {20, 5, 8};
const float ObjectBox[] = {10, 4, 7};
const int NumberOfParticles = 50000;
const int NumberOfCells = int((2 * walls[0] / CellDistance) * (2 * walls[1] / CellDistance) * (2 * walls[2] / CellDistance));
const float ParticleSize = 0.05;
const float MeanDensity = (NumberOfParticles * pow(CellWidth, 3)) / (7 * walls[0] * walls[1] * walls[2]) ;
const float ArrowScale = 0.003 * (1 / MeanDensity);
const float ForceScale = 0.0000001 / MeanDensity;
const float cSpread = 0.4;
const double k_T = 100;
const float dt = ParticleSize / (k_T * 10);
const int NumberOfThreads = 10;
const float wallCores = 0.8;
const float surfaceCores = 0.9;

#endif /* Settings_h */
