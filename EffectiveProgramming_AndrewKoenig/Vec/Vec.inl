//
// Created by Max Roschupkin on 21.12.2020.
//

#include "Vec.hpp"

template<class T>
Vec<T>& Vec<T>::operator=(const Vec &rhs) {
    if(rhs!=this)   //Проверка на самоприсваивание, если указатели одинаковы до мы уничтожим оба вектора
    {
        uncreate();
        create(rhs.begin(),rhs.end());  //Копируем из rhs
    }
    return *this;
}

template<class T>
void Vec<T>::push_back(const T &val) {
    if(avail==limit)    //Если дошли до дополнительно выделенной памяти
        grow();         //Увеличиваем вектор
    unchecked_append(val);  //Дополняем значением
}

template<class T>
void Vec<T>::create() {
    data=avail=limit= nullptr;  //Пустой вектор
}

template<class T>
void Vec<T>::create(Vec::size_type n, const T &val) {
    data=alloc.allocate(n); //Выделяем память на n элементов
    limit=avail=data+n; //Устанавливаем указатели на последний элемент
    std::uninitialized_fill(data,limit,val);    //Заполняем все val
}

template<class T>
void Vec<T>::create(Vec::const_iterator i, Vec::const_iterator j) {
    data=alloc.allocate(j-i);   //Выделяем память на n элементов
    limit=avail=std::uninitialized_copy(i,j,data);  //Копируем все элементы с i до j в data
}

template<class T>
void Vec<T>::uncreate() {
    if(data)
    {
        //В обратном порядке разрушаем элементы
        iterator it=avail;
        while(it!=data)
            alloc.destroy(--it);    //Деструктор элемента
        alloc.deallocate(data,limit-data);   //Очищаем память
    }
    data=avail=limit= nullptr;  //Вектор снова пустой
}

template<class T>
void Vec<T>::grow() {
    //При увеличении выделяем вдвое больше памяти
    size_type new_size=std::max(2*(limit-data),ptrdiff_t(2));   //Сравниваем с двойкой (чтобы не присвоить ноль)

    //Если сразу присвоить указателям значения то последующее освобеждение памяти
    //Уничтожит свежеиспеченный вектор
    iterator new_data=alloc.allocate(new_size); //Выделяем память
    iterator new_avail=std::uninitialized_copy(data,avail,new_data);    //Копируем старые элементы в новую область

    //Освобождаем память
    uncreate();

    //Ставим на рельсы
    data=new_data;
    avail=new_avail;
    limit=data+new_size;
}

template<class T>
void Vec<T>::unchecked_append(const T &val) {
    alloc.construct(avail++,val);   //Вызываем конструктор
}

template<class T>
void Vec<T>::clear() {
    while(avail!=data)
        alloc.destroy(--avail);
    avail=data;
}

template<class T>
typename Vec<T>::iterator Vec<T>::erase(Vec::iterator i, Vec::iterator j) { //Сам выдаст исключение
    iterator it=j;
    while(it!=i)    //Уничтожаем все в отрезке
        alloc.destroy(--it);
    it=std::uninitialized_copy(j,avail,i);  //Копируем то что было после
    while(avail!=it)    //Удаляем все что осталось
        alloc.destroy(--avail);
    return it;
}

template<class T>
typename Vec<T>::iterator Vec<T>::erase(Vec::iterator i) {  //Он сам выдаст исключение
    alloc.destroy(i);   //Уничтожаем нужную
    alloc.destroy(std::uninitialized_copy(i+1,avail--,i));  //Копируем те что были после и удаляем последнюю
    return i;
}

template<class T>
bool Vec<T>::empty() {
    return data!=avail;
}

