#include <inc.h>
#include <game.h>

//#include <btBulletDynamicsCommon.h>

int main(int argc, char** argv)
{
	Game game;
	game.createWindow(800, 600);
	game.start();

	/*
	btDefaultCollisionConfiguration* collisionConfig = new
		btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);

	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
		solver, collisionConfig);

	dynamicsWorld->setGravity(btVector3(0.f, 10.f, 0.f));

	for (unsigned i = 0; i < 100; i++) {
		dynamicsWorld->stepSimulation(1.f/60.f, 10);
		
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j++) {
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;

			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(trans);
			} else {
				trans = obj->getWorldTransform();
			}

			std::clog << "Object world pos: " << j << " "
				<< trans.getOrigin().getX() << " "
				<< trans.getOrigin().getY() << " "
				<< trans.getOrigin().getZ() << std::endl;
		}
	}

	for (unsigned i = dynamicsWorld->getNumCollisionObjects(); i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for (unsigned i = 0; i < )*/

	return 0;
}
