#pragma once

#include <vector>
#include <functional>
#include <iostream>

namespace test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}
        /* The virtual function is c++'s way of declaring abstract functions (I think). There
         * is a difference between purely virtual functions and virtual functions. These are virtual functions.
         * eg. of purely virtual would be: virtual void OnUpdate(float deltaTime) = 0;
         * This would require all 3 virtual funcs to be defined in implementation file.
         * However, the cool thing about virtual functions is that they all need not to be implemented */
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };

    /* Class TestMenu inherits from abstract class Test. This is the syntax in c++ to do that. */
    class TestMenu : public Test
    {
    public:
        TestMenu(Test*& CurrentTestPointer);


        void OnImGuiRender() override; // implementation of a virtual function requires the override keyword.

        template<typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering test " << name << std::endl;

            /* The lambda function that is the second argument, stores the function in the second slot of the pair that goes into m_Tests.
             * Then, in function OnImGuiRender (see Test.cpp), when you use the expression test.second() (referring to 2nd element of pair) it calls that stored
             *  function which in turn returns a dynamic (heap) instance of whatever T is. */
            m_Tests.push_back(std::make_pair(name, []() { return new T(); }));    // Lambda function here!
        }

        template<typename T>
        void RegisterTest(const std::string& name, float vertices[], unsigned int indices[])
        {
            std::cout << "Registering test " << name << std::endl;

            /* The lambda function that is the second argument, stores the function in the second slot of the pair that goes into m_Tests.
             * Then, in function OnImGuiRender (see Test.cpp), when you use the expression test.second() (referring to 2nd element of pair) it calls that stored
             *  function which in turn returns a dynamic (heap) instance of whatever T is. */
            m_Tests.push_back(std::make_pair(name, [=]() { return new T(vertices, indices); }));    // Lambda function here!
        }

    private:
        Test*& m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test*()>> > m_Tests;

    };
}