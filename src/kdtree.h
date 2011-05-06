#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include "vector3d.h"
#include "raycolor.h"

// Forward declarations
class PhotonIntersection;
// For now 3DTree only
// Todo: Template argument for dimension(also for datatype)
/**
 * \brief Naive and straight-forward kdtree implementation
 *
 */
class KDTreeNode {
public:
    KDTreeNode(const Vector3D& pos, const Vector3D& dir, const RayColor&);

    KDTreeNode* m_leftChild;
    KDTreeNode* m_rightChild;
    const Vector3D& position() { return m_position; }
    const Vector3D& incomingDirection() { return m_incomingDirection; }
    const RayColor& color() { return m_color; }
    static KDTreeNode* makeTree(std::vector<PhotonIntersection*>&, int depth=0);
    void dumpTree(int depth=0);
    /**
     * Performs a Nearest-neighbour seach
     *
     */
    KDTreeNode* NNSearch(const Vector3D& point);
    KDTreeNode* _NNSearch(const Vector3D& point, 
                         KDTreeNode*& currentBest,
                         int depth);

private:
    // TODO: template extra data
    Vector3D m_position;
    Vector3D m_incomingDirection;
    RayColor m_color;

};

#endif
