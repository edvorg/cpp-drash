#ifndef DRASH_JOINT_H
#define DRASH_JOINT_H

class b2Joint;

namespace drash
{

class CJoint
{
public:
    friend class CScene;

protected:
private:
    b2Joint *mJoint = nullptr;
};

} // namespace drash

#endif // DRASH_JOINT_H
