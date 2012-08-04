#ifndef CPLAYEREVENT_H
#define CPLAYEREVENT_H

#include "cvec2.h"

namespace drash{

class CPlayerEvent
{
public:
    enum PlayerAction{StartMoveLeft,
                      StartMoveRight,
                      EndMoveLeft,
                      EndMoveRight ,
                      jump,
                      fire
                     };
    explicit CPlayerEvent( const PlayerAction & _action , const CVec2 _posMouse = CVec2());


    PlayerAction mType;
    void SetPosMouse( const CVec2 &_pos );
    CVec2 GetPosMouse()const;
private:
    CVec2 mPosMouse;
};


}// namespace drash
#endif // CPLAYEREVENT_H
