#include "SceneGenerator.h"
#include <random>

std::vector<glm::vec3> SceneGenerator::generateCoralPositions(
    int numCorals, 
    CoralPosParams& pos
)
{
    pos.minY = pos.maxY;
    std::vector<glm::vec3> positions;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create uniform distributions for x, y, and z
    std::uniform_real_distribution<> distX(pos.minX, pos.maxX);
    std::uniform_real_distribution<> distY(pos.minY, pos.maxY);
    std::uniform_real_distribution<> distZ(pos.minZ, pos.maxZ);

    for (int i = 0; i < numCorals; ++i) {
        positions.emplace_back(
            distX(gen),  // Random X position
            distY(gen),  // Random Y position (usually near the bottom)
            distZ(gen)   // Random Z position
        );
    }

    return positions;
  
}

std::vector<glm::vec3> SceneGenerator::generateFishSchool(
    const glm::vec3& centerPosition,
    FishSchoolParams& params
)
{
    std::vector<glm::vec3> positions;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create normal distributions for slight variations
    std::normal_distribution<> distX(0.0, params.spread / 3);
    std::normal_distribution<> distY(0.0, params.spread / 4);
    std::normal_distribution<> distZ(0.0, params.spread / 3);

    for (int i = 0; i < params.numFish; ++i) {
        positions.emplace_back(
            centerPosition.x + distX(gen),
            centerPosition.y + distY(gen),
            centerPosition.z + distZ(gen)
        );
    }

    return positions;
}
