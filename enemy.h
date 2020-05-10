#ifndef ENEMY_H
#define ENEMY_H
#include "tankbase.h"
#include "config.h"

class Enemy : public tankBase
{
public:
    Enemy();
    bool operator==(const Enemy& other);
    virtual ~Enemy();
};

#endif // ENEMY_H
