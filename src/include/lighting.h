#ifndef LIGHTING_H
#define LIGHTING_H

#include <opengl.h>

struct Material {
    glm::vec4 ambient {};       // 环境光
    glm::vec4 diffuse {};       // 漫反射
    glm::vec4 specular {};      // 镜面反射
    float shininess {};         // 光泽度
};

// 黄金材质
extern const Material MatGold;

// 白银材质
extern const Material MatSilver;

// 青铜材质
extern const Material MatBronze;

struct AmbientLight {
    glm::vec4 ambient {};       // 环境光
    glm::vec4 diffuse {};       // 漫反射
    glm::vec4 specular {};      // 镜面反射
};

struct DirectLight : virtual AmbientLight {
    glm::vec3 direction {}; // 定向光方向
    DirectLight() = default;
    DirectLight(const AmbientLight& amb, const glm::vec3& dir): AmbientLight(amb), direction(dir) {}
};

struct PosLight : virtual AmbientLight {
    glm::vec3 position {};  // 光源位置
    glm::vec3 factor {};    // 衰减因子系数
    PosLight() = default;
    PosLight(const AmbientLight& amb, const glm::vec3& pos, const glm::vec3& fac): AmbientLight(amb), position(pos), factor(fac) {}
    float AttenuationFactor(float distance) const;
};


extern const AmbientLight GlobalAmbientWhite;
extern const DirectLight DirLightRed;
extern const PosLight PosLightRed;



#endif //LIGHTING_H
