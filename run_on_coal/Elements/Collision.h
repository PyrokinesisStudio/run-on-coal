#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Collision final : public Element
{
    btRigidBody *m_rigidBody;
    int m_motionType;
    glm::vec3 m_scale;

    Collision(const Collision &that);
    Collision& operator=(const Collision &that);
public:
    enum CollisionType
    {
        CT_Sphere,
        CT_Box,
        CT_Cylinder,
        CT_Capsule,
        CT_Cone
    };
    enum CollisionMotionType
    {
        CMT_Default,
        CMT_Static,
        CMT_Kinematic
    };

    void SetPosition(const glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos) const;

    void SetRotation(const glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot) const;

    void GetScale(glm::vec3 &f_val) const;

    void SetVelocity(const glm::vec3 &f_val);
    void GetVelocity(glm::vec3 &f_val) const;

    void SetAngularVelocity(const glm::vec3 &f_val);
    void GetAngularVelocity(glm::vec3 &f_val) const;

    void SetLinearFactor(const glm::vec3 &f_val);
    void GetLinearFactor(glm::vec3 &f_val) const;

    void SetAngularFactor(const glm::vec3 &f_val);
    void GetAngularFactor(glm::vec3 &f_val) const;

    float GetMass() const;

    void SetFriction(float f_val);
    inline float GetFriction() const { return (m_rigidBody ? m_rigidBody->getFriction() : -1.f); }

    void ApplyForce(const glm::vec3 &f_force, const glm::vec3 &f_rp);
    void ApplyCentralForce(const glm::vec3 &f_force);

    void ApplyImpulse(const glm::vec3 &f_impulse, const glm::vec3 &f_rp);
    void ApplyCentralImpulse(const glm::vec3 &f_impulse);

    void ApplyTorque(const glm::vec3 &f_torque, bool f_impulse);

    void SetMotionType(int f_type);
    inline int GetMotionType() const { return m_motionType; }

    void GetMatrix(glm::mat4 &f_mat) const;
protected:
    Collision();
    ~Collision();

    bool Create(int f_type, const glm::vec3 &f_size, float f_mass);

    inline bool IsActive() const { return m_rigidBody->isActive(); }

    void SetScale(const glm::vec3 &f_val);

    inline btRigidBody* GetRigidBody() const { return m_rigidBody; }

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class Model;
};

}
