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

    CPlayer();
    virtual ~CPlayer();

protected:

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);

    virtual void Step(double _dt);

};

}// namespace drash
#endif // CPLAYER_H
