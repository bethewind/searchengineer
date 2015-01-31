#ifndef LINKQUEUE_H
#define LINKQUEUE_H
#include "priorityqueue.h"
#include "link.h"
class LinkQueue : public PriorityQueue<Link*>
{
public:
    LinkQueue(int size);
    bool lessThan(Link* hitA, Link* hitB);
};

#endif // LINKQUEUE_H
