#include "models.h"
#include "Utils.h"

#include <cmath>
#include <vector>

using namespace std;

double Sphere::toRadian(const double degree) {
    return degree * M_PI / 180.0f;
}

Sphere::Sphere(int prec) {
    init(prec);
}

std::vector<Sphere::Vertex> Sphere::getVertices() const {
    vector<Vertex> serializedVertices {};
    serializedVertices.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedVertices[i] = vertices[indices[i]];
    }
    return serializedVertices;
}

std::vector<glm::vec3> Sphere::getPositions() const {
    vector<glm::vec3> serializedPositions {};
    serializedPositions.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedPositions[i] = vertices[indices[i]].position;
    }
    return serializedPositions;
}

std::vector<glm::vec2> Sphere::getTexCoords() const {
    vector<glm::vec2> serializedTexCoords {};
    serializedTexCoords.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedTexCoords[i] = vertices[indices[i]].texCoord;
    }
    return serializedTexCoords;
}

std::vector<glm::vec3> Sphere::getNormals() const {
    vector<glm::vec3> serializedNormals {};
    serializedNormals.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedNormals[i] = vertices[indices[i]].normal;
    }
    return serializedNormals;
}

int Sphere::getVertexNum() const {
    return indices.size();
}

void Sphere::init(int prec) {
    if (prec <= 1) {
        prec = 2;
    }

    vertices.clear();
    vertices.resize((prec + 1) * (prec + 1));
    indices.clear();
    indices.resize(prec * prec * 6);

    // 计算顶点
    for (int i = 0; i <= prec; ++i) {
        for (int j = 0; j <= prec; ++j) {
            auto y = cos(toRadian(180.0 - i * 180.0 / prec));
            auto x = - cos(toRadian(j * 360.0 / prec)) * abs(cos(asin(y)));
            auto z = sin(toRadian(j * 360.0 / prec)) * abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = Vertex {
                .position = glm::vec3(x, y, z),
                .texCoord = glm::vec2(static_cast<float>(j) / prec, static_cast<float>(i) / prec),
                .normal = glm::vec3(x, y, z),
            };
        }
    }

    // 计算三角形索引
    // i loop: i = prec 是最后一层, 再上面没有顶点了, 无法再构成三角形
    // j loop: j = prec 是一层的最后一个点, 这个点转了360度和第一个点是重合的, 不需要再构建三角形
    for (int i = 0; i < prec; ++i) {
        for (int j = 0; j < prec; ++j) {
            // 第一个三角形, 第i层的两个顶点 & 第i+1层的一个顶点
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;

            // 第二个三角形, 第i层的一个顶点 & 第i+1层的两个顶点
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

