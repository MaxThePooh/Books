//
// Created by Max Roschupkin on 21.12.2020.
//

#ifndef BOOKS_VEC_HPP
#define BOOKS_VEC_HPP


#if __has_cpp_attribute(nodiscard)
#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif //NODISCARD
#else
#define NODISCARD
#endif //__has_cpp_attribute(nodiscard)


#include <memory>
#include <algorithm>

template<class T> class Vec {
public:
    typedef T *iterator;
    typedef const T *const_iterator;
    typedef std::size_t size_type;
    typedef T value_type;
    typedef T &reference;
    typedef const T& const_reference;
    Vec(){ create(); }
    explicit Vec(size_type n,const T& t=T()){ create(n,t); }//Создаем обьект с n элементов равных t
                                                            //Используем explicit чтобы избежать неявного преобразования
                                                            //Иначе Vec<int> v=100; эквивалентно Vec<int> v(100);
                                                            //Особого смысла это не несет, наверно удобно
    Vec(const Vec& v){ create(v.begin, v.end()); }  //Конструктор копирования
    Vec& operator=(const Vec&);
    ~Vec(){ uncreate(); }

    T& operator[](size_type i){ return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    void push_back(const T&);

    /// \
    NODISCARD size_type size() const{ return avail-data; }      //Аттрибут [[nodiscard]] выдает предупреждение
    NODISCARD size_type capacity() const{ return limit-avail; } //Что просто так не стоит вызывать функцию
    NODISCARD size_type max_size() const{ return limit-data; }  //Тоесть нельзя игнорировать возвращаемое значение

    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    bool empty();

    void clear();
    iterator erase(iterator ,iterator);
    iterator erase(iterator);
private:
    iterator data;  //Начальный элемент
    iterator avail; //Последний существующий элемент

    iterator limit; //Конец зарезервированной памяти


    std::allocator<T> alloc;    //allocator позволяет управлять памятью

    void create();
    void create(size_type, const T&);
    void create(const_iterator,const_iterator);

    void uncreate();

    void grow();

    void unchecked_append(const T &);
};

#include "Vec.inl"

#endif //BOOKS_VEC_HPP
