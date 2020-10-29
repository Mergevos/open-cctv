#include <a_samp>
#include <original-zcmd>
#include "./cctv.inc"

main()
{
    return;
}
static cctvid; 
public OnGameModeInit()
{
    
    cctvid = 
    CCTV_Create(134.9397,-72.9322,1.4297, 0, 0, 0, 138.0, -73, 2, 0,0);

    return 1;
}
