#include "types.h" 
#include "user.h" 

void batchcalls(int a) 
{

}
typedef struct {
        int moveto[3];
        int lineto[3];              //struct to fill up with values
        int setpencolour[4];
        int selectpen[2];
        int fillrect[2];
    } batchcallsstruct;

int main(int argc, char* argv[]) 
{  

    int hdc = beginpaint(0);
    batchcalls(1);

    moveto(hdc, 10, 5);             // won't run straight away if we add while loop
    setpencolour(21, 3, 2, 1);
    selectpen(hdc, 21);
    lineto(hdc, 10 + 20, 5);
    lineto(hdc, 10 + 20, 5 + 20);
    lineto(hdc, 10, 5 + 20);
    lineto(hdc,10, 5); 
    
    batchcalls(0);
    endpaint(hdc);
    exit(); 
} 