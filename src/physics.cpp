#include <physics.h>

PhysicsManager::PhysicsManager() {
        setup();
}

PhysicsManager::~PhysicsManager() {
        delete m_dynamicsWorld;
        delete m_solver;
        delete m_broadphase;
        delete m_dispatcher;
        delete m_collisionConfiguration;
}

void PhysicsManager::setup() {
        m_collisionConfiguration = new btDefaultCollisionConfiguration();
        m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
        m_broadphase = new btDbvtBroadphase();
        m_solver = new btSequentialImpulseConstraintSolver();

        m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
        m_dynamicsWorld->setGravity(btVector3(0.f, -0.1f, 0.f));
}

void PhysicsManager::updateTransforms() {
        for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
                btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                btTransform transform;

                if (body && body->getMotionState()) {
                        body->getMotionState()->getWorldTransform(transform);
                } else {
                        transform = obj->getWorldTransform();
                }

                Transform* entityTransform = (Transform*)body->getUserPointer();
                entityTransform->translate(glm::vec3(0.f, 
                                                transform.getOrigin().getY(), 
                                                transform.getOrigin().getZ()), Transform::WORLD_RELATIVE);
        }
}

void PhysicsManager::destroyInactiveRigidBodies() {
        for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end();) {
                if (!it->m_isActive) {
                        auto tmp = it++;
                        m_rigidBodies.erase(tmp);

			if (it == m_rigidBodies.end()) 
				break;
		} else {
			it++;
		}
        }
}

PRigidBody* PhysicsManager::createRigidBody(btCollisionShape* shape, Transform* transform, bool isKinematic, btScalar mass) {
        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setOrigin(btVector3(transform->m_pos.x, 
                                        transform->m_pos.y, 
                                        transform->m_pos.z));

        btVector3 localInertia(0, 0, 0);
        if (mass != 0.f)
                shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbinfo(mass, motionState, shape, localInertia);
        btRigidBody* body = new btRigidBody(rbinfo);

        body->setUserPointer(transform);

        if (isKinematic)
                std::clog << "kinematic" << std::endl;

        m_dynamicsWorld->addRigidBody(body);

        m_rigidBodies.emplace_back(PRigidBody(body, shape, transform, isKinematic, mass));
        return &m_rigidBodies.back();
}

void PhysicsManager::step() {
        m_dynamicsWorld->stepSimulation(1 / 60.f, 1, 1 / 60.f);
}