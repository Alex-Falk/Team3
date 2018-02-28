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
#include "FixedWorldPartition.h"
#include <algorithm>
#include "MultiGameObject.h"

FixedWorldPartition::FixedWorldPartition(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, std::vector<PhysicsNode*> * elements)
{
	FixedWorldPartition(Vector3(xmin, ymin, zmin), Vector3(xmax, ymax, zmax), elements);
	physicsNodes = nullptr;
}

FixedWorldPartition::FixedWorldPartition(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  * elements)
{
	root = new Node;
	root->AABB = new BoundingBox;
	root->AABB->ExpandToFit(mins);
	root->AABB->ExpandToFit(maxs);
	root->children = nullptr;
	

	physicsNodes = new std::vector<PhysicsNode*>;
	bigNodes = new std::vector<PhysicsNode*>;

	for (std::vector<PhysicsNode*>::iterator itr = elements->begin(); itr != elements->end(); ++itr)
	{
		if ((*itr)->GetType() == BIG_NODE || (*itr)->GetType() == INVISIBLE_WALL)		//Nikos, Added Invisible Walls here so the camera can move pass them But we still want them as big nodes
		{
			bigNodes->push_back((*itr));
		}
		else
		{
			physicsNodes->push_back((*itr));
		}
	}

	if (physicsNodes->size() > maxNumber)
	{
		SplitBox(root, *physicsNodes);
	}
	else
	{
		root->elements = *physicsNodes;
	}
}

FixedWorldPartition::~FixedWorldPartition()
{
	TerminateTree(&root);

	if (physicsNodes)
	{
		delete physicsNodes;
	}
	if (bigNodes)
	{
		delete bigNodes;
	}
	
}

void FixedWorldPartition::TerminateTree(Node ** node) {
	if (*node) 
	{
		if ((*node)->children) 
		{
			for (int i = 0; i < 8; ++i) 
			{
				if ((*node)->children[i]) 
				{
					TerminateTree(&((*node)->children[i])); 
				}
			}
		}

		(*node)->children = nullptr;
		delete (*node);
		(*node) = nullptr;

	}
}

void FixedWorldPartition::RepartitionWorld(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  *elements)
{
	if (root)
	{
		TerminateTree(&root);
	}

	root = new Node;
	root->AABB = new BoundingBox;
	root->AABB->ExpandToFit(mins);
	root->AABB->ExpandToFit(maxs);
	root->children = nullptr;

	if (physicsNodes)
	{
		delete physicsNodes;
	}
	physicsNodes = new std::vector<PhysicsNode*>;
	
	if (bigNodes)
	{
		delete bigNodes;
	}
	bigNodes = new std::vector<PhysicsNode*>;

	for (std::vector<PhysicsNode*>::iterator itr = elements->begin(); itr != elements->end(); ++itr)
	{
		if ((*itr)->GetType() == BIG_NODE)
		{
			bigNodes->push_back((*itr));
		}
		else
		{
			physicsNodes->push_back((*itr));
		}
	}

	if (physicsNodes->size() > maxNumber)
	{
		SplitBox(root, *physicsNodes);
	}
	else
	{
		root->elements = *physicsNodes;
	}
}


void FixedWorldPartition::AddObject(PhysicsNode* newNode)
{
	if (newNode && physicsNodes)
	{
		physicsNodes->push_back(newNode);
	}
}

void FixedWorldPartition::RemovePhysicsObject(PhysicsNode* obj)
{
	if (obj)
	{
		//Lookup the physics node
		auto found_loc = std::find(physicsNodes->begin(), physicsNodes->end(), obj);

		//If found, remove it from the list
		if (found_loc != physicsNodes->end())
		{
			physicsNodes->erase(found_loc);
		}

		if (root)
		{
			RemoveObjectFromNode(root, obj);
		}

	}
}

void FixedWorldPartition::RemoveObjectFromNode(Node* node, PhysicsNode* obj)
{
	if (node)
	{
		if (node->children)
		{
			for (int i = 0; i < 8; ++i)
			{
				RemoveObjectFromNode(node->children[i], obj);
			}
		}

		//Lookup the physics node
		auto found_loc = std::find(node->elements.begin(), node->elements.end(), obj);

		//If found, remove it from the list
		if (found_loc != node->elements.end())
		{
			node->elements.erase(found_loc);
		}
	}
}


void FixedWorldPartition::SplitBox(Node * node, std::vector<PhysicsNode*> parentElements)
{
	Vector3 max = node->AABB->_max;
	Vector3 min = node->AABB->_min;

	Vector3 half = (max + min) * 0.5f;

	node->children = new Node*[8];

	node->children[0] = new Node;
	node->children[0]->parent = node;
	node->children[0]->children = nullptr;
	node->children[0]->AABB = new BoundingBox;
	node->children[0]->AABB->ExpandToFit(min);
	node->children[0]->AABB->ExpandToFit(half);

	node->children[1] = new Node;
	node->children[1]->parent = node;
	node->children[1]->children = nullptr;
	node->children[1]->AABB = new BoundingBox;
	node->children[1]->AABB->ExpandToFit(Vector3(half.x, min.y, min.z));
	node->children[1]->AABB->ExpandToFit(Vector3(max.x, half.y, half.z));

	node->children[2] = new Node;
	node->children[2]->parent = node;
	node->children[2]->children = nullptr;
	node->children[2]->AABB = new BoundingBox;
	node->children[2]->AABB->ExpandToFit(Vector3(min.x, half.y, min.z));
	node->children[2]->AABB->ExpandToFit(Vector3(half.x, max.y, half.z));

	node->children[3] = new Node;
	node->children[3]->parent = node;
	node->children[3]->children = nullptr;
	node->children[3]->AABB = new BoundingBox;
	node->children[3]->AABB->ExpandToFit(Vector3(half.x, half.y, min.z));
	node->children[3]->AABB->ExpandToFit(Vector3(max.x, max.y, half.z));

	node->children[4] = new Node;
	node->children[4]->parent = node;
	node->children[4]->children = nullptr;
	node->children[4]->AABB = new BoundingBox;
	node->children[4]->AABB->ExpandToFit(Vector3(min.x, min.y, half.z));
	node->children[4]->AABB->ExpandToFit(Vector3(half.x, half.y, max.z));

	node->children[5] = new Node;
	node->children[5]->parent = node;
	node->children[5]->children = nullptr;
	node->children[5]->AABB = new BoundingBox;
	node->children[5]->AABB->ExpandToFit(Vector3(half.x, min.y, half.z));
	node->children[5]->AABB->ExpandToFit(Vector3(max.x, half.y, max.z));

	node->children[6] = new Node;
	node->children[6]->parent = node;
	node->children[6]->children = nullptr;
	node->children[6]->AABB = new BoundingBox;
	node->children[6]->AABB->ExpandToFit(Vector3(min.x, half.y, half.z));
	node->children[6]->AABB->ExpandToFit(Vector3(half.x, max.y, max.z));

	node->children[7] = new Node;
	node->children[7]->parent = node;
	node->children[7]->children = nullptr;
	node->children[7]->AABB = new BoundingBox;
	node->children[7]->AABB->ExpandToFit(half);
	node->children[7]->AABB->ExpandToFit(max);

	Vector3 halfsize = (max - min) * 0.25f;

	for (int i = 0; i < 8; ++i) 
	{
		std::vector<PhysicsNode*> childphysnodes = FindElementsInNode(node->children[i], parentElements);

		if (childphysnodes.size() > maxNumber && abs(halfsize.x) >= minSize && abs(halfsize.y) >= minSize && abs(halfsize.z) >= minSize) 
		{
			SplitBox(node->children[i], childphysnodes);
		}
		
		else 
		{
			node->children[i]->elements = childphysnodes;
		}

	}
}

std::vector<PhysicsNode*> FixedWorldPartition::FindElementsInNode(Node * node, std::vector<PhysicsNode*> parentElements)
{
	std::vector<PhysicsNode*> newPhysicsNodes;

	for (size_t i = 0; i < parentElements.size(); ++i) 
	{
		if (node->AABB->CollidingWithSphere(parentElements[i]->GetPosition(), parentElements[i]->GetBoundingRadius())) 
		{
			newPhysicsNodes.push_back(parentElements[i]);
		}
	}

	return newPhysicsNodes;
}

std::vector<CollisionPair> FixedWorldPartition::CreatePairs(Node * node)
{
	std::vector<CollisionPair> pairs;

	if (node) {
		PhysicsNode *pnodeA, *pnodeB;

		if (node->children) {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i])
				{
					std::vector<CollisionPair> childpairs = CreatePairs(node->children[i]);
					bool flag = false;

					for (size_t j = 0; j < childpairs.size(); ++j) 
					{
						flag = false;
						for (size_t k = 0; k < pairs.size(); ++k) 
						{
							if (childpairs[j].pObjectA == pairs[k].pObjectA	&& childpairs[j].pObjectB == pairs[k].pObjectB) 
							{
								flag = true;
								break;

							}
						}
						if (!flag) 
						{
							pairs.push_back(childpairs[j]); 
						}
					}
				}
			}
		}
		else 
		{
			if (node->elements.size() > 0) 
			{
				if (node->elements.size() != 1)
				{
					for (size_t i = 0; i < node->elements.size() - 1; ++i)
					{
						for (size_t j = i + 1; j < node->elements.size(); ++j)
						{
							pnodeA = node->elements[i];
							pnodeB = node->elements[j];

							//Check they both at least have collision shapes
							bool siblings;
							if (pnodeA->GetParent())
							{
								siblings = pnodeA->GetParent()->SiblingsCollide();
							}
							else
							{
								siblings = true;
							}

							if (pnodeA->GetCollisionShape() != NULL
								&& pnodeB->GetCollisionShape() != NULL
								&& (pnodeA->GetPosition() - pnodeB->GetPosition()).Length() <= pnodeA->GetBoundingRadius() + pnodeB->GetBoundingRadius()
								&& (pnodeA->GetParent() != pnodeB->GetParent() || (pnodeA->GetParent() == pnodeB->GetParent() && siblings))
								)
							{
								CollisionPair cp;
								cp.pObjectA = pnodeA;
								cp.pObjectB = pnodeB;
								pairs.push_back(cp);
							}

							for (std::vector<PhysicsNode*>::iterator itr = bigNodes->begin(); itr != bigNodes->end(); itr++)
							{
								CollisionPair cp;
								cp.pObjectA = *itr;
								cp.pObjectB = node->elements[i];
								pairs.push_back(cp);
							}
						}

						if (i == node->elements.size() - 2)
						{
							for (std::vector<PhysicsNode*>::iterator itr = bigNodes->begin(); itr != bigNodes->end(); itr++)
							{
								CollisionPair cp;
								cp.pObjectA = *itr;
								cp.pObjectB = node->elements[i + 1];
								pairs.push_back(cp);
							}
						}
					}
				}
				else
				{
					for (std::vector<PhysicsNode*>::iterator itr = bigNodes->begin(); itr != bigNodes->end(); itr++)
					{
						CollisionPair cp;
						cp.pObjectA = *itr;
						cp.pObjectB = node->elements[0];
						pairs.push_back(cp);
					}
				}
			}
		}
	}

	return pairs;
}

void FixedWorldPartition::UpdateFixedWorldPartition() 
{

	if (physicsNodes->size() > 0) 
	{
		for (size_t i = 0; i < physicsNodes->size(); ++i) 
		{
			if ((*physicsNodes)[i]->GetLinearVelocity().Length() > 0)
			{
				updateList.push_back((*physicsNodes)[i]);
			}
		}

		if (updateList.size() > 0) 
		{
			CheckElements(root);
		}
		
		updateList.clear();
	}
}


void FixedWorldPartition::CheckElements(Node * node)
{
	if (node)
	{
		if (node->children) 
		{
			for (int i = 0; i < 8; ++i) 
			{
				CheckElements(node->children[i]);
			}
		}
		else 
		{
			// For each updating object check if the current node contains it
			for (size_t i = 0; i < updateList.size(); ++i) 
			{
				// if it does contain it, check if the object is still in the node and remove it if it doesnt
				std::vector<PhysicsNode*>::iterator it = std::find(node->elements.begin(), node->elements.end(), updateList[i]);
				if (it != node->elements.end()) 
				{
					if (!(node->AABB->CollidingWithSphere(updateList[i]->GetPosition(), updateList[i]->GetBoundingRadius()))) 
					{
						node->elements.erase(std::remove(node->elements.begin(), node->elements.end(), *it), node->elements.end());
					}
				}
				//if it doesnt already contain it, check if the object is now in the node and if so, add it to the list 
				else 
				{
					if (node->AABB->CollidingWithSphere(updateList[i]->GetPosition(), updateList[i]->GetBoundingRadius())) 
					{
						node->elements.push_back(updateList[i]);
					}
				}
			}
		}
	}
}

//Returns all the physics objects in the world space 
std::vector<PhysicsNode*> FixedWorldPartition::GetElemsInChildren(Node * node) 
{
	std::vector<PhysicsNode*> nodes;

	if (node->children)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (node->children[i])
			{
				std::vector<PhysicsNode*> childnodes = GetElemsInChildren(node->children[i]);
				for (size_t j = 0; j < childnodes.size(); ++j) 
				{
					if (std::find(nodes.begin(), nodes.end(), childnodes[j]) == nodes.end()) 
					{
						nodes.push_back(childnodes[j]);
					}
				}
			}
		}
	}

	else 
	{
		nodes = node->elements;
	}

	return nodes;
}

// If A node has children but the total physicsnodes in this parent node is less than the limit, delete the children and store the physics nodes in own vector
void FixedWorldPartition::CombineNode(Node * parent) 
{
	for (int i = 0; i < 8; ++i) 
	{
		// Combine All children's Nodes
		if (parent->children[i]) 
		{
			for (size_t j = 0; j < parent->children[i]->elements.size(); ++j) 
			{
				if (std::find(parent->elements.begin(), parent->elements.end(), parent->children[i]->elements[j]) == parent->elements.end()) 
				{
					parent->elements.push_back(parent->children[i]->elements[j]);
				}
			}
			delete parent->children[i];
		}
	}
	parent->children = nullptr;
}


void FixedWorldPartition::DebugDraw(Node * node) const
{
	if (node) 
	{
		if (node->children) 
		{
			for (int i = 0; i < 8; ++i) 
			{
				if (node->children[i])
				{
					DebugDraw(node->children[i]);
				}
			}
		}
		else 
		{
			node->AABB->DebugDraw();
		}
	}
}