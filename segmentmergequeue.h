#ifndef SEGMENTMERGEQUEUE_H
#define SEGMENTMERGEQUEUE_H
#include "priorityqueue.h"

class SegmentMergeInfo;
class SegmentMergeQueue : public PriorityQueue<SegmentMergeInfo*>
{
public:
    SegmentMergeQueue(int size);
    void close();
protected:
                //Overloaded method that implements the lessThan operator for the parent class
        //This method is used by the parent class Priority queue to reorder its internal
        //data structures. This implementation check if stiA is less than the current term of stiB.
                bool lessThan(SegmentMergeInfo* a, SegmentMergeInfo* b);
};

#endif // SEGMENTMERGEQUEUE_H
