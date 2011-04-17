#include "kdtree.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include "photonintersection.h"

using std::cout;
using std::endl;

bool xsort(PhotonIntersection* l, PhotonIntersection* r)
{
    return l->position.x() < r->position.x();
}
bool ysort(PhotonIntersection* l, PhotonIntersection* r)
{
    return l->position.y() < r->position.y();
}
bool zsort(PhotonIntersection* l, PhotonIntersection* r)
{
    return l->position.z() < r->position.z();
}

KDTreeNode::KDTreeNode(const Vector3D& pos, const Vector3D& dir)
    : 
      m_leftChild(0),
      m_rightChild(0),
      m_position(pos),
      m_incomingDirection(dir)
{
}

KDTreeNode* KDTreeNode::makeTree(std::vector<PhotonIntersection*>& isecs, 
                                        int depth)
{
    if(isecs.size() == 0)
        return 0;

    int axis = depth % 3;
    if(axis == 0)
        sort(isecs.begin(), isecs.end(), xsort);
    if(axis == 1)
        sort(isecs.begin(), isecs.end(), ysort);
    if(axis == 2)
        sort(isecs.begin(), isecs.end(), zsort);

    // median element
    unsigned int medianIndex = isecs.size()/2;
    PhotonIntersection* median = isecs[medianIndex];

    KDTreeNode* node = new KDTreeNode(median->position, 
                                      median->incomingDirection);
    std::vector<PhotonIntersection*> lefts;//(medianIndex);
    std::vector<PhotonIntersection*> rights;//(medianIndex);
    // Copy everthing smaller than medianIndex to lefts
    // and everything larger than medianIndex to rights
    std::copy(isecs.begin(), isecs.begin() + medianIndex, back_inserter(lefts));
    std::copy(isecs.begin()+medianIndex+1, isecs.end(), back_inserter(rights));
    node->m_leftChild = makeTree(lefts, depth+1);
    node->m_rightChild = makeTree(rights, depth+1);
    return node;
}

void KDTreeNode::dumpTree(int depth)
{
    for(int i=0;i<depth;++i)
        cout << "\t";
    cout << m_position << endl;
    if(m_leftChild)
        m_leftChild->dumpTree(depth+1);
    if(m_rightChild)
        m_rightChild->dumpTree(depth+1);
}

KDTreeNode* KDTreeNode::NNSearch(const Vector3D& point)
{
    KDTreeNode* node = this;
    return _NNSearch(point, node, 0);
}

KDTreeNode* KDTreeNode::_NNSearch(const Vector3D& point, 
                                  KDTreeNode*& currentBest,
                                  int depth)
{
    if(currentBest == 0)
        currentBest = this;
    else if(Vector3D::distanceSquared(m_position, point) <
       Vector3D::distanceSquared(point, currentBest->m_position))
       currentBest = this;

    if(m_leftChild != 0)
        currentBest = m_leftChild->_NNSearch(point, currentBest, depth+1);

    // If distance from currentBest to pointis less than distance
    // to splitting hyperplane plane we can cut of the whole right tree.
    double axisDistanceSq = 0;
    int axis = depth % 3;
    if(axis == 0)
        axisDistanceSq = pow(m_position.x() - point.x(), 2);
    if(axis == 1)
        axisDistanceSq = pow(m_position.y() - point.y(), 2);
    if(axis == 2)
        axisDistanceSq = pow(m_position.z() - point.z(), 2);
    if(axisDistanceSq < Vector3D::distanceSquared(point, currentBest->m_position))
    {
        if(m_rightChild != 0)
            currentBest = m_rightChild->_NNSearch(point, currentBest, depth+1);

    } 
    return currentBest;
}
