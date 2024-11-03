#define IO volatile unsigned int *

IO gpio_data    = (IO) 0x40000000;
IO gpio_oe      = (IO) 0x40000004;

IO uart_ctrl    = (IO) 0x40000000;
IO uart_bauddiv = (IO) 0x40000004;
IO uart_status  = (IO) 0x40000008;
IO uart_data    = (IO) 0x4000000C;

IO dmac_saddr   = (IO) 0x80000000;
IO dmac_daddr   = (IO) 0x80000004;
IO dmac_ctrl    = (IO) 0x80000008;
IO dmac_scfg    = (IO) 0x8000000C;
IO dmac_dcfg    = (IO) 0x80000010;
IO dmac_cfg     = (IO) 0x80000014;
IO dmac_bcount  = (IO) 0x80000018;
IO dmac_bsize   = (IO) 0x8000001C;
IO dmac_status  = (IO) 0x80000020;

typedef struct {
    unsigned int saddr;    
    unsigned int daddr;    
    unsigned char bcount;
    unsigned char bsize;
    unsigned char sinc;
    unsigned char dinc;
    unsigned char ssize;
    unsigned char dsize;
    unsigned char wfi;
    unsigned char irqsrc;
} dmac_descriptor;

char *str = "Buddy you're a boy make a big noise\0Playin' in the street gonna be a big man some day\n";
char d[256] = {0};


void uart_init(int bauddiv){
    *uart_bauddiv = bauddiv;
    *uart_ctrl = 1;
}

void uart_putc(char c){
    while(*uart_status == 0);
    *uart_data = c;
    *uart_ctrl |= 2;
}

void uart_puts(char *s){
    for(int i=0; s[i]; i++)
        uart_putc(s[i]);
}

void dmac_start(){
    *dmac_ctrl = 1;
    *dmac_ctrl = 0;
}

void dmac_init(dmac_descriptor *d){
    *dmac_saddr = d->saddr;
    *dmac_daddr = d->daddr;
    *dmac_scfg  = d->ssize  + d->sinc*16;
    *dmac_dcfg  = d->dsize  + d->dinc*16;
    *dmac_cfg   = d->wfi    + d->irqsrc*16;
    *dmac_bcount= d->bcount;
    *dmac_bsize = d->bsize;
}

void exit(){
    *gpio_data = 0xF00FE00E;
}

dmac_descriptor dd;

int main(){
   // *gpio_oe = 0xFFFFFFFF;  // configure the GPIO as an output

    uart_init(10);
    uart_puts("\nHello World!\n\n ");

    dd.saddr = (unsigned int) str;
    dd.daddr = (unsigned int) d;
    dd.bcount = 6;
    dd.bsize = 8;
    dd.wfi = 0;
    dd.irqsrc = 0;
    dd.sinc=1;
    dd.dinc=1;
    dd.ssize=0;
    dd.dsize=0;

    dmac_init(&dd);
    dmac_start();
    while((*dmac_status) != 1);

    uart_puts(d);
    //exit();
    //while(1);
    return 0;
}
