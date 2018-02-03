#pragma once

#include "PhysicsNode.h"
#include "CollisionShape.h"
#include "BoundingBox.h"

struct CollisionPair	//Forms the output of the broadphase collision detection
{
	PhysicsNode* pObjectA;
	PhysicsNode* pObjectB;
};

class OcTree
{
public:

	struct Node {
		Node * parent = nullptr;
		Node ** children;

		BoundingBox * AABB;

		std::vector<PhysicsNode*> elements;
	};

	Node * root = nullptr;

	OcTree(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, std::vector<PhysicsNode*>  * elements);
	OcTree(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  * elements);
	~OcTree();

	void SetMinSize(float m) { minSize = m; }

	void TerminateTree(Node ** node);

	void SplitBox(Node * node, std::vector<PhysicsNode*> parentElements);
	std::vector<PhysicsNode*> FindElementsInNode(Node * node, std::vector<PhysicsNode*> parentElems);
	void DebugDraw() const { if(root) {DebugDraw(root);} }

	void UpdateOcTree();
	std::vector<CollisionPair> CreatePairs() { return CreatePairs(root); }

	// create box
	// loop through elements and count them (+add to list)
	// return true if there are more elements in the box than the limit allows
	// if this is the case split up the box
	// otherwise, leave it


protected:

	int maxNumber = 8;
	float minSize = 2.0f;

	std::vector<PhysicsNode*> * physicsNodes;
	std::vector<PhysicsNode*> updateList;

	std::vector<CollisionPair> CreatePairs(Node * node);

	void DebugDraw(Node * node) const;
	void CheckElements(Node * node);
	void UpdateNode(Node * node);
	std::vector<PhysicsNode*> GetElemsInChildren(Node * node);
	void CombineNode(Node * parent);
	
};

