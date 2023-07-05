#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "Node.h"
using namespace std;

template<class T>
class LinkedList {
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const T arr[], int arrSize);
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;

    int getNumberOfNodes() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    void insertAtTheHead(const T &data);
    void insertAtTheTail(const T &data);
    void insertSorted(const T &data);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();

    void print() const;

    T *toArray() const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    Node<T> *tail;
};

template<class T>
LinkedList<T>::LinkedList() {
    /* TODO */
    head = nullptr;
    tail = nullptr;
}

template<class T>
LinkedList<T>::LinkedList(const T arr[], int arrSize) {
    /* TODO */
    head = nullptr;
    tail = nullptr;
    Node<T> *p = head;
    for(int i = 0 ; i < arrSize; i++){
        Node<T> *newnode = new Node<T>(arr[i],nullptr, nullptr);
        if(p){
            p->next = newnode;
            newnode->prev = p;
        }
        else{
            head = newnode;
        }
        p = newnode;
    }
    tail = p;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) {
    /* TODO */
    Node<T> *p_obj = obj.head;
    Node<T> *p_this = nullptr;
    head = nullptr;
    tail = nullptr;
    while(p_obj){
        Node<T> *newnode = new Node<T>(p_obj->data, nullptr, nullptr);
        if(p_this){
            p_this->next = newnode;
            newnode->prev = p_this;
        }
        else{
            head = newnode;
        }
        p_obj = p_obj->next;
        p_this = newnode;
    }
    tail = p_this;
}

template<class T>
LinkedList<T>::~LinkedList() {
    /* TODO */
    Node<T> *p = head,*temp ;
    while(p){
        temp = p->next;
        delete p;
        p = temp;
    }
    head = nullptr;
    tail = nullptr;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const {
    /* TODO */
    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const {
    /* TODO */
    return tail;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const {
    /* TODO */
    Node <T> *p = head;
    while(p){
        if(p->data == data){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

template<class T>
int LinkedList<T>::getNumberOfNodes() const {
    /* TODO */
    int size = 0;
    Node <T> *p = head;
    while(p){
        size++;
        p = p->next;
    }
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
    /* TODO */
    return head == nullptr;
}

template<class T>
bool LinkedList<T>::contains(Node<T> *node) const {
    /* TODO */
    Node <T> *p = head;
    while(p) {
        if ((p->next == node->next) && (p->prev == node->prev) && (p->data == node->data)) {
            return true;
        }
        p = p->next;
    }
    return false;
}

template<class T>
void LinkedList<T>::insertAtTheHead(const T &data) {
    /* TODO */
    Node<T> *p = head;
    Node <T> *newnode = new Node<T>(data, nullptr, nullptr);
    if(p){
        newnode->next = p;
        p->prev = newnode;
        head = newnode;
    }
    else{
        newnode->next = p;
        head = newnode;
        tail = newnode;
    }
}

template<class T>
void LinkedList<T>::insertAtTheTail(const T &data) {
    /* TODO */
    Node<T> *p = tail;
    Node <T> *newnode = new Node<T>(data, nullptr, nullptr);
    if(p){
        newnode->prev = p;
        p->next = newnode;
        tail = newnode;
    }
    else{
        newnode->prev = p;
        tail = newnode;
        head = newnode;
    }
}

template<class T>
void LinkedList<T>::insertSorted(const T &data) {
    /* TODO */
    Node <T> *curr = head;
    Node <T> *newnode = new Node<T>(data, nullptr, nullptr);
    while(curr && (curr->data < data)){
        curr = curr->next;
    }
    if(!head){
        head = newnode;
        tail = newnode;
    }
    else if(curr == head){
        newnode->next = curr;
        head = newnode;
        curr->prev = newnode;
    }
    else if(head && !curr ){
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
    }
    else{
        newnode->next = curr;
        newnode->prev = curr->prev;
        curr->prev->next = newnode;
        curr->prev = newnode;
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node) {
    /* TODO */
    Node<T> *p = head;
    while(p){
        if(((p->data == node->data) && (p->next == node->next) && (p->prev == node->prev))){
            break;
        }
        p = p->next;
    }
    if(p && (head == tail)){
        delete p;
        head = nullptr;
        tail = nullptr;
    }
    else if(p && (p == head)){
        head = p->next;
        p->next->prev = nullptr;
        delete p;
    }
    else if(p && (p == tail)){
        tail = p->prev;
        p->prev->next = nullptr;
        delete p;
    }
    else if(p){
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data) {
    /* TODO */
    Node<T> *p = head;
    while(p){
        if(p->data == data){
            break;
        }
        p = p->next;
    }
    if(p && (head == tail)){
        delete p;
        head = nullptr;
        tail = nullptr;
    }
    else if(p && (p == head)){
        head = p->next;
        p->next->prev = nullptr;
        delete p;
    }
    else if(p && (p == tail)){
        tail = p->prev;
        p->prev->next = nullptr;
        delete p;
    }
    else if(p){
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
    }
}

template<class T>
void LinkedList<T>::removeAllNodes() {
    /* TODO */
    Node <T> *p = head,*temp;
    while(p){
        temp = p->next;
        delete p;
        p = temp;
    }
    head = nullptr;
    tail = nullptr;
}

template<class T>
void LinkedList<T>::print() const {
    std::cout << "Printing the linked list ..." << std::endl;

    if (this->isEmpty()) {
        std::cout << "empty" << std::endl;
        return;
    }

    Node<T> *node = this->head;

    while (node) {
        std::cout << *node << std::endl;
        node = node->next;
    }
}

template<class T>
T *LinkedList<T>::toArray() const {
    /* TODO */
    if(isEmpty()){
        return nullptr;
    }
    int size = getNumberOfNodes();
    Node <T> *p = head;
    T *arry = new T[size];
    for(int i = 0 ; i < size ;i++){
        arry[i] = p->data;
        p = p->next;
    }
    return arry;
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) {
    /* TODO */
    if(this != &rhs){
        removeAllNodes();
        Node <T> *p_this = nullptr,*p_rhs = rhs.head,*temp;
        while(p_rhs){
            if(p_this){
                temp = new Node<T>(p_rhs->data, nullptr, nullptr);
                p_this->next = temp;
                temp->prev = p_this;
                p_this = temp;
            }
            else{
                head = new Node<T>(p_rhs->data, nullptr, nullptr);
                p_this = head;
            }
            p_rhs = p_rhs->next;
        }
        tail = p_this;
    }
    return *this;
}


#endif //LINKEDLIST_H
