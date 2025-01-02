/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    ~UGraphModel()
    {
        // TODO
        this->clear();
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (fromNode == 0)
            throw VertexNotFoundException(this->vertex2str(from));
        if (toNode == 0)
            throw VertexNotFoundException(this->vertex2str(to));

        typename AbstractGraph<T>::Edge *edge = fromNode->getEdge(toNode);
        if (edge != 0) {
            fromNode->removeTo(toNode);
            fromNode->connect(toNode, weight);
            if(!fromNode->equals(toNode)) {
                toNode->removeTo(fromNode);
                toNode->connect(fromNode, weight);
            }
        } else {
            fromNode->connect(toNode, weight);
            if(!fromNode->equals(toNode)) {
                toNode->connect(fromNode, weight);
            }
        }
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (fromNode == 0)
            throw VertexNotFoundException(this->vertex2str(from));
        if (toNode == 0)
            throw VertexNotFoundException(this->vertex2str(to));
        
        typename AbstractGraph<T>::Edge* edge = fromNode->getEdge(toNode);
        if(edge == 0) throw EdgeNotFoundException(this->edge2Str(*edge));

        fromNode->removeTo(toNode);
        toNode->removeTo(fromNode);
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *node = this->getVertexNode(vertex);
        if (node == 0)
            throw VertexNotFoundException(this->vertex2str(vertex));
        
        typename AbstractGraph<T>::Iterator it = this->begin();
        while(it != this->end()){
            typename AbstractGraph<T>::VertexNode* curr_node = this->getVertexNode(*it);
            if(curr_node->getEdge(node) != 0){
                curr_node->removeTo(node);
                node->removeTo(curr_node);
            }
            it++;
        }

        this->nodeList.removeItem(node);
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);
        for (int idx = 0; idx < nvertices; idx++)
        {
            graph->add(vertices[idx]);
        }
        for (int idx = 0; idx < nedges; idx++)
        {
            graph->connect(edges[idx].from, edges[idx].to, edges[idx].weight);
        }
        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
