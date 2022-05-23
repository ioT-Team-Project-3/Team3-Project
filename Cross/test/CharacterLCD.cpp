#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define CLCD "/dev/clcd"
using namespace std;

class CharacterLCD
{
private:
    int clcd_fd;

    // CharacterLCD에 출력
    void print(string S)
    {
        char *cstr = new char[S.length() + 1];
        strcpy(cstr, S.c_str());
        clcd_fd = open(CLCD, O_WRONLY);
        write(clcd_fd, cstr, 32);
        close(clcd_fd);
    }

public:
    void beforeGame()
    {
        string s1 = "   3jo Game   ";
        string s2 = "   Press Start  ";
        print(s1 + s2);
    }

    void gaming()
    {
        string s1 = "game";
        string s2 = "Start";
        print(s1 + s2);
    }

    void gameOver()
    {
        string s1 = "haha";
        string s2 = "   Game Over    ";
        print(s1 + s2);
    }
};
