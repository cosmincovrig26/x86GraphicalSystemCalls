#include "types.h" 
#include "user.h" 
 
int main(int argc, char* argv[]) 
{
    int hdc = beginpaint(0);
    for (int i = 0; i < 16; i++) 
    { 
     moveto(hdc, i * 10, i * 5 + 8); 
     setpencolour(i + 40, i * 2, i * 3, i * 4); 
     selectpen(hdc, i + 40); 
     lineto(hdc, i * 10 + 20, i * 5 + 8); 
     lineto(hdc, i * 10 + 20, i * 5 + 28); 
     lineto(hdc, i * 10, i * 5 + 28); 
     lineto(hdc, i * 10, i * 5 + 8); 
     sleep(1);
    }
    endpaint(hdc);
    exit(); 
} 