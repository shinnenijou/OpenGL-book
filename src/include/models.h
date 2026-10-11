#ifndef MODELS_H
#define MODELS_H

#include "opengl.h"
#include "Utils.h"

#include <vector>

class ModelBase {
public:
    struct Vertex {
        glm::vec3 position {};
        glm::vec2 texCoord {};
        glm::vec3 normal {};
    };

public:
    ModelBase(): vertices(), indices() {}
    std::vector<Vertex> getVertices() const;
    std::vector<glm::vec3> getPositions() const;
    std::vector<glm::vec2> getTexCoords() const;
    std::vector<glm::vec3> getNormals() const;
    int getVertexNum() const;
    const std::vector<Vertex>& getRawVertices() const;
    const std::vector<int>& getRawIndices() const;
protected:
    std::vector<Vertex> vertices {};
    std::vector<int> indices {};
};

class Sphere : public ModelBase{
public:
    explicit Sphere(int prec = 48);
private:
    void init(int prec);
};

class Torus : public ModelBase {
public:
    explicit Torus(float inner = 0.5f, float outer = 0.2f, int prec = 20);
private:
    void init(float inner, float outer, int prec);
};

class ObjModel: public ModelBase {
public:
    explicit ObjModel(const Utils::path& path);
private:
    void init(const Utils::path& path);
};


#endif //MODELS_H
