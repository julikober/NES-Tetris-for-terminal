#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void)
{
    struct winsize w;
    struct winsize w2;
    for (;;)
    {
        ioctl(0, TIOCGWINSZ, &w);
        if (w.ws_row == w2.ws_row && w.ws_col == w2.ws_col)
        {
            continue;
        }
        else
        {
            w2 = w;
        }
        std::cout << w2.ws_row << " " << w2.ws_col << std::endl;
    }
}