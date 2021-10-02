#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "VertexBufferLayout.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatcher.h"
#include <tests/TestTextureBatch.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Here I specify the version of OpenGL that I want to use.
     * This is done by changing the context of the window:
     * Major version to 3, minor to 2, setting profile to core (prevents use of deprecated functions)
     * and enabling forward compatibility. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core does not provide a default vao at index 0. Compat does. This is why I need to explicitly declare a vao.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout<< "Error!" << std::endl;

    std::cout<< glGetString(GL_VERSION) << std::endl;

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

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true );
    //ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS)); For some reason this returned version 130
    ImGui_ImplOpenGL3_Init("#version 150"); // had to hardcode version 150 since it is first version that supports core profile
    ImGui::StyleColorsDark();

    float positions[] = {
            -60.0f,  -100.0f,   0.18f, 0.6f, 0.96f, 1.0f,
            140.0f,  -100.0f,   0.18f, 0.6f, 0.96f, 1.0f,
            140.0f,   100.0f,   0.18f, 0.6f, 0.96f, 1.0f,
            -60.0f,   100.0f,   0.18f, 0.6f, 0.96f, 1.0f,

            240.0f,  -100.0f,   1.0f, 0.93f, 0.24f, 1.0f,
            440.0f,  -100.0f,   1.0f, 0.93f, 0.24f, 1.0f,
            440.0f,   100.0f,   1.0f, 0.93f, 0.24f, 1.0f,
            240.0f,   100.0f,   1.0f, 0.93f, 0.24f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,

            4, 5, 6, 6, 7, 4
    };


    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    /* The parameter is just a label. The actual type is specified in the template type */
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::TestBatcher>("Batching w/ Color", positions, indices);
    testMenu->RegisterTest<test::TestTextureBatch>("Batching w/ texture");


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        /*Legacy OpenGL -- drawing a triangle
         * very easy (concise) to do, but very limited functionality - basically presets "ON" and "OFF".
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
        */

        /* We use glDrawArrays when we are drawing directly from vertices
         * 1st param is mode: Triangles
         * 2nd param is offset (where to start from) -- we start from first vertex
         * 3rd param is how many vertices to draw -- we draw 3 here (for a triangle). */
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Otherwise, when using an index buffer, we use glDrawElements */

        renderer.Clear();

        /* This seems to be a ImGui specific frame starter*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}