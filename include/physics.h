#ifndef PHYSICS_H
#define PHYSICS_H

#include <inc.h>
#include <transform.h>
#include <list>

struct PRigidBody {
        btRigidBody* m_body;
        btCollisionShape* m_shape;
        btScalar m_mass;

        Transform* m_transform;

        bool m_isActive;
        const bool m_isKinematic;

        PRigidBody(btRigidBody* body, 
                btCollisionShape* shape, 
                Transform* transform,
                bool isKinematic,
                btScalar mass) :
                m_body(body),
                m_shape(shape),
                m_transform(transform),
                m_isActive(true),
                m_isKinematic(isKinematic),
                m_mass(mass)
        {}
};

class PhysicsManager {
public:
        PhysicsManager();
        ~PhysicsManager();

        void step();

        void setup();
        void updateTransforms();
        void destroyInactiveRigidBodies();

        PRigidBody* createRigidBody(btCollisionShape* shape, Transform* transform, bool isKinematic, btScalar mass);
private:
        btDynamicsWorld* m_dynamicsWorld;
	btSequentialImpulseConstraintSolver* m_solver;
	btCollisionConfiguration* m_collisionConfiguration;
	btBroadphaseInterface* m_broadphase;
        btCollisionDispatcher* m_dispatcher;

        std::list<PRigidBody> m_rigidBodies;
};

#endif //PHYSICS_H