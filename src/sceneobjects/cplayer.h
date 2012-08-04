#ifndef CPLAYER_H
#define CPLAYER_H

#include "csolidbody.h"

namespace drash{

class CPlayerParams: public CSolidBodyParams{
public:
    float mSpeedJump;
    float mSpeedMoving;
    CPlayerParams();
};

class CPlayer: public CSolidBody
{
public:
    typedef CPlayerParams ParamsT;
    friend class CScene;


    enum PlayerEvent{StartMoveLeft,StartMoveRight,EndMoveLeft,EndMoveRight ,jump,fire};

    virtual void onEvent(const PlayerEvent & _event);
    virtual void BeginContact(CSceneObject *_object, const b2Manifold *_manifold);
    virtual void EndContact(CSceneObject *_object, const b2Manifold *_manifold);
protected:
    CPlayer();
    virtual ~CPlayer();

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);
    void Jump();
    void MoveRight();
    void MoveLeft();

    bool mJumpAllowed;
    bool mMovingLeft;
    bool mMovingRight;
    float mSpeedJump;
    float mSpeedMoving;

    static const float mHeightJump;
};

}// namespace drash
#endif // CPLAYER_H
