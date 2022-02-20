#include "types.h" 
#include "user.h" 
 
int main(int argc, char* argv[]) 
{
    setvideomode(0x13);
    int hdc = beginpaint(0);
    struct rect rect;
    rect.top = 70;
    rect.bottom = 100;
    rect.left = 20;
    rect.right = 250;
    selectpen(0,10);
    fillrect(hdc, &rect);
    endpaint(hdc);
    getch();
    setvideomode(0x03);
    exit(); 
} 