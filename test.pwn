#include <open.mp>
#include "cctv.inc"

#include <YSI_Visual\y_commands>

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

CMD:watch(playerid, params[])
{
    CCTV_Watch(playerid, cctvid);
    return 1;
}

CMD:unwatch(playerid, params[])
{
    CCTV_Unwatch(playerid);
    return 1;
}
