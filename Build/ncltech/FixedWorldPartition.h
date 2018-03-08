//					,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888         Fixed World Partition Class          888  88
//88  888.        ..       04/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"  
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

	int maxNumber = 8;
	float minSize = 5.0f;


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

	void RemoveObjectFromNode(Node* node, PhysicsNode* obj);

	
	//Destroys the tree and its children
	void TerminateTree(Node ** node);

	//Divides a node into 8 children
	void SplitBox(Node * node, std::vector<PhysicsNode*> parentElements);

	//Returns which physics nodes in parentElems are colliding with the AABB of node
	std::vector<PhysicsNode*> FindElementsInNode(Node * node, std::vector<PhysicsNode*> parentElems);

	//Draws the entire world partition
	void DebugDraw() const { if (root) { DebugDraw(root); } }

	//Repartitions the world based on current positions of objects sent
	void RepartitionWorld(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  *elements);
	
	inline std::vector<PhysicsNode*>* GetBigNodes() { if (bigNodes) { return bigNodes; } }

	//Checks if any objects with velocity have moved to a different leaf node and updates the elemet 
	//lists of each node lists
	void UpdateFixedWorldPartition();

	void AddObject(PhysicsNode*);
	void RemovePhysicsObject(PhysicsNode* obj);

	~FixedWorldPartition();

};

