#pragma once
#include "interfaces.h"
#include <iostream>

using namespace std;

template <class T>
 class MyQueue: public IQueue<T>
{
private:
    T *QueuePtr;    
    int MaxSize, begin, end, current;     
public:
    MyQueue(int Max);        
    ~MyQueue();             
   virtual void put(const T &NewElem); 
   virtual T take();   
   virtual T get(int i) const; 
   virtual int size() const;  
};


template <class T>
MyQueue<T>::MyQueue(int Max){
	begin=0;
	MaxSize=Max;
	end=0;
	current=0;
    QueuePtr = new T[MaxSize + 1];
}

template <class T>
MyQueue<T>::~MyQueue(){
    delete [] QueuePtr;
}
 
template <class T>
void MyQueue<T>::put(const T &NewElem){
    if (current<MaxSize){
    QueuePtr[end++] = NewElem;
    current++;
    if (end>MaxSize+1)
        end=end-MaxSize - 1;
	}
	else 
		cout<<"queue overflow"<<endl;
}
 
template <class T>
T MyQueue<T>::take(){
    if (current>0){
    T returnValue = QueuePtr[begin++];
    current--;
    if (begin>MaxSize+1)
        begin=begin-MaxSize - 1; 
    return (returnValue);
	}
	else
		cout<<"queue is empty"<<endl;
		return(NULL);	
}
 
template <class T>
T MyQueue<T>::get(int i) const{
	if ((i>=0)&&(i<=MaxSize+1)&&(i<current-1))
		return (QueuePtr[(i+begin)%(MaxSize+1)]);
	else{
		cout<<"wrong number"<<endl;
		return(NULL);
	}
}

template <class T>
int MyQueue<T> :: size() const
{
		return (current);
}
