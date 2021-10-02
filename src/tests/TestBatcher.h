#pragma once

#include "Test.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include <memory>

namespace test
{
    class TestBatcher : public Test
    {
    public:
        TestBatcher(const float vertices[], const unsigned int indices[]);
        ~TestBatcher();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

        std::unique_ptr<Shader> m_Shader;

        glm::mat4 m_Proj, m_View;
        glm::vec3 m_Translation;

        const float* m_Vertices;
        const unsigned int* m_Indices;
    };
}