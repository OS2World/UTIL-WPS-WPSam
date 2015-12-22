#define INCL_VIO

#include <os2.h>

int main (void)
    {
    VIOINTENSITY vioi;
    VIOSETULINELOC viou;
    ULONG i, j;
    BYTE byte[32];

    vioi.cb = 6;
    vioi.type = 2;
    vioi.fs = 1;
    VioSetState (&vioi, 0);

    viou.cb = 6;
    viou.type = 5;
    viou.scanline = 3;
    VioSetState (&viou, 0);

    for (i=0; i<0xf; i++)
        {
        for (j=0; j<0xf; j++)
            {
            byte[2*j] = 'x';
            byte[2*j+1] = (i<<4)+j;
            }
        VioWrtCellStr (byte, 32, i, 0, 0);
        }
    return 0;
    }
