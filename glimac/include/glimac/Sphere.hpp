#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0)
class Sphere {
    void build(GLfloat radius, GLsizei discLat, GLsizei discLong);

public:
    Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong):
        m_nVertexCount(0) {
        build(radius, discLat, discLong);
    }

    const ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }

    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount;
};
    
}