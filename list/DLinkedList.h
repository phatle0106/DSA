/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T> {
 public:
  class Node;         // Forward declaration
  class Iterator;     // Forward declaration
  class BWDIterator;  // Forward declaration

 protected:
  Node *head;
  Node *tail;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(DLinkedList<T> *);

 public:
  DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
              bool (*itemEqual)(T &, T &) = 0);
  DLinkedList(const DLinkedList<T> &list);
  DLinkedList<T> &operator=(const DLinkedList<T> &list);
  ~DLinkedList();

  // Inherit from IList: BEGIN
  void add(T e);
  void add(int index, T e);
  T removeAt(int index);
  bool removeItem(T item, void (*removeItemData)(T) = 0);
  bool empty();
  int size();
  void clear();
  T &get(int index);
  int indexOf(T item);
  bool contains(T item);
  string toString(string (*item2str)(T &) = 0);
  // Inherit from IList: END

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  bool contains(T array[], int size) {
    int idx = 0;
    for (DLinkedList<T>::Iterator it = begin(); it != end(); it++) {
      if (!equals(*it, array[idx++], this->itemEqual)) return false;
    }
    return true;
  }

  Iterator begin() { return Iterator(this, true); }
  Iterator end() { return Iterator(this, false); }

  BWDIterator bbegin() { return BWDIterator(this, true); }
  BWDIterator bend() { return BWDIterator(this, false); }

 protected:
  void copyFrom(const DLinkedList<T> &list);
  void removeInternalData();
  Node *getPreviousNodeOf(int index);

  //! FUNTION STATIC
 public:
  static void free(DLinkedList<T> *list) {
    typename DLinkedList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

 protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

 public:
  class Node {
   public:
    T data;
    Node *next;
    Node *prev;
    friend class DLinkedList<T>;

   public:
    Node(Node *next = 0, Node *prev = 0) {
      this->next = next;
      this->prev = prev;
    }
    Node(T data, Node *next = 0, Node *prev = 0) {
      this->data = data;
      this->next = next;
      this->prev = prev;
    }
  };

 public:
  class Iterator {
   private:
    DLinkedList<T> *pList;
    Node *pNode;

   public:
    Iterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if (begin) {
        if (pList != 0)
          this->pNode = pList->head->next;
        else
          pNode = 0;
      } else {
        if (pList != 0)
          this->pNode = pList->tail;
        else
          pNode = 0;
      }
      this->pList = pList;
    }

    Iterator &operator=(const Iterator &iterator) {
      this->pNode = iterator.pNode;
      this->pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev;  // MUST prev, so iterator++ will go to end
      if (removeItemData != 0) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const Iterator &iterator) {
      return pNode != iterator.pNode;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      pNode = pNode->next;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };

  class BWDIterator {
    // TODO implement
   private:
    DLinkedList<T> *pList;
    Node* pNode;
   public:
    BWDIterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if(begin) {
        if(pList != 0){
          this->pNode = pList->tail->prev;
        } else {
          pNode = 0;
        }
      } else {
        if(pList != 0)
          this->pNode = pList->head;
        else
          this->pNode = 0;
      }
      this->pList = pList;
    }

    BWDIterator &operator=(const BWDIterator &bwdIterator){
      this->pNode = bwdIterator->pNode;
      this->pList = bwdIterator->pList;
      return *this;
    }

    void remove(void (*removeItemData)(T) = 0){
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node* pNext = pNode->next;
      if(removeItemData) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count--;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const BWDIterator bwdIterator){
      return pNode != bwdIterator.pNode;
    }
    BWDIterator &operator--() {
      pNode = pNode->prev;
      return *this;
    }
    BWDIterator operator--(int) {
		  BWDIterator iterator = *this;
		  --*this;
		  return iterator;
	  }
    BWDIterator &operator++() {
      return --*this;
    }
    BWDIterator operator++(int) {
      BWDIterator iterator;
      --*this;
      return iterator;
    }

  };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *),
                            bool (*itemEqual)(T &, T &)) {
  // TODO implement
  this->head = new Node();
  this->tail = new Node();
  this->head->next = this->tail;
  this->tail->prev = this->head;
  this->count = 0;
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;

}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) {
  // TODO implement
  this->head = new Node();
  this->tail = new Node();
  this->head->next = this->tail;
  this->tail->prev = this->head;
  this->count = 0;
  this->itemEqual = list.itemEqual;
  this->deleteUserData = list.deleteUserData;
  copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) {
  // TODO implement
  if(this != &list){
    clear();
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
    copyFrom(list);
  }
  return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList() {
  // TODO implement
  clear();
  delete this->head;
  delete this->tail;
}

template <class T>
void DLinkedList<T>::add(T e) {
  // TODO implement
  Node* newNode = new Node(e, this->tail, this->tail->prev);
  this->tail->prev->next = newNode;
  this->tail->prev = newNode;
  this->count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e) {
  // TODO implement
  if(index < 0 || index > count){
    throw std::out_of_range("Index is out of range!");
  }
  // if(index == count){
  //   add(e);
  // } else if (index == 0){
  //   Node* newNode = new Node(e, this->head, this->head->next);
  //   this->head->next->prev = newNode;
  //   this->head->next = newNode;
  //   count++;
  // }
  // else {
  //   Node* prevNode = getPreviousNodeOf(index);
  //   Node* newNode = new Node(e, prevNode->next, prevNode);
  //   prevNode->next->prev = newNode;
  //   prevNode->next = newNode;
  //   count++;
  // }
  // if(this->tail->prev->next == this->tail) cout << "tail" << endl;
  // else cout << "lost tail" << endl;

    if (count == 0 || index == count) {
        add(e);
    }
    else if (index == 0) {
        Node* newNode = new Node(e, head->next, head);
		head->next->prev = newNode;
		head->next = newNode;
		count++;
    } else {
      Node* prevNode = getPreviousNodeOf(index);
      Node* newNode = new Node(e, prevNode->next, prevNode);
      prevNode->next->prev = newNode;
		  prevNode->next = newNode;
		  count++;
	}
}

template <class T>
T DLinkedList<T>::removeAt(int index) {
  // TODO implement
  if(index < 0 || index >= count){
    throw std::out_of_range("Index is out of range!");
  }

  if(count == 1){
    T item = this->head->next->data;
    delete this->head->next;
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
    return item;
  } else {
    Node* prevNode = getPreviousNodeOf(index);
    Node* delNode = prevNode->next;
    T item = delNode->data;
    
    prevNode->next = delNode->next;
    prevNode->next->prev = prevNode;

    delete delNode;
    this->count--;
    return item;
  }
}

template <class T>
bool DLinkedList<T>::empty() {
  // TODO implement
  return count == 0;
}

template <class T>
int DLinkedList<T>::size() {
  // TODO implement
  return count;
}

template <class T>
void DLinkedList<T>::clear() {
  // TODO implement
  Node* current = this->head->next;
  while(current != this->tail){
    Node* delNode = current;
    current = current->next;
    delete delNode;
  }
  this->head->next = this->tail;
  this->tail->prev = this->head;
  this->count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index) {
  // TODO implement
  return getPreviousNodeOf(index)->next->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item) {
  // TODO implement
  Node* current = this->head->next;
  int index = 0;
  while(current != this->tail){
    if(equals(current->data, item, itemEqual)){
      return index;
    }
    index++;
    current = current->next;
  }
  return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) {
  // TODO implement
  int index = indexOf(item);
  if(index == -1) return false;
  else {
    Node* prevNode = getPreviousNodeOf(index);
    Node* delNode = prevNode->next;
    if(removeItemData) removeItemData(delNode->data);
    prevNode->next = delNode->next;
    if(!delNode->next) cout << "null" << endl;
    prevNode->next->prev = prevNode;
    delete delNode;
    this->count--;
    return true;
  }
}

template <class T>
bool DLinkedList<T>::contains(T item) {
  // TODO implement
  return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &)) {
    stringstream ss;
    ss << "[";
    Node* current = this->head->next;
    // cout << "string test1" << endl;
    // while (current != this->tail) {
    //   cout << "string test2" << endl;
    //     if (current != this->head->next) ss << ", ";
    //     if (item2str) {
    //         ss << item2str(current->data);
    //     } else {
    //         ss << current->data;
    //     }
    //     current = current->next;
    // }
    // cout << "string test3" << endl;

  for(int i = 0; i < count; i++){
    if(current != this->head->next) ss << ", ";
    if(item2str){
      ss << item2str(current->data);
    } else {
      ss << current->data;
    }
    current = current->next;
  }

    ss << "]";
    return ss.str();
}


//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  /**
   * Copies the contents of another doubly linked list into this list.
   * Initializes the current list to an empty state and then duplicates all data
   * and pointers from the source list. Iterates through the source list and
   * adds each element, preserving the order of the nodes.
   */
  // TODO implement
  if(this != &list){
    clear();
    Node* current = list.head->next;
    while(current != list.tail){
      add(current->data);
      current = current->next;
    }
  }
}

template <class T>
void DLinkedList<T>::removeInternalData() {
  /**
   * Clears the internal data of the list by deleting all nodes and user-defined
   * data. If a custom deletion function is provided, it is used to free the
   * user's data stored in the nodes. Traverses and deletes each node between
   * the head and tail to release memory.
   */
  // TODO implement
  clear();
  if(deleteUserData) deleteUserData(this);
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  /**
   * Returns the node preceding the specified index in the doubly linked list.
   * If the index is in the first half of the list, it traverses from the head;
   * otherwise, it traverses from the tail. Efficiently navigates to the node by
   * choosing the shorter path based on the index's position.
   */
  // TODO implement
  if(index < 0 || index >= count){
    throw std::out_of_range("Index is out of range!");
  }
  if(index == 0) return this->head;
  if(index <= count / 2){
    Node* current = this->head->next;
    for(int i = 0; i < index - 1; i++){
      current = current->next;
    }
    return current;
  } else {
    Node* current = this->tail->prev;
    for(int i = count; i > index; i--){
      current = current->prev;
    }
    return current;
  }
}

#endif /* DLINKEDLIST_H */
