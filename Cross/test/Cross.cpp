#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <ctime>

#define ENUM_SET

using namespace std;

// coord구조체 사용
#ifndef COORD_SET
typedef struct coord
{
    int y;
    int x;
} coord;
#endif

//ENUM을 활용해서 0,1,2,3,4를 설정
typedef enum : int
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OK
} UDLR;

class Cross {
    private:
    // 플레이어 좌표
    coord currentYX;

    int getRandomNum(int max) { return rand() % max; }

    static bool isOnSpace(coord C)
    {
        if (C.y > 7 || C.y < 0 || C.x > 7 || C.x < 0)
        {
            return false;
        }
        return true;
    }

    public:
    Cross()
    {
        srand((unsigned int)time(NULL));
        reset();
    }

    void reset()
    {
        printf("game : resetting game\n");
        coord C = {6, 3};
        currentYX = C;
    }

    //향하는 방향으로 움직임.
    bool go(int heading)
    {
        coord ret = currentYX;
        switch (heading)
        {
        case UP:
            ret.y -= 1;
            break;

        case DOWN:
            ret.y += 1;
            break;

        case LEFT:
            ret.x += 1;
            break;

        case RIGHT:
            ret.x -= 1;
            break;

        default:
            break;
        }
        if (isOnSpace(ret))
        {
            //규정된 space내에 위치한다면, 좌표로이동
            //그렇지 않다면 게임오버.
            currentYX = ret;

            return true;
        }
        return false;
    }

    // 8*8의 판 위에 존재하는 지 확인하는 코드

    bool isGet() { return isOnSpace(currentYX); }
    coord get() { return currentYX; }
};