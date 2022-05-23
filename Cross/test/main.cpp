#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "DotMatrix.cpp"
#include "Cross.cpp"
#include "TactSW.cpp"
#include "CharacterLCD.cpp"

// 1/600초를 정의
#define TIME_QUANTUM 1667

using namespace std;

// YX좌표를 표현하기 위한 구조체
#ifndef COORD_SET
typedef struct coord
{
    int y;
    int x;
} coord;
#endif

class Game
{
private:
    int timer;
    Cross s;
    DotMatrix DM;
    TactSW TSW;
    CharacterLCD CL;

public:
    Game()
    {
    }

    // 게임 시작
    void start()
    {
        int head, temp, temp2;

        // CLCD에 게임시작방법 표시
        printf("game : waiting for user to press OK\n");
        CL.beforeGame();
        while (true)
        {
            //TactSW가 OK 버튼을 누를때까지 대기
            while (TSW.get() != 4)
                ;

            //타이머 초기화
            timer = 1;

            // head를 -1로 초기화하여 사용자가 플레이어를 확인하고 시작하게 도움
            head = -1;


            CL.gaming();


            temp2 = s.isGet();
            printf("game : user selected head and game just started\n");

            while (temp2)
            {
                // 20/600초 동안 dotMatrix에 표시
                print(TIME_QUANTUM * 20);

                temp = TSW.get();

                // TactSW의 값이 내가 원하는 값인가?
                if ((temp >= 0) && (temp < 4))
                {
                    // 그렇다면 진행방향으로 이동
                    head = temp;
                    s.go(head);
                }

                if (!s.isGet()) {
                        // 이동에 문제가 발생하였다면 게임오버
                        break;
                }
                
                timer++;
            }

            // While문을 탈출했다면
            CL.gameOver();

            // 게임 리셋
            printf("game : waiting for user to press OK\n");
            CL.beforeGame();
            s.reset();
        }
    }

    //장비에 출력
    void print(int microSec)
    {
        DM.clear();
        DM.set(s.get());
        DM.drawToMatrix(microSec);
    }
};

int main(int argc, const char *argv[])
{
    Game g;

    printf("starting game...\n");
    g.start();
    printf("ending game...\n");

    return 0;
}
