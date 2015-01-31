#include "segmentmergequeue.h"
#include "segmentmergeinfo.h"
SegmentMergeQueue::SegmentMergeQueue(int size)
{
     initialize(size);
}
bool SegmentMergeQueue::lessThan(SegmentMergeInfo* a, SegmentMergeInfo* b)
{

    SegmentMergeInfo* stiA = a;
    SegmentMergeInfo* stiB = b;
    int comparison = stiA->term->compareTo(b->term);
    if (comparison == 0)
      return stiA->base < stiB->base;
    else
      return comparison < 0;

}
//
void SegmentMergeQueue::close()
{
    while(top() != 0)
    {
        SegmentMergeInfo* temM = pop();
        temM->close();
    }
}
