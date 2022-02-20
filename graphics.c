#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "spinlock.h"

static struct {
    struct spinlock lock;
    int locking;
} graphics;

uchar* adr = (uchar*)P2V(0xA0000);

typedef struct {
    int pen;
    int position[2];
} devicecontext;


devicecontext dc[8] = {
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}},
    {0,{0,0}}
};

void graphicsinit(void) {
    initlock(&graphics.lock, "devicecontext");
}


int devicecontextinit(void) {
    int hdc = -1;
    for (int i = 0; i < 8; i++) {
        if (dc[i].pen == 0 && dc[i].position[0] == 0 && dc[i].position[1] == 0) {
            hdc = i;
            break;
        }
    }
    return hdc;
}

void clear320x200x256() {
    unsigned short offset;

	uchar *adr;                         //function to clear all of the pixels
    adr = (uchar*)P2V(0xA0000);
    for (int x=0; x < 320; x++) {
        for (int y=0; y < 200; y++) {
            offset = 320 * y + x;
            adr[offset] = 0x0;
        }
    }
}

int sys_setpixel(void) {
    
    acquire(&graphics.lock);
    int hdc;
    int x;
    int y;

    argint(0, &hdc);
    argint(2, &y);
    argint(1, &x);

    ushort offset;

	offset = 320 * y + x;
    adr[offset] = dc[hdc].pen;

    release(&graphics.lock);
	return 0;
}

int sys_moveto(void) {

    int hdc;
    int x;
    int y;
    acquire(&graphics.lock); //initiate lock due to modification to our array of structs
    argint(0, &hdc);
    argint(2, &y);
    argint(1, &x);
    if (x>320) {
        x = 320;
    }
    if (y>200) {
        y = 200;
    }
    dc[hdc].position[0] = x;
    dc[hdc].position[1] = y; //array of structs initialised with x,y
    release(&graphics.lock); //release our lock

    return 0;
    
}

int sys_lineto(void) {


    int x,y,dx,dy,swap,temp,s1,s2,p,i;
    int x1,y1,x2,y2;
    int hdc;
    unsigned short offset;

    acquire(&graphics.lock);
    argint(0, &hdc);
    argint(1, &x2);
    argint(2, &y2);
    x1 = dc[hdc].position[0];
    y1 = dc[hdc].position[1];
    
    x=x1;                           //Bresenham's line algorithm
    y=y1;

    dx=x2-x1;
    dx = dx * ((dx>0) - (dx<0));
    dy=y2-y1;
    dy = dy * ((dy>0) - (dy<0));

    s1=x2-x1;
    s1=(s1 > 0) - (s1 < 0);
    s2=y2-y1;
    s2=(s2 > 0) - (s2 < 0);

    swap=0;
    offset = 320 * y1 + x1;
    adr[offset] = dc[hdc].pen;

    if(dy>dx) {
    temp=dx;
    dx=dy;
    dy=temp;
    swap=1;
    }

    p=2*dy-dx;

    for(i=0;i<dx;i++) {
        offset = 320 * y + x;
        adr[offset] = dc[hdc].pen;
        while(p>=0) {
            p=p-2*dx;
            if(swap) {
                x+=s1;
            }
            else {
                y+=s2;
            }
 
        }
        p=p+2*dy;
        if(swap) {
            y+=s2;
        }

        else {
            x+=s1;
        }
    }
    offset = 320 * y2 + x2;
    adr[offset] = dc[hdc].pen;
    dc[hdc].position[0] = x2;
    dc[hdc].position[1] = y2;
    release(&graphics.lock);

    return 0;
}

int sys_setpencolour(void) {

    int index, red, green, blue;

    acquire(&graphics.lock);
    argint(0, &index);
    argint(1, &red);
    argint(2, &green);
    argint(3, &blue);
    if (index < 16 || index > 255) {
        return -1;
    } 
    outb(0x3C8, index);
    outb(0x3C9, red);
    outb(0x3C9, green);
    outb(0x3C9, blue);
    release(&graphics.lock);
    return 0;
 
}
 
int sys_selectpen(void) {

    int pen;
    int hdc;
    acquire(&graphics.lock);
    argint(0, &hdc);
    argint(1, &pen);
    dc[hdc].pen = pen;
    release(&graphics.lock);
    return 0;
}

int sys_fillrect(void) {

    int hdc;    
    uchar *adr;
    adr = (uchar*)P2V(0xA0000);
    ushort offset;

    struct rect {
        int top;
        int left;
        int bottom;
        int right;
    };

    acquire(&graphics.lock);
    struct rect *rect;
    argptr(1, (void*)&rect, sizeof(*rect));
    argint(0,&hdc);
    
    for (int y=rect->top;y < rect->bottom; y++) {
        for (int x = rect->left; x < rect->right; x++) {
        offset = 320 * y + x;
        adr[offset] = dc[hdc].pen;
        }
    }
    release(&graphics.lock);
    return 0;
}

int sys_beginpaint(void) {

    acquire(&graphics.lock);
    int hdc = devicecontextinit();
    release(&graphics.lock);

    if (hdc >= 0) {
        return hdc;
    }
    else {
        return -1;
    }

}

int sys_endpaint(void)  {


    int hdc;
    acquire(&graphics.lock);
    argint(0, &hdc);
    dc[hdc].pen = 0;
    dc[hdc].position[0] = 0;
    dc[hdc].position[1] = 0;
    release(&graphics.lock);
    return 0;
}

