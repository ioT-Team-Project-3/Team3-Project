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

//ENUM을 활용해서 0,1,2,3,4,5,6,7을 설정
typedef enum : int
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	OK,
	EASY,
	NORMAL,
	HARD
} UDLR;

typedef struct obstacle {

    // 0: 정지, 1: 움직임
    bool isMoving;

    // 이동한다면 (0: 우, 1: 좌) 시작
    bool direction;

    // 장애물의 좌표들
    vector<coord> crd;

} obstacle;

class Cross {
    private:
    // 플레이어 좌표
     coord currentYX;

    // 장애물들
    vector<obstacle> obstacles;

    int getRandomNum(int max) { return rand() % max; }

    public:
    Cross()
    {
        srand((unsigned int)time(NULL));
        reset();
    }

    void reset()
    {
        printf("game : resetting game\n");
        obstacles.clear();
        // coord C = {getRandomNum(8), getRandomNum(8)};
        coord C = {6, 3};
        currentYX = C;
    }

    void regenObstacle() {
        
        // 이미 장애물이 5개가 있다면 생성 안함
        if (obstacles.size() > 4) {
            return;
        }

        // 장애물이 하나라도 이미 있는 상태에서 마지막으로 생성된 장애물의 y좌표가 2보다 작다면 생성 안함
        // 즉 장애물 사이의 간격을 두기 위함
        if ((!obstacles.empty()) && (obstacles.back().crd[0].y < 2)) {
            return;
        }


        // 장애물이 이동인지 정지인지 설정
        bool moving = !!getRandomNum(2);

        if (moving) { // 움직이는 장애물이라면

            // 장애물의 길이
            int length = getRandomNum(5);

            // 좌, 우 설정
            bool dir = !!getRandomNum(2);

            if (dir) { // <- 방향으로 움직임

                // 새로운 장애물 객체 생성
                obstacle newObstacle;

                // 장애물 설정 값을 입력해줌
                newObstacle.isMoving = moving;
                newObstacle.direction = dir;
                // newObstacle.crd.clear();

                // 장애물의 길이(n)만큼 포문, 0부터 n까지
                for (int i = 0; i < length; i++) {

                    // 장애물의 좌표 벡터에 추가함
                    newObstacle.crd.push_back({0, i});
                }

                // 생성하고 설정한 장애물을 벡터에 추가함
                obstacles.push_back(newObstacle);

            } else { // -> 방향으로 움직임
                
                // 새로운 장애물 객체 생성
                obstacle newObstacle;

                // 장애물 설정 값을 입력해줌
                newObstacle.isMoving = moving;
                newObstacle.direction = dir;
                // newObstacle.crd.clear();
                
                // 장애물의 길이(n)만큼 포문, 7부터 7-n까지
                for (int i = 7; i > 7-length; i--) {

                    // 장애물의 좌표 벡터에 추가함
                    newObstacle.crd.push_back({0, i});
                }

                // 생성하고 설정한 장애물을 벡터에 추가함
                obstacles.push_back(newObstacle);
            }
            
        } else { // 움직이는 장애물이 아니라면

            // 새로운 장애물 객체 생성
            obstacle newObstacle;

            // 장애물 설정 값을 입력해줌
            newObstacle.isMoving = moving;
            newObstacle.direction = false;
            // newObstacle.crd.clear();

            // 4번의 포문
            for (int i = 0; i < 4; i++) {

                // 장애물의 좌표 벡터에 추가함
                // x좌표를 랜덤으로 뽑기 때문에 중복으로 뽑힌다면 자동으로 장애물의 칸 수 조절이 됨
                // 즉, 1~4개의 칸을 가질 수 있음
                newObstacle.crd.push_back({0, getRandomNum(7)});
            }

            // 생성하고 설정한 장애물을 벡터에 추가함
            obstacles.push_back(newObstacle);
        }
    }


    void downMovingObject(bool playerMove) {
        // 장애물들에 대한 접근
        for (int i = 0; i < obstacles.size(); i++) {

            // 각 장애물의 좌표들에 대한 접근
            for (int j = 0; j < obstacles[i].crd.size(); j++) {

                // 장애물을 아래로 이동시킴
                obstacles[i].crd[j].y += 1;

                // 이때 장애물이 맵 밖으로 사라진다면
                if (obstacles[i].crd[j].y > 7) {

                    // 장애물을 삭제함
                    obstacles.erase(obstacles.begin());
                    
                    // 위에서 벡터의 첫번째 원소를 지우기 때문에 i를 감소시켜줌
                    i--;

                    // i 번째 for문 탈출
                    break;
                }
            }
        }

        // 플레이어의 이동 입력이 없다면 장애물과 함께 뒤로 이동함
        if (!playerMove) {
            go(1);
        }
    }

    void horizonMovingObstacle() {
        // 장애물들에 대한 접근
        for (int i = 0; i < obstacles.size(); i++) {

            // 장애물이 움직이는지 확인
            if (obstacles[i].isMoving) {

                // 이동 방향 확인
                if (obstacles[i].direction) { // <- 방향으로 이동인 경우

                    // 각 장애물의 좌표들에 대한 접근
                    for (int j = 0; j < obstacles[i].crd.size(); j++) {

                        if (obstacles[i].crd[j].x == 7) { // 왼쪽 끝에 있다면
                            obstacles[i].crd[j].x = 0; // 오른쪽 끝에서 다시 생성

                        } else { // 왼쪽 끝이 아니라면
                            obstacles[i].crd[j].x += 1; // 왼쪽으로 한 칸 이동
                        }
                    }
                } else { // -> 방향으로 이동인 경우

                    // 각 장애물의 좌표들에 대한 접근
                    for (int j = 0; j < obstacles[i].crd.size(); j++) {

                        if (obstacles[i].crd[j].x == 0) { // 오른쪽 끝에 있다면
                            obstacles[i].crd[j].x = 7; // 왼쪽 끝에서 다시 생성

                        } else { // 오른쪽 끝이 아니라면
                            obstacles[i].crd[j].x -= 1; // 오른쪽으로 한 칸 이동   
                        }
                    }
                }
            }
        }
    }

    //향하는 방향으로 움직임.
    bool go(int heading)
    {
        coord ret = currentYX;
        switch (heading)
        {
        case UP:
            if (ret.y == 0) {
                break;
            } else {
                ret.y -= 1;
            }
            break;

        case DOWN:
            ret.y += 1;
            break;

        case LEFT:
            if (ret.x == 7) {
                break;
            } else {
                ret.x += 1;
            }
            break;

        case RIGHT:
            if (ret.x == 0) {
                break;
            } else {
                ret.x -= 1;
            }
            break;

        default:
            break;
        }

        currentYX = ret;

        return false;
    }


    bool isPlaying() { 

        // 플레이가 맵 아래로 이탈하지 않았는 지 확인
        if (currentYX.y < 8) {

            // 장애물들에 대한 접근
            for (int i = 0; i < obstacles.size(); i++) {

                // 각 장애물의 좌표들에 대한 접근
                for (int j = 0; j < obstacles[i].crd.size(); j++) {

                    // 플레이어와 장애물이 충돌했는지 확인
                    if ((currentYX.x == obstacles[i].crd[j].x) && (currentYX.y == obstacles[i].crd[j].y)) {

                        // 겹쳤다면 false
                        return false;
                    }
                }
            }

            // 겹치지 않았다면 true
            return true;
        }

        // 맵 아래로 이탈 했다면 false
        return false;
    }


    coord get() { return currentYX; }

    vector<obstacle> getObstacle() { return obstacles; }
};