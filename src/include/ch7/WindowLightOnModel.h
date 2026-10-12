#ifndef WINDOWLIGHTONMODEL_H
#define WINDOWLIGHTONMODEL_H

#include "WindowBase.h"
#include "models.h"
#include "lighting.h"

class WindowLightOnModel : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 2;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees
public:
    WindowLightOnModel(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    void SetupVertices(const float points[], int size, int index);
    void InstallLight(const glm::mat4& vMat);
private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    glm::vec3 camara_ {};
    glm::vec3 init_loc_ {};
    Torus model_ {};
    PosLight light_ {};
    AmbientLight globalAmbient_ {};
    Material mat_ {MatGold};
};

#endif //WINDOWLIGHTONMODEL_H
