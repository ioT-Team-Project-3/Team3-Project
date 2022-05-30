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
#include "FND.cpp"

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
    int highScore;
    Cross s;
    DotMatrix DM;
    TactSW TSW;
    CharacterLCD CL;
    FND FN;

    //주어진 벡터를 닷 매트릭스로 설정
    void vector2Matrix(vector<obstacle> obstacles) {
        DM.clear();
        // 장애물들에 대한 접근
        for (int i = 0; i < obstacles.size(); i++) {

            // 각 장애물의 좌표들에 대한 접근
            for (int j = 0; j < obstacles[i].crd.size(); j++) {

                DM.set(obstacles[i].crd[j]);    
            }
        }
    }

public:
    Game()
    {
        highScore = 0;
    }

    // 게임 시작
    void start() {
        int head, score, playerTimer, obstacleTimer, horizonTimer;

        // CLCD에 게임시작방법 표시
        printf("game : waiting for user to press OK\n");
        CL.beforeGame();
        while (true) {
            //TactSW가 OK 버튼을 누를때까지 대기
            while (TSW.get() != 4)
                ;

            //타이머와 스코어 초기화
            horizonTimer = 0;
            obstacleTimer = 0;
            playerTimer = 0;
            score = 0;

            CL.gaming(highScore);

            printf("game : user selected head and game just started\n");

            // 플레이어가 맵 밖으로 이탈하거나 장애물과 충돌한다면 탈출
            while (s.isPlaying()) {

                // 20/600초 동안 dotMatrix에 플레이어와 장애물들을 표시
                print(TIME_QUANTUM * 20);

                // 20/600초 동안 FND에 표시
                FN.draw(TIME_QUANTUM * 20);

                // 타이머들의 시간 업데이트
                horizonTimer += TIME_QUANTUM;
                obstacleTimer += TIME_QUANTUM;
                playerTimer += TIME_QUANTUM;

                // tactsw 입력값 받기
                head = TSW.get();

                // if (!(obstacleTimer % (TIME_QUANTUM*30)))
                if (obstacleTimer > (TIME_QUANTUM*15)) { // 맵의 이동속도

                    // 장애물 타이머 초기화
                    obstacleTimer = 0;

                    if ((head >= 0) && (head < 4)) { // tactsw 입력값이 이동키에 해당한다면
                        s.downMovingObject(true); // 장애물들만 아래로 움직이게

                    } else { // tactsw 입력값이 이동키에 해당하지 않는다면                        
                        s.downMovingObject(false); // 플레이어와 장애물을 함께 아래로 움직이게
                    }

                    // 장애물 생성
                    s.regenObstacle();

                    // fnd 표기 점수 1 증가
                    FN.next();
                    score ++;
                }

                if (horizonTimer > (TIME_QUANTUM*10)) { // 움직이는 장애물 좌우 이동속도

                    // 수평 타이머 초기화
                    horizonTimer = 0;

                    // 움직이는 장애물 좌우 이동
                    s.horizonMovingObstacle();
                }

                if (playerTimer > (TIME_QUANTUM*5)) { // 플레이어 이동속도

                    if ((head >= 0) && (head < 4)) { // tactsw 입력값이 이동키에 해당한다면

                        // 플레이어 타이머 초기화
                        playerTimer = 0;

                        // 플레이어를 해당 방향으로 이동
                        s.go(head);
                    }
                }

            }

            // While문을 탈출했다면 CLCD값 변경
            printf("game : user just losted the game\nscore : %d\n", score);
            CL.gameOver(score);

            // HighScore를 넘겼다면 값 변경
            if (score > highScore)
            {
                highScore = score;
            }

            // 게임 리셋
            printf("game : waiting for user to press OK\n");
            CL.beforeGame();
            FN.reset();
            s.reset();
        }
    }

    //장비에 출력
    void print(int microSec)
    {
        // 도트매트릭스에 장애물 입력
        vector2Matrix(s.getObstacle());

        // 도트매트릭스에 플레이어 입력
        DM.set(s.get());

        // 도트매트릭스에 출력(점등)
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