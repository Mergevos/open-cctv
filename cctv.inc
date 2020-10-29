#include <a_samp>
#include <YSI_Data\y_foreach>
#include <YSI_Coding\y_hooks>
#include <streamer>

#if !defined MAX_CCTVS
    #define MAX_CCTVS 256
#endif

static 
    Iterator: cctv_Iter<MAX_CCTVS>,
    Float: cctv_Pos[MAX_CCTVS][3],
    Float: cctv_Rot[MAX_CCTVS][3],
    Float: cctv_LookPos[MAX_CCTVS][3],
    cctv_Interior[MAX_CCTVS], 
    cctv_VirtualWorld[MAX_CCTVS],
    cctv_Handle[MAX_CCTVS];

//--------------------------------------
hook OnPlayerConnect(playerid)
{
    SetPVarInt(playerid, "IsWatching", -1);
    return 1;
}

//-------------------------------------------
/*
 * <summary> Checks if player is watching any of the cameras</summary>
 * <param name="playerid"> Player whom we are checking</param>
 */ 

stock bool: CCTV_IsPlayerWatchingAny(playerid)
{
    return (GetPVarInt(playerid, "IsWatching") != -1) ? (true) : (false);
}

/*
 * <summary> Checks if player is watching the cameras</summary>
 * <param name="playerid"> Player whom we are checking</param>
 * <param name="cctvid"> Cctv which is watched by player</param>
 */ 


stock bool: CCTV_IsPlayerWatching(playerid, cctvid)
{
    return (GetPVarInt(playerid, "IsWatching") == cctvid) ? (true) : (false);
}

/*
 * <summary> Checks if player is near the camera</summary>
 * <param name="playerid"> Player whom we are checking</param>
 * <param name="cctvid"> The cctv we are checking if player is near to</param>
 */ 

stock CCTV_IsPlayerNear(playerid, cctvid)
{
    return IsPlayerInRangeOfPoint(playerid, 2.0, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);

}

//----------------------------------------

/*
 * <summary> Creates a camera</summary>
 * <param name="x"> The X coordinate of the camera</param>
 * <param name="y"> The Y coordinate of the camera</param>
 * <param name="z"> The Z coordinate of the camera</param>
 * <param name="rotX"> The X rotation of the camera</param>
 * <param name="rotY"> The Y rotation of the camera</param>
 * <param name="rotZ"> The Z rotation of the camera</param>
 * <param name="lookX"> The X coordinate of the camera looking</param>
 * <param name="lookY"> The Y coordinate of the camera looking</param>
 * <param name="lookX"> The Z coordinate of the camera looking</param>
 * <param name="virtualworld"> The virtualworld of the camera</param>
 * <param name="interiorid"> The interior of the camera</param>
 */ 

stock CCTV_Create(Float: x, Float: y,  Float: z, Float: rotX, Float: rotY, Float: rotZ, Float: lookX, Float: lookY, Float: lookZ, virtualworld, interiorid)
{
    new id = Iter_Alloc(cctv_Iter);

    cctv_Handle[id] = CreateDynamicObject(2921, x, y, z, rotX, rotY, rotZ, virtualworld, interiorid);

    cctv_Interior[id] = interiorid, 
    cctv_VirtualWorld[id] = virtualworld;
    cctv_Pos[id][0] = x,
    cctv_Pos[id][1] = y, 
    cctv_Pos[id][2] = z;
    cctv_Rot[id][0] = rotX, 
    cctv_Rot[id][1] = rotY, 
    cctv_Rot[id][2] = rotZ;
    cctv_LookPos[id][0] = lookX, 
    cctv_LookPos[id][1] = lookY, 
    cctv_LookPos[id][2] = lookZ;
    return id;
}

/*
 * <summary> Makes a player watch the camera</summary>
 * <param name="playerid"> Playerid we are making to watch the camera</param>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Watch(playerid, cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || !IsValidDynamicObject(cctv_Handle[cctvid]) || CCTV_IsPlayerWatchingAny(playerid)) {
        return 0;
    }
    SetPlayerCameraPos(playerid, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);
    SetPlayerCameraLookAt(playerid, cctv_LookPos[cctvid][0], cctv_LookPos[cctvid][1], cctv_LookPos[cctvid][2], CAMERA_CUT);
    TogglePlayerControllable(playerid, false);
    SetPVarInt(playerid, "IsWatching", cctvid);
    return 1;
}

/*
 * <summary> Makes a player stop watching the camera</summary>
 * <param name="playerid"> Playerid we are making to unwatch the camera</param>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Unwatch(playerid, cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || !IsValidDynamicObject(cctv_Handle[cctvid]) || !CCTV_IsPlayerWatchingAny(playerid)) {
        return 0;
    }
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetPVarInt(playerid, "IsWatching", -1);
    return 1;
}

/*
 * <summary> Destroys the camera</summary>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Destroy(cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || !IsValidDynamicObject(cctv_Handle[cctvid])) {
        return 0;
    }
    Iter_Remove(cctv_Iter, cctvid);
    DestroyDynamicObject(cctv_Handle[cctvid]);
    return 1;
}

