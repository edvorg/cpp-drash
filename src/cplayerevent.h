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
    explicit CPlayerEvent( const PlayerAction & _action , const CVec2 _mousePos = CVec2());


    PlayerAction mType;
    void SetMousePos( const CVec2 &_pos );
    CVec2 GetMousePos()const;
private:
    CVec2 mMousePos;
};


}// namespace drash
#endif // CPLAYEREVENT_H
