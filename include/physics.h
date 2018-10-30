#ifndef PHYSICS_H
#define PHYSICS_H

#include <inc.h>
#include <transform.h>
#include <list>

class Physics {
public:
        Physics();
        ~Physics();

        void step(float delta);

        void setup();
        void updateTransforms();

        btRigidBody* createRigidBody(btCollisionShape* shape, bool isKinematic, btScalar mass, Transform* user);

        void destroyRigidBody(btRigidBody* body);

        btDynamicsWorld* m_dynamicsWorld;

        static btTransform castTransform(const Transform& transform);
        static void syncTransform(btRigidBody* body, const Transform& transform);
        static void lockOrientation(btRigidBody* body);
private:
	btSequentialImpulseConstraintSolver* m_solver;
	btCollisionConfiguration* m_collisionConfiguration;
	btBroadphaseInterface* m_broadphase;
        btCollisionDispatcher* m_dispatcher;
};

#endif //PHYSICS_H