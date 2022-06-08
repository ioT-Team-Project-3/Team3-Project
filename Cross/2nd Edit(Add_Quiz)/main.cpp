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
    int dif_map;
    int dif_obs;
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
        bool exit = true;

        // CLCD에 게임시작방법 표시
        printf("game : waiting for user to press OK\n");
        CL.beforeGame();
        while (true) {

            //TactSW가 OK 버튼을 누를때까지 대기
            while (TSW.get() != 4)
                ;

	        CL.choose_dif();
	        //TactSW가 난이도 선택 버튼을 누를때까지 대기 (10,11,12번 버튼)
	        while (1) {
		    if (TSW.get() == 5) {
		        dif_map = 10;
		        dif_obs = 5;
		        break;
		    }
		    if (TSW.get() == 6) {
		        dif_map = 15;
		        dif_obs = 10;
		        break;
		    }
		    if (TSW.get() == 7) {
		        dif_map = 20;
		        dif_obs = 15;
		        break;
		    }
	    }

            //타이머와 스코어 초기화
            horizonTimer = 0;
            obstacleTimer = 0;
            playerTimer = 0;
            score = 0;

            CL.gaming(highScore);

            printf("game : user selected head and game just started\n");

            // 플레이어가 맵 밖으로 이탈하거나 장애물과 충돌, 중간문제 오답 시 탈출
            while (s.isPlaying() && exit) {

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
                if (obstacleTimer > (TIME_QUANTUM*dif_map)) { // 맵의 이동속도

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

                if (horizonTimer > (TIME_QUANTUM*dif_obs)) { // 움직이는 장애물 좌우 이동속도

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

                if ((score != 0) && (score % 10 == 0))
                {
                    int first = s.first();
                    int second = s.second();
                    bool OX = s.choose_answer();
                    int sum;
                    if (OX)
                        sum = first + second;
                    else
                        sum = first + second + 10;
                    CL.quiz(first, second, sum);
                    while (1)
                    {
                        if (sum == first + second)
                        {
                            if (TSW.get() == 8)
                            {
                                CL.gaming(highScore);
                                break;
                            }
                            if (TSW.get() == 9)
                            {
                                exit = false;
                                break;
                            }
                        }
                        else
                        {
                            if (TSW.get() == 8)
                            {
                                exit = false;
                                break;
                            }
                            if (TSW.get() == 9)
                            {
                                CL.gaming(highScore);
                                break;
                            }
                        }
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
