#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();    // Returns a vector of VectorBufferElements (see VectorBufferLayout class)
    unsigned int offset = 0; //
    for (unsigned int i = 0; i < elements.size(); i ++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        /* Important: this function defines an array that will make up an attribute for a vertex.
         * The 2nd parameter "size" refers to the number of components that make up the specific attribute, in this case
         *    2 since we have x coord and y coord.
         * The 5th parameter "stride" refers to the amount of bytes between each VERTEX. i.e. the size of each vertex.
         * The 6th parameter "pointer" refers to the offset in bytes of a specific attribute WITHIN the vertex wrt the
         *   beginning of the vertex */
        GLCall(glVertexAttribPointer(i, element.count, element.type,
                element.normalized, layout.GetStride(), (const void*) offset));  // This line of code is what actually links the vao and the vertex buffer!
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
