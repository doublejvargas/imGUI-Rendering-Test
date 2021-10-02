#pragma once

#include "Test.h"

#include "Shader.h"
#include <VertexArray.h>
#include "IndexBuffer.h"
#include "Texture.h"

#include <memory>


namespace test
{
    class TestTextureBatch : public Test
    {
    public:
        TestTextureBatch();
        ~TestTextureBatch();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_TextureA;
        std::unique_ptr<Texture> m_TextureB;

        glm::mat4 m_Proj, m_View;
        glm::vec3 m_Translation;

        float m_Vertices[72] = {
                -60.0f,  -100.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 0.0f,     0.0f,
                140.0f,  -100.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 0.0f,     0.0f,
                140.0f,   100.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 1.0f,     0.0f,
                -60.0f,   100.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 1.0f,     0.0f,

                240.0f,  -100.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 0.0f,     1.0f,
                440.0f,  -100.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 0.0f,     1.0f,
                440.0f,   100.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 1.0f,     1.0f,
                240.0f,   100.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 1.0f,     1.0f
        };

        unsigned int m_Indices[12] = {
                0, 1, 2, 2, 3, 0,

                4, 5, 6, 6, 7, 4
        };
    };

}