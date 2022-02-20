#include "types.h" 
#include "user.h" 
 
int main() 
{
    
    int pid;
    char *argv[] = { "test2", 0};

    setvideomode(0x13);
    pid = fork();
    if (pid < 0) {
        setvideomode(0x03);
        printf(1, "test2: fork failed \n");
        exit();
    }
    if (pid == 0) {
        exec("test2", argv);
        setvideomode(0x03);
        printf(1, "test2: exec test2 failed\n");
        exit();
    }
    int hdc = beginpaint(0);
    for (int i = 0; i < 20; i++) 
    { 
        moveto(hdc, i * 10, i * 5); 
        setpencolour(i + 60, i * 3, i * 2, i); 
        selectpen(hdc, i + 60); 
        lineto(hdc, i * 10 + 20, i * 5); 
        lineto(hdc, i * 10 + 20, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5); 
        sleep(1);
    }
    endpaint(hdc);
    getch();
    setvideomode(0x03);
    wait();
    exit(); 
} 