/******************************************************************************
Class: PhysicsEngine
Implements:
Author: 
	Pieran Marris      <p.marris@newcastle.ac.uk> and YOU!
Description:

	The glue that brings all our physics dreams together. This class is provided
	pretty much as is, as most of the sub-systems are tied in to this already
	with a bit of debugging code to visualise various parts of the physics engine.

	The general runtime consists of:
		- Update(float dt)
		  - UpdatePhysics()
			 - Broadphase Collision Detection
			   Quickly identifies possible collisions in between objects, hopefully
			   with world space partitioning systems like Octrees.. but currently just
			   builds a list colliding all objects with all other objects. (Hopefully you
			   can find some free time to fix this =] )

			 - Narrowphase Collision Detection
			   Takes the list provided by the broadphase collision detection and 
			   accurately collides all objects, building a collision manifold as
			   required. (Tutorial 4/5)

			 - Solve Constraints & Collisions
			   Solves all velocity constraints in the physics system, these include
			   both Collision Constraints (Tutorial 5,6) and misc world constraints
			   like distance constraints (Tutorial 3)

			 - Update Physics Objects
			   Moves all physics objects through time, updating positions/rotations
			   etc. each iteration (Tutorial 2)

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "PhysicsNode.h"
#include "Constraint.h"
#include "Manifold.h"
#include "FixedWorldPartition.h"
#include <nclgl\TSingleton.h>
#include <nclgl\PerfTimer.h>
#include <vector>
#include <mutex>

//Number of jacobi iterations to apply in order to
// assure the constraints are solved. (Last tutorial)
#define SOLVER_ITERATIONS 50


//Just saves including windows.h for the sake of defining true/false
#ifndef FALSE
	#define FALSE	0
	#define TRUE	1
#endif

//Debugging flags to let the engine know what to debugging information should be drawn
// - To toggle a debug draw flag on/off use <physics>->SetDebugDrawFlags(<physics>->GetDebugDrawFlags() ^ <debug flag to toggle>)"
#define DEBUGDRAW_FLAGS_CONSTRAINT				0x1
#define DEBUGDRAW_FLAGS_MANIFOLD				0x2
#define DEBUGDRAW_FLAGS_COLLISIONVOLUMES		0x4
#define DEBUGDRAW_FLAGS_COLLISIONNORMALS		0x8
#define DEBUGDRAW_FLAGS_FIXED_WORLD				0x10

struct WorldLimits
{
	Vector3 minVals;
	Vector3 maxVals;

};

//phil 13/02/2018
//holds the shape that was hit and the location where it happened
//this might be replaced with something else in the future
struct LineCollision {
	PhysicsNode* node;
	float dist;
};

class PhysicsEngine : public TSingleton<PhysicsEngine>
{
	friend class TSingleton < PhysicsEngine > ;
public:
	//Reset Default Values like gravity/timestep - called when scene is switched out
	void SetDefaults();

	//Add/Remove Physics Objects
	void AddPhysicsObject(PhysicsNode* obj);
	void ResetWorldPartition();
	void RemovePhysicsObject(PhysicsNode* obj);
	void RemoveAllPhysicsObjects(); //Delete all physics entities etc and reset-physics environment for new scene to be initialized

	//Add Constraints
	void AddConstraint(Constraint* c) { constraints.push_back(c); }
	

	//Update Physics Engine
	void Update(float deltaTime);			//DeltaTime here is 'seconds' since last update not milliseconds
	
	//Debug draw all physics objects, manifolds and constraints
	void DebugRender();



	//Getters / Setters 
	inline bool IsPaused() const				{ return isPaused; }
	inline void SetPaused(bool paused)			{ isPaused = paused; }

	inline uint GetDebugDrawFlags() const { return debugDrawFlags;  }
	inline void SetDebugDrawFlags(uint flags)   { debugDrawFlags = flags; }
	
	inline float GetUpdateTimestep() const { return updateTimestep; }
	inline void SetUpdateTimestep(float updateTimestep) { this->updateTimestep = updateTimestep; }

	inline const Vector3& GetGravity() const	{ return gravity; }
	inline void SetGravity(const Vector3& g)	{ gravity = g; }

	inline float GetDampingFactor() const		{ return dampingFactor; }
	inline void  SetDampingFactor(float d)		{ dampingFactor = d; }

	inline float GetDeltaTime() const			{ return updateTimestep; }

	inline void SetIntegrator(Integrator i)		{ integrator = i; }
	inline void SetWorldPartitionMinSize(float s)		{ worldPartitioning->SetMinSize(s); }

	inline std::vector<PhysicsNode*>* GetBigNodes() {	return worldPartitioning->GetBigNodes();  }

	std::vector<PhysicsNode*>* GetAllNodesOfType(PhysNodeType type);

	inline void SetLimits(Vector3 minvals, Vector3 maxvals) { 
		limits.minVals = minvals; 
		limits.maxVals = maxvals; 
	}

	void PrintPerformanceTimers(const Vector4& color)
	{
		perfUpdate.PrintOutputToStatusEntry(color,		"            Integration :");
		perfBroadphase.PrintOutputToStatusEntry(color,	"            Broadphase  :");
		perfNarrowphase.PrintOutputToStatusEntry(color,	"            Narrowphase :");
		perfSolver.PrintOutputToStatusEntry(color,		"            Solver      :");
	}

	//allow the camera access to all the physics nodes
	friend class Camera;
protected:
	PhysicsEngine();
	~PhysicsEngine();

	//The actual time-independant update function
	void UpdatePhysics();

	//Handles broadphase collision detection
	void BroadPhaseCollisions();

	//Handles narrowphase collision detection
	void NarrowPhaseCollisions();

	//Phil 13/02/2018
	LineCollision CastRay(Vector3 origin, Vector3 direction);

protected:
	bool		isPaused;
	float		updateTimestep, updateRealTimeAccum;
	uint		debugDrawFlags;

	WorldLimits limits;
	Vector3		gravity;
	float		dampingFactor;


	std::vector<CollisionPair>  broadphaseColPairs;

	std::vector<PhysicsNode*>	physicsNodes;

	std::vector<Constraint*>	constraints;		// Misc constraints applying to one or more physics objects e.g our DistanceConstraint
	std::vector<Manifold*>		manifolds;			// Contact constraints between pairs of objects

	PerfTimer perfUpdate;
	PerfTimer perfBroadphase;
	PerfTimer perfNarrowphase;
	PerfTimer perfSolver;

	Integrator integrator = SYMPLETIC;

	FixedWorldPartition *worldPartitioning;
};