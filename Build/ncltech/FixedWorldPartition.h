#pragma once

#include "PhysicsNode.h"
#include "CollisionShape.h"
#include "BoundingBox.h"

struct CollisionPair	//Forms the output of the broadphase collision detection
{
	PhysicsNode* pObjectA;
	PhysicsNode* pObjectB;
};

class FixedWorldPartition
{
public:

	struct Node {
		Node * parent = nullptr;
		Node ** children;

		BoundingBox * AABB;

		std::vector<PhysicsNode*> elements;
	};

protected:

	int maxNumber = 3;
	float minSize = 2.0f;


	std::vector<PhysicsNode*> * physicsNodes;
	std::vector<PhysicsNode*> * bigNodes;

	std::vector<PhysicsNode*> updateList;

	//Draws all of the nodes to the screen
	void DebugDraw(Node * node) const;

	//Checks if any moving objects have left or entered partition nodes
	void CheckElements(Node * node);

	//Returns the physics nodes found inside the node and its children
	std::vector<PhysicsNode*> GetElemsInChildren(Node * node);

	//Fuses all of the children of a node
	void CombineNode(Node * parent);

public:

	FixedWorldPartition(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, std::vector<PhysicsNode*>  * elements);
	FixedWorldPartition(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  * elements);
	
	Node * root = nullptr;

	//Sets the smallest size a node can be
	void SetMinSize(float m) { minSize = m; }

	//Returns a vector of all collision pairs within a node and its children
	std::vector<CollisionPair> CreatePairs(Node * node);

	//Destroys the tree and its children
	void TerminateTree(Node ** node);

	//Divides a node into 8 children
	void SplitBox(Node * node, std::vector<PhysicsNode*> parentElements);

	//Returns which physics nodes in parentElems are colliding with the AABB of node
	std::vector<PhysicsNode*> FindElementsInNode(Node * node, std::vector<PhysicsNode*> parentElems);

	std::vector<PhysicsNode*>* GetBigNodes() { return bigNodes; }

	//Draws the entire world partition
	void DebugDraw() const { if (root) { DebugDraw(root); } }

	//Repartitions the world based on current positions of objects sent
	void RepartitionWorld(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  *elements);
	
	inline std::vector<PhysicsNode*>* GetBigNodes() { if (bigNodes) { return bigNodes; } }
	//Checks if any objects with velocity have moved to a different leaf node and updates the elemet 
	//lists of each node lists
	void UpdateFixedWorldPartition();

	~FixedWorldPartition();

};

