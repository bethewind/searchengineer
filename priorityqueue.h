#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
template <class Object>
class PriorityQueue
{
public:
    PriorityQueue(){}
    //Adds an Object to a PriorityQueue in log(size) time.
    void put(Object element)
    {
        size++;
        heap[size] = element;
        upHeap();
      }
    //
    void append(Object element)
    {
        if(size+1 > maxSize)
        {
            Object* newheap = new Object[maxSize*2 + 1];
            for(int i = 1;i <= maxSize;i++)
            {
                newheap[i] = heap[i];
            }
            delete[] heap;heap = 0;
            heap = newheap;
            maxSize *= 2;

            put(element);
        }
        else
            put(element);
    }
    int getSize(){return size;}
    //Removes and returns the least element of the PriorityQueue in log(size) time.
    Object pop()
    {
        if (size > 0)
        {
          Object result = heap[1];			  // save first value
          heap[1] = heap[size];			  // move last to first
          heap[size] = 0;			  // permit GC of objects
          size--;
          downHeap();				  // adjust heap
          return result;
        } else
          return 0;
    }
    // Returns the least element of the PriorityQueue in constant time.
      Object top()
      {
        if (size > 0)
          return heap[1];
        else
          return 0;
      }
private:
        Object* heap;//array
        int size;
        int maxSize;

        void upHeap()
        {
            int i = size;
            Object node = heap[i];			  // save bottom node
            int j = ((unsigned int)i) >> 1;
            while (j > 0 && lessThan(node, heap[j])) {
              heap[i] = heap[j];			  // shift parents down
              i = j;
              j = ((unsigned int)j) >> 1;
            }
            heap[i] = node;				  // install saved node
          }
        //
        void downHeap() {
            int i = 1;
            Object node = heap[i];			  // save top node
            int j = i << 1;				  // find smaller child
            int k = j + 1;
            if (k <= size && lessThan(heap[k], heap[j])) {
              j = k;
            }
            while (j <= size && lessThan(heap[j], node)) {
              heap[i] = heap[j];			  // shift up child
              i = j;
              j = i << 1;
              k = j + 1;
              if (k <= size && lessThan(heap[k], heap[j])) {
                j = k;
              }
            }
            heap[i] = node;				  // install saved node
          }

    /* Subclass constructors must call this. */
protected:
  void initialize(int maxSize)
  {
      size = 0;
      int heapSize = maxSize + 1;
      heap = new Object[heapSize];
      this->maxSize = maxSize;
  }
  virtual bool lessThan(Object a, Object b)=0;
};

#endif // PRIORITYQUEUE_H
