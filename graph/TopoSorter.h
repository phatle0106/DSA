/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "hash/xMap.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"

template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0){
        //TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        //TODO
        return (mode == DFS)? dfsSort(sorted): bfsSort(sorted);
    }

    void bfsIterative(T vertex, xMap<T, bool>& visited, Queue<T>& queue){
        //TODO
        queue.push(vertex);
        
        while(!queue.empty()){
            T current = queue.pop();
            if(!visited.get(current)) visited.get(current) = true;

            for(T neighbor: this->graph->getOutwardEdges(current)){
                if(!visited.get(neighbor)){
                    queue.push(neighbor);
                }
            }
        }
    }

    DLinkedList<T> bfsSort(bool sorted=true){ 
        //TODO 
        int n = this->graph->size();
        xMap<T, bool> visited = xMap<T, bool>(hash_code);
        Queue<T> queue;
        DLinkedList<T> result;

        for(typename AbstractGraph<T>::Iterator it = graph->begin(); it != graph->end(); it++){
            T vertex = *it;
            visited.put(vertex, false);
        }

        for(typename AbstractGraph<T>::Iterator it = graph->begin(); it != graph->end(); it++){
            T vertex = *it;
            if(!visited.get(vertex)) bfsIterative(vertex, visited, queue);
        }

        while(!queue.empty()){
            T vertex = queue.pop();
            if(!result.contains(vertex)) result.add(vertex);
        }

        return result;
    }

    void dfsIterative(T vertex, xMap<T, bool>& visited, Stack<T>& stack){
        //TODO
        stack.push(vertex);
        
        while(!stack.empty()){
            T current = stack.pop();
            if(!visited.get(current)) visited.get(current) = true;

            bool allVisited = true;
            for(T neighbor: graph->getOutwardEdges(current)){
                if(!visited.get(neighbor)){
                    stack.push(neighbor);
                    allVisited = false;
                    break;
                }
            }

            if(allVisited) stack.push(current);
        }
    }

    DLinkedList<T> dfsSort(bool sorted=true){
        //TODO
        int n = graph->size();
        xMap<T, bool> visited = xMap<T, bool>(hash_code);
        Stack<T> stack;
        DLinkedList<T> result;

        for(typename AbstractGraph<T>::Iterator it = graph->begin(); it != graph->end(); it++){
            T vertex = *it;
            visited.put(vertex, false);
        }

        for(typename AbstractGraph<T>::Iterator it = graph->begin(); it != graph->end(); it++){
            T vertex = *it;
            if(!visited.get(vertex)) dfsIterative(vertex, visited, stack);
        }

        while(!stack.empty()){
            T vertex = stack.pop();
            if(!result.contains(vertex)) result.add(vertex);
        }

        return result;
    }

protected:

    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int));
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int));
    DLinkedList<T> listOfZeroInDegrees();

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */

