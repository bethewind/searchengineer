#include "linkqueue.h"

LinkQueue::LinkQueue(int size)
{
    this->initialize(size);
}
//
bool LinkQueue::lessThan(Link *hitA, Link *hitB)
{
    if(hitA->getDepth() == hitB->getDepth())
    {
        return hitA->id > hitB->id;
    }

    return hitA->getDepth() < hitB->getDepth();
}
