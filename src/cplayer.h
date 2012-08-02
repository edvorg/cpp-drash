#ifndef CPLAYER_H
#define CPLAYER_H

#include "csceneobject.h"

namespace drash{

class CPlayerParams: public CSceneObjectParams{
public:
    CPlayerParams();
};

class CPlayer: public CSceneObject
{
public:
    typedef CPlayerParams ParamsT;
    friend class CScene;

    CPlayer();
    virtual ~CPlayer();

protected:

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);

};

}// namespace drash
#endif // CPLAYER_H
