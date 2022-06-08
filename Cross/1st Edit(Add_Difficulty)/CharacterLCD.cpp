#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>

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

    string scoreSpacer(int I, int space)
    {
        string STR;
        stringstream ss;  
        ss << I;  
        ss >> STR;  

        // string STR = to_string(I);
        for (int i = STR.length(); i < space; i++)
        {
            STR = '0' + STR;
        }
        return STR;
    }

public:
    void beforeGame()
    {
        string s1 = "   Cross Game   ";
        string s2 = "   Press Start  ";
        print(s1 + s2);
    }

    void choose_dif()
    {
	string s1 = "   Choose Dif   ";
	string s2 = "Easy Normal Hard";
	print(s1 + s2);
    }

    void gaming(int highScore)
    {
        string s1 = "   Cross Game   ";
        string s2 = "High : " + scoreSpacer(highScore, 4) + "  ";
        print(s1 + s2);
    }

    void gameOver(int score)
    {
        string s1 = "Score   : " + scoreSpacer(score, 4) + "  ";
        string s2 = "   Game Over    ";
        print(s1 + s2);
    }
};
