#include "lighting.h"
#include <iostream>

const Material MatGold = {
    .ambient = {0.2473f, 0.1995f, 0.0745f, 1},
    .diffuse = {0.7516f, 0.6065f, 0.2265f, 1},
    .specular = {0.6283f, 0.5559f, 0.3661f, 1},
    .shininess = 51.2f,
};

const Material MatSilver = {
    .ambient = {0.1923f, 0.1923f, 0.1923f, 1},
    .diffuse = {0.5075f, 0.5075f, 0.5075f, 1},
    .specular = {0.5083f, 0.5083f, 0.5083f, 1},
    .shininess = 51.2f,
};

const Material MatBronze = {
    .ambient = {0.2125f, 0.1275f, 0.0540f, 1},
    .diffuse = {0.7140f, 0.4284f, 0.1814f, 1},
    .specular = {0.3936f, 0.2719f, 0.1667f, 1},
    .shininess = 25.6f,
};

const AmbientLight GlobalAmbientWhite = {
    .ambient = {0.6f, 0.6f, 0.6f, 1},
    .diffuse = {0, 0, 0, 0},
    .specular = {0, 0, 0, 0},
};

const DirectLight DirLightRed{
    {
        .ambient = {0.1f, 0, 0, 1},
        .diffuse = {1.0f, 0, 0, 1},
        .specular = {1.0f, 0, 0, 1},
    },
    {0, 0, -1},
};

const PosLight PosLightRed{
    {
        .ambient = {0.1f, 0, 0, 1},
        .diffuse = {1.0f, 0, 0, 1},
        .specular = {1.0f, 0, 0, 1},
    },
    {5.0f, 2.0f, -3.0f},
    {2, 1, 1},
};

float PosLight::AttenuationFactor(float distance) const {
    if (distance < 0) {
        return 0;
    }

    const float t = factor.x + factor.y * distance + factor.z * distance * distance;
    if (t <= 0) {
        return 0;
    }

    return 1 / t;
}
