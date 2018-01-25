#ifndef SUPERVECTOR_H
#define SUPERVECTOR_H

template <typename T>
class SuperVector {
protected:
  T* ptr;
  int size;
  int top;
  void resize(int size);
  void shift(int begin);

public:
//  typedef T* Iterator;

  SuperVector(int n = 0);
  SuperVector(const SuperVector<T>&);
  ~SuperVector();

  inline void push_back(const T&);
  inline T pop_back();
  inline void insert(const T&, const int& pos);
  void remove(const int& pos);
  inline void set(const int& index, const T& value);
  inline void swap(const int& pos1, const int& pos2);
  const T& operator[](const int& pos);
  inline T* getValuePtr(const int& pos) { return &ptr[pos]; }
  int indexOf(T item, const int& firstPos = 0); // возвращает индекс первого элемента, значение которого равно предоставленному, или -1, если такого значения нет.
  inline int getSize() const { return size; }
  inline int getTop() const { return top; }
  inline bool empty() { return top == 0; }
  inline void removeGarbage();
//  Iterator begin() { return ptr; }
//  Iterator end() { return ptr + top; }
};

template <typename T>
SuperVector<T>::SuperVector(int n) :
  size(n) {
  ptr = new T[size];
  top = 0;
}

template <typename T>
SuperVector<T>::SuperVector(const SuperVector<T>& other) :
  size(other.getSize())
{
  ptr = new T[size];
  top = other.getTop();
  for (int i = 0; i < top; i++)
    ptr[i] = other.ptr[i];
}

template <typename T>
SuperVector<T>::~SuperVector() {
  delete[] ptr;
}

template <typename T>
inline void SuperVector<T>::push_back(const T &value) {
  if (top >= size) resize(size + 1);
  ptr[top++] = value;
}

template <typename T>
inline T SuperVector<T>::pop_back() {
  if (empty()) throw "can't pop; vector is empty";
  return ptr[--top];
}

template <typename T>
inline void SuperVector<T>::insert(const T &value, const int& pos) {
  if (pos > top || pos < 0) throw "can't insert; assigned position is out of vector range";
  shift(pos);
  ptr[pos] = value;
}

template <typename T>
void SuperVector<T>::shift(int begin) {
  if (top >= size) resize(size + 1);

  for (int i = top; i > begin; i--)
    ptr[i] = ptr[i - 1];

  top++;
}

template <typename T>
void SuperVector<T>::remove(const int& pos) {
  if (pos > top || pos < 0) throw "can't insert; assigned position is out of vector range";

  for (int i = pos; i < top - 1; i++)
    ptr[i] = ptr[i + 1];

  top--;
}

template <typename T>
inline void SuperVector<T>::set(const int& index, const T& value) {
  if (index >= top || index < 0) throw "assigned position is out of vector range";
  ptr[index] = value;
}

template <typename T>
inline void SuperVector<T>::swap(const int& pos1, const int& pos2) {
  T temp = ptr[pos1];
  ptr[pos1] = ptr[pos2];
  ptr[pos2] = temp;
}

template <typename T>
const T& SuperVector<T>::operator[](const int& pos) {
  if (pos < 0 || pos >= top) throw "assigned position is out of vector range";
  return ptr[pos];
}

template <typename T>
int SuperVector<T>::indexOf(T value, const int& firstPos) {
  for (int i = firstPos; i < size; i++)
    if (ptr[i] == value)
     return i;

  return -1;
}

template <typename T>
inline void SuperVector<T>::removeGarbage() {
  if (size != top)
    resize(top);
}

template <typename T>
void SuperVector<T>::resize(int n_size) {
  if (n_size < top) throw "resize error: n_size < top";

  size = n_size;
  T* ptr2 = new T[size];

  for (int i = 0; i < top; i++)
    ptr2[i] = ptr[i];

  delete[] ptr;
  ptr = ptr2;
}

#endif // SUPERVECTOR_H
