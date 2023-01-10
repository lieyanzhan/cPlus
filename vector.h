

template<typename T>
class Vector
{
public:
    // constructor
    Vector()
    {
        // allocate 2 elements
        ReAlloc(2);
    }

    ~Vector(){
        delete[] m_Data;
    }

    void PopBack(){
        if(m_Size > 0){
            m_Size--;
            m_Data[m_Size].~T();
        }
    }

    void Clear(){
        for(int i = 0; i < m_Size;++i) {
            m_Data[i].~T();
        }
        m_Size = 0;
    }

    void PushBack (const T& value) {
        if(m_Size >= m_Capacity){
            ReAlloc(m_Capacity + m_Capacity / 2);
        }
        m_Data[m_Size] = value;
        ++m_Size;
    }

    size_t Size() const {return m_Size;}

    // 将类成员函数声明为const,以表示这个函数不可以修改类中的成员变量。
    const T& operator[] (size_t index) const{
        //assert(index < m_Size);
        return m_Data[index];
    }

    T& operator[] (size_t index) {
        //assert(index < m_Size);
        return m_Data[index];
    }

private:

    void ReAlloc(size_t newCapacity)
    {
        // 1 allocate
        // 2 move elements
        // 3 delete elements
        if (newCapacity < 0) {
            // throw __EXCEPTIONS("newCapacity is illegal!");
            return;
        }
        if (newCapacity < m_Size) {
            m_Size = newCapacity;
        }
        T* new_Data = new T[newCapacity];
        for (int i=0; i<m_Size; ++i) {
            new_Data[i] = std::move(m_Data[i]);
        }
        delete[] m_Data;
        m_Data = new_Data;
        m_Capacity = newCapacity;
    }

    T* m_Data = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
};