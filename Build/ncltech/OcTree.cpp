#include "OcTree.h"
#include <algorithm>
#include "MultiGameObject.h"

OcTree::OcTree(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, std::vector<PhysicsNode*> * elements)
{
	OcTree(Vector3(xmin, ymin, zmin), Vector3(xmax, ymax, zmax), elements);
}

OcTree::OcTree(Vector3 mins, Vector3 maxs, std::vector<PhysicsNode*>  * elements)
{
	root = new Node;
	root->AABB = new BoundingBox;
	root->AABB->ExpandToFit(mins);
	root->AABB->ExpandToFit(maxs);
	root->children = nullptr;

	physicsNodes = elements;

	if (physicsNodes->size() > maxNumber) {
		SplitBox(root, *physicsNodes);
	}
	else {
		root->elements = *physicsNodes;
	}
}

OcTree::~OcTree()
{
	TerminateTree(&root);
}

void OcTree::TerminateTree(Node ** node) {
	if (*node) {
		if ((*node)->children) {
			for (int i = 0; i < 8; ++i) {
				if ((*node)->children[i]) { TerminateTree(&((*node)->children[i])); }
			}
		}

		(*node)->children = nullptr;
		delete (*node);
		(*node) = nullptr;
		
	}
}

void OcTree::SplitBox(Node * node, std::vector<PhysicsNode*> parentElements)
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

for (int i = 0; i < 8; ++i) {
	std::vector<PhysicsNode*> childphysnodes = FindElementsInNode(node->children[i], parentElements);

	

	if (childphysnodes.size() > maxNumber && abs(halfsize.x) >= minSize && abs(halfsize.y) >= minSize && abs(halfsize.z) >= minSize) {
		SplitBox(node->children[i], childphysnodes);
	}
	//else if (childphysnodes.size() == 0) {
	//	node->children[i] = nullptr;
	//}
	else {
		node->children[i]->elements = childphysnodes;
	}

}
}

std::vector<PhysicsNode*> OcTree::FindElementsInNode(Node * node, std::vector<PhysicsNode*> parentElements)
{
	std::vector<PhysicsNode*> newPhysicsNodes;
	for (size_t i = 0; i < parentElements.size(); ++i) {
		if (node->AABB->CollidingWithSphere(parentElements[i]->GetPosition(), parentElements[i]->GetBoundingRadius())) {
			newPhysicsNodes.push_back(parentElements[i]);
		}
	}
	return newPhysicsNodes;
}

std::vector<CollisionPair> OcTree::CreatePairs(Node * node)
{
	std::vector<CollisionPair> pairs;

	if (node) {
		PhysicsNode *pnodeA, *pnodeB;

		if (node->children) {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i])
				{
					std::vector<CollisionPair> childpairs = CreatePairs(node->children[i]);
					//pairs.insert(pairs.end(), childpairs.begin(), childpairs.end());
	
					for (size_t j = 0; j < childpairs.size(); ++j) {
						bool flag = false;
						for (size_t k = 0; k < pairs.size(); ++k) {
							if (childpairs[j].pObjectA == pairs[k].pObjectA
								&& childpairs[j].pObjectB == pairs[k].pObjectB
								) {
								flag = true;
								break;
								
							}
						}
						if (!flag) { pairs.push_back(childpairs[j]); }
					}
				}

			}
		}
		else {
			if (node->elements.size() > 0) {
				for (size_t i = 0; i < node->elements.size() - 1; ++i)
				{
					for (size_t j = i + 1; j < node->elements.size(); ++j)
					{
						pnodeA = node->elements[i];
						pnodeB = node->elements[j];

						//Check they both atleast have collision shapes
						bool siblings;
						if (pnodeA->GetParent()) {
							siblings = pnodeA->GetParent()->SiblingsCollide();
						}
						else {
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
					}
				}
			}
		}
	}

	return pairs;
}

void OcTree::UpdateOcTree() {

	if (physicsNodes->size() > 0) {
		for (size_t i = 0; i < physicsNodes->size(); ++i) {
			if ((*physicsNodes)[i]->GetLinearVelocity().Length() > 0) {
				updateList.push_back((*physicsNodes)[i]);
			}
		}

		if (updateList.size() > 0) {
			CheckElements(root);
			UpdateNode(root);
		}


		updateList.clear();
	}
}


void OcTree::CheckElements(Node * node)
{
	if (node)
	{
		if (node->children) {
			for (int i = 0; i < 8; ++i) {
				CheckElements(node->children[i]);
			}
		}
		else {
			// For each updating object check if the current node contains it
			for (size_t i = 0; i < updateList.size(); ++i) {
				// if it does contain it, check if the object is still in the node and remove it if it doesnt
				std::vector<PhysicsNode*>::iterator it = std::find(node->elements.begin(), node->elements.end(), updateList[i]);
				if (it != node->elements.end()) {
					if (!(node->AABB->CollidingWithSphere(updateList[i]->GetPosition(), updateList[i]->GetBoundingRadius()))) {
						node->elements.erase(std::remove(node->elements.begin(), node->elements.end(), *it), node->elements.end());
					}
				}
				//if it doesnt already contain it, check if the object is now in the node and if so, add it to the list 
				else {
					if (node->AABB->CollidingWithSphere(updateList[i]->GetPosition(), updateList[i]->GetBoundingRadius())) {
						node->elements.push_back(updateList[i]);
					}
				}
			}
		}
	}
}

// Update a given node
void OcTree::UpdateNode(Node * node) {
	if (node)
	{
		if (node->children) {
			int totalElems = 0;
			vector<PhysicsNode*> totalnodes = GetElemsInChildren(node);
			totalElems = totalnodes.size();
			// if the total elements is less than the limit combine all the children into one parent node
			if (totalElems <= maxNumber) {
				CombineNode(node);
			}
			else {
				// if the total is still over the limit then go into children and run update node again
				for (int i = 0; i < 8; ++i) {
					if (node->children)
						UpdateNode(node->children[i]);
				}
			}
		}
		// if there are no children, check if node needs to be split into more children after the update
		else {
			// if the node now has more than the max number of elements split it
			Vector3 half = (node->AABB->_max - node->AABB->_min) * 0.5f;
			if (node->elements.size() > maxNumber) {
				bool flag = true;
			}
			if (node->elements.size() > maxNumber  && abs(half.x) >= minSize && abs(half.y) >= minSize && abs(half.z) >= minSize) {
				SplitBox(node, node->elements);
				node->elements.clear();
			}

		}
	}
}

std::vector<PhysicsNode*> OcTree::GetElemsInChildren(Node * node) {
	std::vector<PhysicsNode*> nodes;
	if (node->children) {
		for (int i = 0; i < 8; ++i) {
			if (node->children[i]) {
				std::vector<PhysicsNode*> childnodes = GetElemsInChildren(node->children[i]);
				for (size_t j = 0; j < childnodes.size(); ++j) {
					if (std::find(nodes.begin(), nodes.end(), childnodes[j]) == nodes.end()) {
						nodes.push_back(childnodes[j]);
					}
				}
			}
		}
	}
	else {
		nodes = node->elements;
	}
	return nodes;
}

// If A node has children but the total physicsnodes in this parent node is less than the limit, delete the children and store the physics nodes in own vector
void OcTree::CombineNode(Node * parent) {
	for (int i = 0; i < 8; ++i) {
		// Combine All children's Nodes
		if (parent->children[i]) {
			for (size_t j = 0; j < parent->children[i]->elements.size(); ++j) {
				if (std::find(parent->elements.begin(), parent->elements.end(), parent->children[i]->elements[j]) == parent->elements.end()) {
					parent->elements.push_back(parent->children[i]->elements[j]);
				}
			}
			delete parent->children[i];
		}
	}
	parent->children = nullptr;
}


void OcTree::DebugDraw(Node * node) const {
	if (node) {
		if (node->children) {
			for (int i = 0; i < 8; ++i) {
				if (node->children[i])
				{
					DebugDraw(node->children[i]);
				}
			}
		}
		else {
			node->AABB->DebugDraw();
		}
	}
}