#include "BufferComposer.hpp"

using namespace Commons::Data;

BufferComposer::BufferComposer()
        : mVector(new ConstBufferVector())
        , mOwner(true)
{}

BufferComposer::BufferComposer(ConstBufferVector& vector)
        : mVector(&vector)
        , mOwner(false)
{}

BufferComposer::~BufferComposer()
{
    if (mOwner) // Deletes vector only if owns it
        delete mVector;
}

BufferComposer::ConstBufferVector& BufferComposer::getVector()
{
    return *mVector;
}
