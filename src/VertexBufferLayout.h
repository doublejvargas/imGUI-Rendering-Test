#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

/* This is a struct that will contain the info needed to define our layout for a vertex buffer
 * We will need to know the (1) data type the buffer will hold
 *  (2) the number of components that make up the data
 *  (3) If the data is normalized
 *  (3) the size in bytes of the data
 *   This will correspond mostly with the glVertexArrayAttribs() call*/
struct VertexBufferElement
{
    unsigned int type;      // e.g: GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE
    unsigned int count;     // The number of components. e.g: 3d coordinates would be 3 (x, y , z), etc...
    unsigned char normalized;   // true or false.

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;  // the size in bytes of each vertex as we add attributes. * One buffer can hold many attribs.
public:
    VertexBufferLayout()
        : m_Stride(0) {}

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(true, "Failed!");
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }

};


template<> inline
void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<> inline
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
