#ifndef CPLAYER_H
#define CPLAYER_H

#include "csolidbody.h"

namespace drash{

class CPlayerParams: public CSolidBodyParams{
public:
    CPlayerParams();
};

class CPlayer: public CSolidBody
{
public:
    typedef CPlayerParams ParamsT;
    friend class CScene;


    enum PlayerEvent{moveLeft,moveRight,jump,fire};

    virtual void onEvent(const PlayerEvent & _event);
protected:
    CPlayer();
    virtual ~CPlayer();

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);

    void Jump();

    bool mJumping;
    static const float mHeightJump;
};

}// namespace drash
#endif // CPLAYER_H
