#include <physics.h>

Physics::Physics() {
        setup();
}

Physics::~Physics() {
        delete m_dynamicsWorld;
        delete m_solver;
        delete m_broadphase;
        delete m_dispatcher;
        delete m_collisionConfiguration;
}

void Physics::setup() {
        m_collisionConfiguration = new btDefaultCollisionConfiguration();
        m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
        m_broadphase = new btDbvtBroadphase();
        m_solver = new btSequentialImpulseConstraintSolver();

        m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
        m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void Physics::updateTransforms() {
        for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
                btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                btTransform transform;

                if (body && body->getMotionState()) {
                        body->getMotionState()->getWorldTransform(transform);
                } else {
                        transform = obj->getWorldTransform();
                }

                Transform* user = (Transform*)body->getUserPointer();
                
                if (user != nullptr) {
                        user->setPos(glm::vec3(
                                transform.getOrigin().getX(), 
                                transform.getOrigin().getY(), 
                                transform.getOrigin().getZ()
                        ));

                        user->setOrientation(glm::quat(
                                transform.getRotation().getW(),
                                transform.getRotation().getX(),
                                transform.getRotation().getY(),
                                transform.getRotation().getZ()
                        ));
                }
        }
}

btRigidBody* Physics::createRigidBody(btCollisionShape* shape, bool isKinematic, btScalar mass, Transform* user) {
        btTransform startTransform = castTransform(*user);

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
                shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbinfo(mass, motionState, shape, localInertia);

        btRigidBody* body = new btRigidBody(rbinfo);

        
        if (isKinematic && mass == 0.f) {
                body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                body->setActivationState(DISABLE_DEACTIVATION);
                body->setUserPointer(nullptr);
        } else {
                body->setUserPointer(user);
        }

        m_dynamicsWorld->addRigidBody(body);

        return body;
}

btTransform Physics::castTransform(const Transform& transform) {
        return btTransform(
                btQuaternion(
                        transform.m_orientation.x,
                        transform.m_orientation.y,
                        transform.m_orientation.z,
                        transform.m_orientation.w
                ),
                btVector3(
                        transform.m_pos.x,
                        transform.m_pos.y,
                        transform.m_pos.z
                )
        );
}

void Physics::syncTransform(btRigidBody* body, const Transform& transform) {
        btTransform newTransform = castTransform(transform);

        body->setWorldTransform(newTransform);
        body->getMotionState()->setWorldTransform(newTransform);
}

void Physics::destroyRigidBody(btRigidBody* body) {

}

void Physics::lockOrientation(btRigidBody* body) {
        btTransform transform;
        body->getMotionState()->getWorldTransform(transform);
        
        transform.setRotation(btQuaternion(0, 0, 0, 1));

        body->setWorldTransform(transform);
        body->getMotionState()->setWorldTransform(transform);
}

void Physics::step(float delta) {
        m_dynamicsWorld->stepSimulation(delta);
}