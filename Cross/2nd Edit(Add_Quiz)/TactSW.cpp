//
//  TactSW.cpp
//  Snake
//
//  Created by Jinwoo on 2020/11/25.
//
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

static char tactswDev[] = "/dev/tactsw";

class TactSW
{
private:
public:
    int get()
    {
        unsigned char b;
        int tactswFd = -1;
        tactswFd = open(tactswDev, O_RDONLY);
        read(tactswFd, &b, sizeof(b));
        close(tactswFd);
        switch (b)
        {
        case 2:
            return 0;
        case 4:
            return 2;
        case 5:
            return 4;
        case 6:
            return 3;
        case 8:
            return 1;

	    // 난이도 조절용 TactSW 
	    case 10:
		    return 5;
	    case 11:
		    return 6;
	    case 12:
		    return 7;

        // OX 퀴즈용 TactSW
        case 1:
            return 8;
        case 3:
            return 9;

        default:
            return -1;
        }
    }
};
