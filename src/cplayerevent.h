#ifndef CPLAYEREVENT_H
#define CPLAYEREVENT_H

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
    explicit CPlayerEvent(const PlayerAction & _action);



    PlayerAction mType;

};


}// namespace drash
#endif // CPLAYEREVENT_H
