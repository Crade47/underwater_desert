#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
struct CoralPosParams {
    float minX = -18.0f;
    float maxX = 18.0f;
    float minY = -5.0f;
    float maxY = -3.0f;
    float minZ = -18.0f;
    float maxZ = 18.0f;
};

struct FishSchoolParams {
    int numFish = 10;
    float spread = 2;
};

class SceneGenerator
{
public:
    static std::vector<glm::vec3> generateCoralPositions(
        int numCorals,
        CoralPosParams& positions
    );
    static std::vector<glm::vec3> generateFishSchool(
        const glm::vec3& centerPosition,
        FishSchoolParams& params
    );

};

