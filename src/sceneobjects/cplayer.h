#ifndef CPLAYER_H
#define CPLAYER_H

#include "csolidbody.h"
// TODO: I not test this class
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


    enum Action{moveLeft,moveRight,jump,fire};

    virtual void onAction(const Action & _action);
protected:
    CPlayer();
    virtual ~CPlayer();

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);

    void mJump();

    bool mJumping;
    static const float mHeightJump;
};

}// namespace drash
#endif // CPLAYER_H
