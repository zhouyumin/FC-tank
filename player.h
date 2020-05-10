#ifndef PLAYER_H
#define PLAYER_H
#include "tankbase.h"

class player : public tankBase
{
public:
    bool ismove;
    player();
    void setDir(direct) override;
};

#endif // PLAYER_H
