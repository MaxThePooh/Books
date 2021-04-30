//
// Created by Max Roschupkin on 31.12.2020.
//

#include "List.hpp"
#if __has_cpp_attribute(nodiscard)
#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif //NODISCARD
#else
#define NODISCARD
#endif //__has_cpp_attribute(nodiscard)

template<class T>
T &List<T>::iterator::operator*() {
    return pointer->data;
}

template<class T>
typename List<T>::iterator &List<T>::iterator::operator++() {
    pointer=pointer->next;
    return *this;
}

template<class T>
typename List<T>::iterator &List<T>::iterator::operator--() {
    pointer=pointer->previous;
    return *this;
}

template<class T>
const typename List<T>::iterator List<T>::iterator::operator++(int) {
    return iterator(pointer=pointer->next);
}

template<class T>
const typename List<T>::iterator List<T>::iterator::operator--(int) {
    return iterator(pointer=pointer->previous);
}

template<class T>
typename List<T>::iterator List<T>::iterator::operator+(List::size_type n) const {
    iterator it=pointer;
    while(n--)
        ++it;
    return it;
}

template<class T>
typename List<T>::iterator List<T>::iterator::operator-(List::size_type n) const {
    iterator it=pointer;
    while(n--)
        --it;
    return it;
}

template<class T>
typename List<T>::iterator &List<T>::iterator::operator=(const List::container &rhs) {
    pointer=rhs;
    return *this;
}

//template<class T>
//typename List<T>::iterator &List<T>::iterator::operator=( List::container *rhs) {
//    pointer=rhs;
//    return *this;
//}

template<class T>
std::size_t List<T>::size() const {
    return distance(first,last);
}

template<class T>
void List<T>::destroy(iterator it) {
    alloc.destroy(it.pointer);
    alloc.deallocate(it.pointer,1);
}

template<class T>
typename List<T>::iterator& List<T>::push_back(const_reference t) {
    iterator it=alloc.allocate(1);//Свежая память


    alloc.construct(it.pointer, t,last.pointer,last->previous);//Заполняем его с последним как следующим
                                                               //И предпоследним как предыдущим

    last->previous->next=it.pointer;  //Связываем новый предпоследний элемент с соседями
    last->previous=it.pointer;



    return it;
}

template<class T>
typename List<T>::iterator  List<T>::pop_back() {
    if(!empty()) {//Если есть что
        iterator WillBePenultimate = last->previous->previous;//Это будет потом предпоследний

        if(last->previous==first)//Если предпоследний это начало то двигаем начало
            first=first->previous;//Всегда становится концом(наверн)
        destroy(last->previous);//Удаляем

        WillBePenultimate->next = last.pointer;//Связываем соседей бывшего элемента
        last->previous = WillBePenultimate.pointer;

        return WillBePenultimate;
    }
    return last;
}

template<class T>
T &List<T>::front() {
    return *first;
}

template<class T>
T &List<T>::back() {
    return last->previous->data;
}

template<class T>
List<T> &List<T>::operator=(const List &rhs) {
    if(rhs!=this)//Ошибка самоочищения
    {
        uncreate();

        create(rhs.begin(),rhs.end());
    }
    return *this;
}


template<class T>
bool List<T>::empty() {
    return first==last;
}

template<class T>
typename List<T>::iterator List<T>::erase( iterator it) {
    iterator next=it+1; //Элемент который "будет на этом месте"

    if(it!=last) {
        it->previous->next=next.pointer;   //Предыдущий элемент связывается со следующим пропуская текущий
        next->previous=it->previous;   //Следующий элемент связывается с предыдущим пропуская текущий
        if(it==first)   //Двигаем первого если надо
            first=first->next;
        destroy(it);
    }

    return next;
}

template<class T>
typename List<T>::iterator List<T>::erase( iterator f,  iterator l) {
    iterator it = f;
    int count = 0;
    while (it++ != l)
        count++;
    if (count <= size())
        throw std::invalid_argument("Invalid erase argument");

    it=f;

    (f - 1)->next = l.pointer;      //Сразу связываем то что должно быть в итоге
    l->previous = (f - 1).pointer;
    while (it != l)//Пока не дойдем удаляем по элементы
    {
        if (it == first)//Если это начало то двигаем
            first = first->next;
    destroy(it++);
    }
    return it;
}

template<class T>
typename List<T>::iterator List<T>::emplace( iterator it,const T& t) {
    iterator element=alloc.allocate(1);//Свежая память
    alloc.construct(element.pointer,    t,it.pointer,it->previous);// Ставим новый элемент перед указанным

    it->previous->next=element.pointer;//У предыдущего элемента новый следующий
    it->previous=element.pointer;//У указанного элемента новый предыдущий
    if(it == first)
        first=first->previous;
    return element;
}


template<class T>
void List<T>::create()
{
    last=first=alloc.allocate(1);
    alloc.construct(first.pointer,  last.pointer,first.pointer);
}
template<class T>
void List<T>::create(std::size_t n, const_reference t) {
    first=alloc.allocate(n+1);
    last=first.pointer+n;
    alloc.construct(first.pointer,  t, first.pointer+1,last.pointer);

    for(iterator it=first.pointer+1;it!=last.pointer;it++)
        alloc.construct(it.pointer, t,it.pointer+1,it.pointer-1);

    alloc.construct(last.pointer, first.pointer, last.pointer-1);
}

template<class T>
void List<T>::create(iterator& i, iterator& j) {
    first=alloc.allocate(j-i+1);
    last=first.pointer+j-i;
    alloc.construct(first.pointer,  i->data,first.pointer+1,last.pointer);

    for(iterator it=i.pointer+1;it!=j;it.pointer++)
        alloc.construct(it.pointer, it->data,it.pointer-1,it.pointer+1);

    alloc.construct(last.pointer,   first.pointer,last.pointer-1);
}

template<class T>
void List<T>::uncreate() {
    while(first!=last)
        destroy(first++);

    destroy(last);
    first=last= nullptr;
}

template<class T>
std::size_t List<T>::distance(iterator i,iterator j) {
    size_type n=0;
    iterator it=i;
    while(it++!=j)
        ++n;
    return n;
}


