#include "TestBatcher.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{

    TestBatcher::TestBatcher(const float vertices[],const unsigned int indices[])
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0 , 0))),
        m_Translation(350, 350, 0),

        m_Vertices(vertices), m_Indices(indices) {
        /* Enabling blender for textures
         *  These could probably be abstracted into the Texture class
         *  How it works is simple: Format of color is RGBAa where a is alpha -- opacity
         *   Blending function uses simple math to compute how colors blend due to transparency.
         *   In this case we use the simple eq: dest = 1 - src
         *   e.g: R_dest = (R_dest * a_dest) + (R_src * (a_src - a_dest))
         *        G_dest = (G_dest * a_dest) + (G_src * (a_src - a_dest))
         *        B_dest = (B_dest * a_dest) + (B_src * (a_src - a_dest))*/
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Initializing Vertex Array and Vertex Buffer) */
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices, 8 * 6 * sizeof(float));
        VertexBufferLayout layout;

        /* position
         * Attrib 0 in the Vertex Array va (see AddBuffer method in VertexArray.cpp) */
        layout.Push<float>(2);
        /* vertex color coordinates
         * Attrib 1 in the Vertex Array va (see AddBuffer method in VertexArray.cpp) */
        layout.Push<float>(4);
        /* The method call below goes to show that one buffer can contain multiple attributes. */
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        /* Initializing index buffer */
        m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices, 12);

        /* Initializing shaders and binding shader ("using program")*/
        m_Shader = std::make_unique<Shader>("res/shaders/BasicBatched.shader");
        m_Shader->Bind();

    }

    TestBatcher::~TestBatcher()
    {

    }

    void TestBatcher::OnUpdate(float deltaTime)
    {
    }

    void TestBatcher::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
            /* Notice the order. Right to left. The actual progression of transformations is model -> view -> projection. */
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }

    void TestBatcher::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_Translation.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}