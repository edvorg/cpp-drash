#ifndef CPLAYER_H
#define CPLAYER_H

#include "csolidbody.h"
#include "../cplayerevent.h"

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

    virtual void onEvent(const CPlayerEvent & _event);
    virtual void BeginContact( const CContact &_contact );
    virtual void EndContact( const CContact &_contact );
protected:
    CPlayer();
    virtual ~CPlayer();

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);
    void Jump();
    void MoveRight();
    void MoveLeft();
    void FireNow(const CVec2 & _fireDirect);
    bool mJumpAllowed;
    bool mMovingLeft;
    bool mMovingRight;
    float mSpeedJump;
    float mSpeedMoving;

    static const float mHeightJump;
};

}// namespace drash
#endif // CPLAYER_H
