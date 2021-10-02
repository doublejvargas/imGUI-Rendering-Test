#include "TestTexture2D.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    test::TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0 , 0))),
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
    {
        float positions[] = {
                -50.0f,  -50.0f,   0.0f, 0.0f,     // 0
                 50.0f,  -50.0f,   1.0f, 0.0f,     // 1
                 50.0f,   50.0f,   1.0f, 1.0f,     // 2
                -50.0f,   50.0f,   0.0f, 1.0f      // 3
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

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
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;

        /* position
         * Attrib 0 in the Vertex Array va (see AddBuffer method in VertexArray.cpp) */
        layout.Push<float>(2);
        /* texture coordinates
         * Attrib 1 in the Vertex Array va (see AddBuffer method in VertexArray.cpp) */
        layout.Push<float>(2);
        /* The method call below goes to show that one buffer can contain multiple attributes. */
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        /* Initializing index buffer */
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        /* Initializing shaders and binding shader ("using program")*/
        m_Shader = std::make_unique<Shader>("res/shaders/Texture.shader");
        m_Shader->Bind();

        /* Uniforms. key: do this after creating and binding shaders */
        //shader.SetUniform4f("u_Color", 0.5f, 0.3f, 0.8f, 1.0f);   //Commented out after started using textures.

        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
        m_Texture->Bind();

        m_Shader->SetUniform1i("u_Texture", 0);
    }

    test::TestTexture2D::~TestTexture2D()
    {
    }

    void test::TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void test::TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            /* Notice the order. Right to left. The actual progression of transformations is model -> view -> projection. */
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void test::TestTexture2D::OnImGuiRender()
    {
        {
            ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
    }

}
