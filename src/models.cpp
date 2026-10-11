#include "models.h"
#include "Utils.h"
#include "debug.h"

#include <cmath>
#include <vector>

using namespace std;

std::vector<ModelBase::Vertex> ModelBase::getVertices() const {
    vector<Vertex> serializedVertices {};
    serializedVertices.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedVertices[i] = vertices[indices[i]];
    }
    return serializedVertices;
}

std::vector<glm::vec3> ModelBase::getPositions() const {
    vector<glm::vec3> serializedPositions {};
    serializedPositions.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedPositions[i] = vertices[indices[i]].position;
    }
    return serializedPositions;
}

std::vector<glm::vec2> ModelBase::getTexCoords() const {
    vector<glm::vec2> serializedTexCoords {};
    serializedTexCoords.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedTexCoords[i] = vertices[indices[i]].texCoord;
    }
    return serializedTexCoords;
}

std::vector<glm::vec3> ModelBase::getNormals() const {
    vector<glm::vec3> serializedNormals {};
    serializedNormals.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        serializedNormals[i] = vertices[indices[i]].normal;
    }
    return serializedNormals;
}

int ModelBase::getVertexNum() const {
    return indices.size();
}

const std::vector<ModelBase::Vertex> & ModelBase::getRawVertices() const {
    return vertices;
}

const std::vector<int> & ModelBase::getRawIndices() const {
    return indices;
}

Sphere::Sphere(int prec) {
    init(prec);
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
            auto y = cos(Utils::toRadian(180.0 - i * 180.0 / prec));
            auto x = - cos(Utils::toRadian(j * 360.0 / prec)) * abs(cos(asin(y)));
            auto z = sin(Utils::toRadian(j * 360.0 / prec)) * abs(cos(asin(y)));
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

Torus::Torus(float inner, float outer, int prec) {
    init(inner, outer, prec);
}

void Torus::init(float inner, float outer, int prec) {
    inner = inner <= 0 ? 0.5f : inner;
    outer = outer <= 0 ? 0.2f : outer;
    prec = prec < 3 ? prec = 20 : prec;

    // 先以 (inner, 0, 0) 为圆心构造 xoy平面的圆, 在圆上切出2 * (prec + 1)个顶点, y = 0轴上有两个点, 便于设置纹理坐标
    prec = 2 * (prec + 1);

    vector<Vertex> initCircle {};
    initCircle.resize(prec);
    for (int i = 0; i < prec; ++i) {
        float radian = Utils::toRadian(i * 360.0 / prec);
        // 绕 z 轴旋转
        auto rMat = glm::rotate(glm::mat4(1.0f), radian, {0, 0, 1});
        // 向 x 方向平移 inner 距离
        auto tMat = glm::translate(glm::mat4(1.0f), {inner, 0, 0});
        // 计算顶点位置
        auto pos = tMat * rMat * glm::vec4(outer, 0, 0, 1.0);
        // 计算纹理坐标: s坐标以该点旋转角度设置, 0 -> 1 -> 0 使360度相交处纹理坐标能够连续
        auto tc = 1 - abs(1 - static_cast<float>(i) / prec * 2);
        // 计算法向量
        auto n = rMat * glm::vec4(outer, 0, 0, 1.0);
        initCircle[i] = Vertex {
            .position = {pos.x, pos.y, pos.z},
            .texCoord = {tc, 0},
            .normal = {n.x, n.y, n.z},
        };
    }

    // 每一个点在空间中绕y轴旋转又可以构成一个圆, 相邻圆上的点之间就可以以类似球体的方式组成三角形
    vertices.clear();
    vertices.resize(prec * prec);
    for (int i = 0; i < prec; ++i) {
        const auto& vertex = initCircle[i];
        for (int j = 0; j < prec; ++j) {
            float radian = Utils::toRadian(j * 360.0 / prec);
            // 绕 y 轴旋转
            auto rMat = glm::rotate(glm::mat4(1.0f), radian, {0, 1, 0});
            // 计算顶点位置
            auto pos = rMat * glm::vec4(vertex.position, 1.0f);
            // 计算纹理坐标: t坐标以该点旋转角度设置 0 -> 1 -> 0 使360度相交处纹理坐标能够连续, s坐标保持不变
            auto tc = 1 - abs(1 - static_cast<float>(j) / prec * 2);
            // 计算法向量: 法向量也随点绕y轴旋转
            auto n = rMat * glm::vec4(vertex.normal, 1.0f);

            vertices[i * prec + j] = Vertex {
                .position = {pos.x, pos.y, pos.z},
                .texCoord = {vertex.texCoord.x, tc},
                .normal = {n.x, n.y, n.z},
            };

            LOG_INFO("tex coords: x = %.5f, y = %.5f", vertex.texCoord.x, tc);
        }
    }

    // 计算三角形索引
    // i loop:
    // j loop: j = prec 是一层的最后一个点, 这个点转了360度和第一个点是重合的, 不需要再构建三角形
    indices.clear();
    indices.resize(prec * prec * 6);
    for (int i = 0; i < prec; ++i) {
        for (int j = 0; j < prec; ++j) {
            // 第一个三角形, 第i层的两个顶点 & 第i+1层的一个顶点
            indices[6 * (i * prec + j) + 0] = i * prec + j;
            indices[6 * (i * prec + j) + 1] = i * prec + (j + 1) % prec;
            indices[6 * (i * prec + j) + 2] = (i + 1) % prec * prec + (j + 1) % prec;

            // 第二个三角形, 第i层的一个顶点 & 第i+1层的两个顶点
            indices[6 * (i * prec + j) + 3] = (i + 1) % prec * prec + (j + 1) % prec;
            indices[6 * (i * prec + j) + 4] = (i + 1) % prec * prec + j;
            indices[6 * (i * prec + j) + 5] = i * prec + j;
        }
    }
}

