//
// Created by Max Roschupkin on 28.12.2020.
//
#ifndef BOOKS_LIST_HPP
#define BOOKS_LIST_HPP

#if __has_cpp_attribute(nodiscard)
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

#include <memory>
#include <exception>





template<class T>
class List{
private:
    struct container;
public:
    struct iterator;

    typedef const iterator const_iterator;
    typedef std::size_t size_type;
    typedef T value_type;
    typedef T &reference;
    typedef const T& const_reference;

    List(){ create(); }
    List(const List& list){ create(list.begin(),list.end()); }
    explicit List(size_type n, const_reference t=T()){ create(n,t); }

    ~List(){ uncreate(); }

    iterator& begin(){ return first; }
    const_iterator& begin() const { return first; }
    iterator& end(){ return last; }
    const_iterator& end() const { return last; }

    NODISCARD size_type size() const;

    iterator& push_back(const_reference);
    iterator pop_back();

    T& front();
    T& back();

    bool empty();

    iterator erase( iterator);
    iterator erase( iterator,  iterator);

    iterator emplace(iterator,const T&);

    List& operator=(const List&);

    struct iterator{
        friend List;
    public:
        iterator() = default;
        explicit iterator(const container& rhs) //Container constructor
        : pointer(rhs)
        {}
        /*No explicit*/ iterator(container* rhs) //Pointer container constructor
        : pointer(rhs)
        {}
        explicit iterator(iterator* rhs) //Copy constructor
        : pointer(rhs)
        {}

        T& operator*();
        container* operator->() { return pointer; }
        iterator& operator++();
        iterator& operator--();
        const iterator  operator++(int);
        const iterator  operator--(int);
        iterator operator+(size_type) const;
        iterator operator-(size_type) const;
        iterator& operator=(const container&);
        explicit operator container*(){ return pointer; } //Implicit convert to container*
                                                          //Provide easier code and constructor
        bool operator==(const_iterator it){ return pointer==it.pointer; }
        bool operator!=(const_iterator it){ return pointer!=it.pointer; }
    private:
        container* pointer;
    };

private:
    void create(); //Default filler
    void create(size_type,const_reference t=T()); //Fill with specified number of t
    void create(iterator& ,iterator&); //Copy from another List

    void uncreate(); //Destroy all the data

    size_type distance(iterator,iterator);



    struct container{
        friend List;
        friend iterator;
    public:
        container() = default;
        container(const container& rhs) //Copy constructor
        : data(rhs.data)
        , next(rhs.next)
        , previous(rhs.previous)
        {}
        explicit container(iterator n= nullptr, iterator p= nullptr) //Empty constructor
        : data()
        , next(n)
        , previous(p)
        {}
        explicit container(const_reference t,iterator n= nullptr,iterator p= nullptr)//Constructor
        : data(t)
        , next(n)
        , previous(p)
        {}

    private:
        container* next;
        container* previous;
        T data;
    };


    void destroy(iterator); //Destroys iterator


    iterator first;
    iterator last;
    std::allocator<container> alloc;
};


#include "List.inl"

#endif //BOOKS_LIST_HPP