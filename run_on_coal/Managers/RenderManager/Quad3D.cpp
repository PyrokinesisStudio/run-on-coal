#include "stdafx.h"

#include "Managers/RenderManager/Quad3D.h"

#include "Utils/GLBinder.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_DefaultPosition;
extern const glm::quat g_DefaultRotation;

extern const float g_QuadVertexUV[];

}

#define ROC_QUAD3D_VERTEX_COUNT 6U

ROC::Quad3D::Quad3D()
{
    m_position = g_DefaultPosition;
    m_rotation = g_DefaultRotation;
    m_size = glm::vec2(0.f);
    m_matrix = g_IdentityMatrix;

    glGenVertexArrays(1, &m_VAO);
    GLBinder::BindVertexArray(m_VAO);

    glGenBuffers(static_cast<int>(QBI_BufferCount), m_VBO);

    glEnableVertexAttribArray(QBI_Vertex);
    GLBinder::BindArrayBuffer(m_VBO[QBI_Vertex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_QUAD3D_VERTEX_COUNT, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(QBI_Vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(QBI_UV);
    GLBinder::BindArrayBuffer(m_VBO[QBI_UV]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2U * ROC_QUAD3D_VERTEX_COUNT, g_QuadVertexUV, GL_STATIC_DRAW);
    glVertexAttribPointer(QBI_UV, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    std::vector<glm::vec3> l_normals(ROC_QUAD3D_VERTEX_COUNT, glm::vec3(0.f, 0.f, 1.f));
    glEnableVertexAttribArray(QBI_Vertex);
    GLBinder::BindArrayBuffer(m_VBO[QBI_Vertex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*ROC_QUAD3D_VERTEX_COUNT, l_normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(QBI_Vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_vertex) iter.z = 0.f;
}
ROC::Quad3D::~Quad3D()
{
    for(size_t i = 0U; i < QBI_BufferCount; i++) GLBinder::ResetArrayBuffer(m_VBO[i]);
    glDeleteBuffers(static_cast<int>(QBI_BufferCount), m_VBO);

    GLBinder::ResetVertexArray(m_VAO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad3D::SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size)
{
    if(f_pos != m_position || f_rot != m_rotation)
    {
        std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
        std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));

        btTransform l_transform = btTransform::getIdentity();
        btVector3 l_position(m_position.x, m_position.y, m_position.z);
        btQuaternion l_rotation(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
        l_transform.setOrigin(l_position);
        l_transform.setRotation(l_rotation);
        l_transform.getOpenGLMatrix(glm::value_ptr(m_matrix));
    }
    if(f_size != m_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].x = m_vertex[1].x = m_vertex[3].x = -m_size.x / 2.f;
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y / 2.f;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x / 2.f;
        m_vertex[1].y = m_vertex[2].y = m_vertex[4].y = -m_size.y / 2.f;

        GLBinder::BindArrayBuffer(m_VBO[QBI_Vertex]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*ROC_QUAD3D_VERTEX_COUNT, m_vertex);
    }
}

void ROC::Quad3D::Draw()
{
    GLBinder::BindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, ROC_QUAD3D_VERTEX_COUNT);
}
