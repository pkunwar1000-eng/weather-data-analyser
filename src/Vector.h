#ifndef ICT283_VECTOR_H
#define ICT283_VECTOR_H

	/**
	 * @class Vector
	 * @brief Minimal, generic dynamic-array container with controlled access.
	 *
	 * Vector is a reusable template container. It encapsulates a raw dynamic
	 * array on the heap, grows automatically when full, and exposes access only
	 * through its public methods. The client never resizes the array directly.
	 *
	 * Vector knows nothing about what it stores: it is parameterised on type T
	 * and must remain free of any application-specific (e.g. weather) knowledge
	 * so that it can be reused unchanged in later work.
	 *
	 * Design notes (rationale for the method set):\n
	 * - Vector(), ~Vector(), Vector(const Vector&), operator= form the Rule of 3
	 *   and are mandatory because the class manages heap memory.\n
	 * - Vector(int) pre-sizes the array; used by the data reader to avoid repeated
	 *   reallocations when the approximate item count is known in advance.\n
	 * - Add inserts an item; without it the container cannot be filled.\n
	 * - GetSize reports the item count; the client cannot iterate without it.\n
	 * - operator[] (const and non-const) IS the controlled access that defines the
	 *   container; the const overload allows a const Vector& to be read.\n
	 * - Clear resets the item count for reuse without reallocation.\n
	 * - GetCapacity exposes the current capacity; used by unit tests to confirm
	 *   that automatic resizing occurs.\n
	 *
	 * I/O and algorithms over the data are deliberately NOT methods: they are
	 * provided by non-member helper routines so the container stays minimal and
	 * decoupled. There are no friends and no I/O methods, so the stored data
	 * cannot be corrupted from outside.
	 *
	 * @author Prasanna Kunwar
	 * @version 03
	 * @date 16/06/2026 Started Lab 5 Date class
	 * @date 25/06/2026 Finalised for Assignment 1: safe operator=, documented rationale
	 */
template <class T>
class Vector
{
public:

		/**
		 * @brief Creates an empty Vector with a default starting capacity.
		 */
	Vector();

		/**
		 * @brief Creates an empty Vector with the given starting capacity.
		 *
		 * @param n - starting capacity (values < 1 are treated as 1)
		 */
	explicit Vector(int n);

		/**
		 * @brief Creates a deep copy of another Vector.
		 *
		 * @param other - Vector to copy from
		 */
	Vector(const Vector<T>& other);

		/**
		 * @brief Releases the heap memory used by the Vector.
		 */
	~Vector();

		/**
		 * @brief Assigns another Vector into this one using a deep copy.
		 *
		 * The existing data is preserved until the new copy has been built,
		 * so the Vector is left unchanged if allocation fails.
		 *
		 * @param other - Vector to copy from
		 * @return Vector<T>& - reference to this Vector
		 */
	Vector<T>& operator=(const Vector<T>& other);

		/**
		 * @brief Adds an item to the end of the Vector, growing if needed.
		 *
		 * @param item - item to add
		 */
	void Add(const T& item);

		/**
		 * @brief Resets the item count to zero without releasing capacity.
		 */
	void Clear();

		/**
		 * @brief Returns the number of items currently stored.
		 *
		 * @return int - current number of items
		 */
	int GetSize() const;

		/**
		 * @brief Returns the current capacity of the underlying array.
		 *
		 * @return int - current capacity
		 */
	int GetCapacity() const;

		/**
		 * @brief Gives read and write access to the item at an index.
		 *
		 * @param index - position of the item
		 * @return T& - reference to the item at index
		 */
	T& operator[](int index);

		/**
		 * @brief Gives read-only access to the item at an index.
		 *
		 * @param index - position of the item
		 * @return const T& - const reference to the item at index
		 */
	const T& operator[](int index) const;

private:
		/// Pointer to the dynamic array stored on the heap
	T* m_data;

		/// Number of items currently stored
	int m_size;

		/// Current maximum number of items before a resize is needed
	int m_capacity;

		/**
		 * @brief Builds this Vector as a deep copy of another.
		 *
		 * Assumes this Vector holds no array yet (m_data is not freed here).
		 *
		 * @param other - Vector to copy from
		 */
	void Copy(const Vector<T>& other);

		/**
		 * @brief Doubles the capacity and moves existing items across.
		 */
	void Resize();
};

// Implementation

template <class T>
Vector<T>::Vector()
	: m_data(nullptr), m_size(0), m_capacity(10)
{
	m_data=new T[m_capacity];
}

template <class T>
Vector<T>::Vector(int n)
	: m_data(nullptr), m_size(0), m_capacity(n>0 ? n : 1)
{
	m_data=new T[m_capacity];
}

template <class T>
Vector<T>::Vector(const Vector<T>& other)
	: m_data(nullptr), m_size(0), m_capacity(0)
{
	Copy(other);
}

template <class T>
Vector<T>::~Vector()
{
	delete[] m_data;
	m_data=nullptr;
	m_size=0;
	m_capacity=0;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if(this!=&other)
	{
		T* newData=new T[other.m_capacity];

		for(int i=0;i<other.m_size;i++)
		{
			newData[i]=other.m_data[i];
		}

		delete[] m_data;

		m_data=newData;
		m_size=other.m_size;
		m_capacity=other.m_capacity;
	}

	return *this;
}

template <class T>
void Vector<T>::Add(const T& item)
{
	if(m_size>=m_capacity)
	{
		Resize();
	}

	m_data[m_size]=item;
	m_size++;
}

template <class T>
void Vector<T>::Clear()
{
	m_size=0;
}

template <class T>
int Vector<T>::GetSize() const
{
	return m_size;
}

template <class T>
int Vector<T>::GetCapacity() const
{
	return m_capacity;
}

template <class T>
T& Vector<T>::operator[](int index)
{
	return m_data[index];
}

template <class T>
const T& Vector<T>::operator[](int index) const
{
	return m_data[index];
}

template <class T>
void Vector<T>::Copy(const Vector<T>& other)
{
	m_size=other.m_size;
	m_capacity=other.m_capacity;
	m_data=new T[m_capacity];

	for(int i=0;i<m_size;i++)
	{
		m_data[i]=other.m_data[i];
	}
}

template <class T>
void Vector<T>::Resize()
{
	int newCapacity=m_capacity*2;
	T* newData=new T[newCapacity];

	for(int i=0;i<m_size;i++)
	{
		newData[i]=m_data[i];
	}

	delete[] m_data;
	m_data=newData;
	m_capacity=newCapacity;
}

#endif
