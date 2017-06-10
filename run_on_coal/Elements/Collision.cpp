#include "stdafx.h"
#include "Elements/Collision.h"
#include "Elements/Model/Model.h"

extern const glm::vec3 g_DefaultScale;

ROC::Collision::Collision()
{
    m_elementType = ElementType::CollisionElement;
    m_rigidBody = nullptr;
    m_scale = g_DefaultScale;
    m_parentModel = nullptr;
}
ROC::Collision::~Collision()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
    }
}

bool ROC::Collision::Create(int f_type, const glm::vec3 &f_size, float f_mass)
{
    if(!m_rigidBody)
    {
        btClamp(f_type, ROC_COLLISION_TYPE_SPHERE, ROC_COLLISION_TYPE_CONE);
        btVector3 l_inertia;
        btCollisionShape *l_shape = nullptr;
        switch(f_type)
        {
            case ROC_COLLISION_TYPE_SPHERE:
                l_shape = new btSphereShape(f_size.x);
                break;
            case ROC_COLLISION_TYPE_BOX:
                l_shape = new btBoxShape(btVector3(f_size.x, f_size.y, f_size.z));
                break;
            case ROC_COLLISION_TYPE_CYLINDER:
                l_shape = new btCylinderShape(btVector3(f_size.x, f_size.y, f_size.z));
                break;
            case ROC_COLLISION_TYPE_CAPSULE:
                l_shape = new btCapsuleShape(f_size.x, f_size.y);
                break;
            case ROC_COLLISION_TYPE_CONE:
                l_shape = new btConeShape(f_size.x, f_size.y);
                break;
        }
        l_shape->calculateLocalInertia(f_mass, l_inertia);
        btTransform l_transform;
        l_transform.setIdentity();
        btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(f_mass, l_fallMotionState, l_shape, l_inertia);
        m_rigidBody = new btRigidBody(fallRigidBodyCI);
    }
    return (m_rigidBody != nullptr);
}

void ROC::Collision::SetParentModel(Model *f_model)
{
    if(m_rigidBody)
    {
        if(f_model)
        {
            glm::vec3 l_pos;
            glm::quat l_rot;
            f_model->GetPosition(l_pos, true);
            f_model->GetRotation(l_rot);
            SetPosition(l_pos);
            SetRotation(l_rot);
            m_rigidBody->setUserPointer(f_model);
        }
        else m_rigidBody->setUserPointer(this);
        m_parentModel = f_model;
    }
}

void ROC::Collision::GetPosition(glm::vec3 &f_pos)
{
    if(m_rigidBody) std::memcpy(&f_pos, m_rigidBody->getCenterOfMassTransform().getOrigin().m_floats, sizeof(glm::vec3));
}
void ROC::Collision::SetPosition(const glm::vec3 &f_pos)
{
    if(m_rigidBody)
    {
        btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
        l_transform.setOrigin(btVector3(f_pos.x, f_pos.y, f_pos.z));
        m_rigidBody->setCenterOfMassTransform(l_transform);
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetRotation(glm::quat &f_rot)
{
    btQuaternion l_rotation = m_rigidBody->getCenterOfMassTransform().getRotation();
    f_rot.x = l_rotation.x();
    f_rot.y = l_rotation.y();
    f_rot.z = l_rotation.z();
    f_rot.w = l_rotation.w();
}
void ROC::Collision::SetRotation(const glm::quat &f_rot)
{
    if(m_rigidBody)
    {
        btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
        l_transform.setRotation((btQuaternion&)f_rot);
        m_rigidBody->setCenterOfMassTransform(l_transform);
        m_rigidBody->activate(true);
    }
}

void ROC::Collision::SetScale(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        if(m_scale != f_val)
        {
            std::memcpy(&m_scale, &f_val, sizeof(glm::vec3));
            m_rigidBody->getCollisionShape()->setLocalScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));
        }
    }
}
void ROC::Collision::GetScale(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, &m_scale, sizeof(glm::vec3));
}

void ROC::Collision::SetVelocity(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setLinearVelocity(btVector3(f_val.x, f_val.y, f_val.z));
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getLinearVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetAngularVelocity(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setAngularVelocity(btVector3(f_val.x, f_val.y, f_val.z));
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetAngularVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getAngularVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetLinearFactor(const glm::vec3 &f_val)
{
    if(m_rigidBody) m_rigidBody->setLinearFactor(btVector3(f_val.x, f_val.y, f_val.z));
}
void ROC::Collision::GetLinearFactor(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getLinearFactor().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetAngularFactor(const glm::vec3 &f_val)
{
    if(m_rigidBody) m_rigidBody->setAngularFactor(btVector3(f_val.x, f_val.y, f_val.z));
}
void ROC::Collision::GetAngularFactor(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getAngularFactor().m_floats, sizeof(glm::vec3));
}

float ROC::Collision::GetMass()
{
    float l_mass = -1.f;
    if(m_rigidBody)
    {
        float l_invMass = m_rigidBody->getInvMass();
        l_mass = ((l_invMass == 0.f) ? 0.f : (1.f / l_invMass));
    }
    return l_mass;
}

void ROC::Collision::SetFriction(float f_val)
{
    if(m_rigidBody)
    {
        btClamp(f_val, 0.f, 1.f);
        m_rigidBody->setFriction(f_val);
        m_rigidBody->activate(true);
    }
}

void ROC::Collision::ApplyForce(const glm::vec3 &f_force, const glm::vec3 &f_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_force(f_force.x, f_force.y, f_force.z);
        btVector3 l_relPos(f_rp.x, f_rp.y, f_rp.z);
        m_rigidBody->applyForce(l_force, l_relPos);
    }
}
void ROC::Collision::ApplyCentralForce(const glm::vec3 &f_force)
{
    if(m_rigidBody) m_rigidBody->applyCentralForce(btVector3(f_force.x, f_force.y, f_force.z));
}

void ROC::Collision::ApplyImpulse(const glm::vec3 &f_impulse, const glm::vec3 &f_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_impulse(f_impulse.x, f_impulse.y, f_impulse.z);
        btVector3 l_relPos(f_rp.x, f_rp.y, f_rp.z);
        m_rigidBody->applyImpulse(l_impulse, l_relPos);
    }
}
void ROC::Collision::ApplyCentralImpulse(const glm::vec3 &f_impulse)
{
    if(m_rigidBody) m_rigidBody->applyCentralImpulse(btVector3(f_impulse.x, f_impulse.y, f_impulse.z));
}

void ROC::Collision::ApplyTorque(const glm::vec3 &f_torque, bool f_impulse)
{
    if(m_rigidBody)
    {
        btVector3 l_torque(f_torque.x, f_torque.y, f_torque.z);
        f_impulse ? m_rigidBody->applyTorqueImpulse(l_torque) : m_rigidBody->applyTorque(l_torque);
    }
}

void ROC::Collision::GetTransform(glm::mat4 &f_mat, glm::vec3 &f_pos, glm::quat &f_rot)
{
    if(m_rigidBody)
    {
        const btTransform &l_transform = m_rigidBody->getCenterOfMassTransform();
        btQuaternion l_rotation = l_transform.getRotation();
        std::memcpy(&f_pos, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
        f_rot.x = l_rotation.x();
        f_rot.y = l_rotation.y();
        f_rot.z = l_rotation.z();
        f_rot.w = l_rotation.w();
        l_transform.getOpenGLMatrix(reinterpret_cast<float*>(&f_mat));
    }
}
