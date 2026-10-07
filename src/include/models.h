#ifndef MODELS_H
#define MODELS_H

#include "opengl.h"

#include <vector>

class Sphere{

public:
    struct Vertex {
        glm::vec3 position {};
        glm::vec2 texCoord {};
        glm::vec3 normal {};
    };

    explicit Sphere(int prec = 48);
    std::vector<Vertex> getVertices() const;
    std::vector<glm::vec3> getPositions() const;
    std::vector<glm::vec2> getTexCoords() const;
    std::vector<glm::vec3> getNormals() const;
    int getVertexNum() const;

private:
    std::vector<Vertex> vertices {};
    std::vector<int> indices {};
private:
    void init(int prec);
};

class Torus{

public:
    struct Vertex {
        glm::vec3 position {};
        glm::vec2 texCoord {};
        glm::vec3 normal {};
    };

    explicit Torus(float inner = 0.5f, float outer = 0.2f, int prec = 20);
    std::vector<Vertex> getVertices() const;
    std::vector<glm::vec3> getPositions() const;
    std::vector<glm::vec2> getTexCoords() const;
    std::vector<glm::vec3> getNormals() const;
    int getVertexNum() const;

    const std::vector<Vertex>& getRawVertices() const;
    const std::vector<int>& getRawIndices() const;

private:
    std::vector<Vertex> vertices {};
    std::vector<int> indices {};
private:
    void init(float inner, float outer, int prec);
};

#endif //MODELS_H
