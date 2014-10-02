

    #include <stddef.h>
    #include <stdlib.h>
    #include <assert.h>
    #include <iostream>

    template <class T >
    class MinHeap
    {
    public:
        MinHeap(int n = 10000): m_max_size(n), m_cur_size(0)
        {
            m_heap = new T[m_max_size];
            assert(m_heap != NULL);
        }
        ~MinHeap()
        {
            if(m_heap != NULL)
                delete [] m_heap;
        }
        bool Insert(const T & x)
        {
            if(m_cur_size == m_max_size)
                return false; // full
            m_heap[m_cur_size] = x;
            FilterUp(m_cur_size++);
            return true;
        }
        T RemoveMin()
        {
            T x = m_heap[0];
            m_heap[0] = m_heap[m_cur_size-1];
            --m_cur_size;
            FilterDown(0, m_cur_size-1);
            return x;
        }
        T& GetMin()
        {
            return m_heap[0];
        }
        bool IsEmpty() const
        {
            return 0 == m_cur_size;
        }
        bool IsFull() const
        {
            return m_cur_size == m_max_size;
        }
        void Clear()
        {
            m_cur_size = 0;
        }

        void print() const
        {
            int num = 0;
            int line = 1;
            for(int i = 0; i < m_cur_size; ++i)
            {
                std::cout << m_heap[i] << " ";
            }
            std::cout << std::endl;
        }
    private:
        MinHeap(const MinHeap &);
        MinHeap & operator = (MinHeap&);

        // 从下往上调整: @start，开始调整的位置
        void FilterUp(const int start)
        {
            int child = start;
            int parent = (child-1)/2;
            T temp = m_heap[child];
            while(child > 0)
            {
                if(temp >= m_heap[parent]) //T 必须支持 operator >=
                    break;
                else
                {
                    m_heap[child] = m_heap[parent];
                    child = parent;
                    parent = (child-1)/2;

                }
            }

            m_heap[child] = temp;
        }
        // 从上到下调整: @start，开始调整的位置；@end，最后调整的位置
        void FilterDown(const int start, const int end)
        {
            int parent = start;
            int child = start*2 +1;
            T temp = m_heap[parent];
            while(child <= end)
            {
                if(child+1 <= end && m_heap[child] > m_heap[child+1]) //T 必须支持 operator >
                {
                    child++; // 取子节点中，较小的一个
                }
                if(temp <= m_heap[child])
                    break;
                else
                {
                    m_heap[parent] = m_heap[child];
                    parent = child;
                    child = parent*2+1;
                }
            }

            m_heap[parent] = temp;
        }
    private:
        int m_max_size;
        int m_cur_size; // 当前已用的个数，实际为 m_heap[0, m_cur_size-1]
        T* m_heap;
    };



