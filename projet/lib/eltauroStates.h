#ifndef ELTAURO_STATE_H
#define ELTAURO_STATE_H

enum class RobotStatePart1
{
    FOLLOW_LINE,
    DETECT_POLE,
};

enum class Sequence
{
    CHOIX_DEPART,
    DEPART_A,
    DEPART_B,
    DEPART_S,
    END
};
#endif