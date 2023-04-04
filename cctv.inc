/*
	* Library: open.mp CCTV (cctv.inc)
	* Author: Mergevos
	* Date: 5. Novembre 2020.
	* Copyright: (C) 2020
	* Credits: 
        samp-incognito - Streamer
        Y_Less - y_iterate * y_hooks
        tr1xy - help about iterating
*/
#if defined _cctv_included
    #endinput
#endif
#define _cctv_included


#include <open.mp>
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

hook OnScriptInit()
{
    printf("[CCTV version 1.0.1 by Mergevos loaded]");
    return 1;
}

hook OnScriptExit()
{
    printf("[CCTV version 1.0.1 by Mergevos unloaded]");
    return 1;
}

hook OnPlayerConnect(playerid)
{
    SetPVarInt(playerid, "IsWatching", -1);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

//-------------------------------------------
/*
 * <summary> Checks if player is watching any camera.</summary>
 * <param name="playerid"> Player whom we are checking.</param>
 * <param name="cctvid"> Camera watched by player.</param>
 * <seealso name="CCTV_IsPlayerWatching" />
 * <seealso name="CCTV_IsPlayerNear" />
 * <returns>Returns true if player is watching the camera, otherwise false.</returns>
 */ 

stock bool: CCTV_IsPlayerWatchingAny(playerid)
{
    return (GetPVarInt(playerid, "IsWatching") != -1) ? (true) : (false);
}

/*
 * <summary> Checks if player is watching specified camera.</summary>
 * <param name="playerid"> Player whom we are checking.</param>
 * <param name="cctvid"> The camera we are checking if player is watching it.</param>
 * <seealso name="CCTV_IsPlayerWatchingAny" />
 * <seealso name="CCTV_IsPlayerNear" />
 * <returns>Returns true if player is watching the camera, otherwise false.</returns>
 */ 
stock bool: CCTV_IsPlayerWatching(playerid, cctvid)
{
    return (GetPVarInt(playerid, "IsWatching") == cctvid) ? (true) : (false);
}

/*
 * <summary> Checks if player is near the camera.</summary>
 * <param name="playerid"> Player whom we are checking.</param>
 * <param name="cctvid"> The camera we are checking if player is near to.</param>
 * <seealso name="CCTV_IsPlayerWatching" />
 * <seealso name="CCTV_IsPlayerWatchingAny" />
 * <returns>Returns true if player is near specified camera, otherwise false.</returns>
 */ 
stock bool: CCTV_IsPlayerNear(playerid, cctvid, Float: range = 2.0)
{
    return IsPlayerInRangeOfPoint(playerid, range, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);

}

//----------------------------------------

/*
 * <summary> Creates a camera.</summary>
 * <param name="x"> The X coordinate of the camera.</param>
 * <param name="y"> The Y coordinate of the camera.</param>
 * <param name="z"> The Z coordinate of the camera.</param>
 * <param name="rotX"> The X rotation of the camera.</param>
 * <param name="rotY"> The Y rotation of the camera.</param>
 * <param name="rotZ"> The Z rotation of the camera.</param>
 * <param name="lookX"> The X coordinate of the camera looking.</param>
 * <param name="lookY"> The Y coordinate of the camera looking.</param>
 * <param name="lookX"> The Z coordinate of the camera looking.</param>
 * <param name="virtualworld"> The virtualworld of the camera.</param>
 * <param name="interiorid"> The interior of the camera.</param>
 * <seealso name="CCTV_Destroy" />
 * <returns>Returns camera id if function is executed successfully, otherwise -1, meaning the camera hasn't been created for some reason.</returns>
 */ 
stock CCTV_Create(Float: x, Float: y,  Float: z, Float: rotX, Float: rotY, Float: rotZ, Float: lookX, Float: lookY, Float: lookZ, virtualworld, interiorid)
{
    new 
        id = Iter_Alloc(cctv_Iter);
        
    if(id == INVALID_ITERATOR_SLOT) 
    {
        return -1;
    }

    cctv_Handle[id] = CreateDynamicObject(2921, x, y, z, rotX, rotY, rotZ, virtualworld, interiorid);
    if(!IsValidDynamicObject(cctv_Handle[id])) 
    {
        Iter_Remove(cctv_Iter, id);
        return -1;
    }
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
 * <summary> Makes a player watch the camera.</summary>
 * <param name="playerid"> Player we are making to watch from the camera.</param>
 * <param name="cctvid"> Camera we are starting to watch.</param>
 * <seealso name="CCTV_Unwatch" />
 * <returns>Returns true if function is executed successfully, otherwise false, meaning the specified player is watching from a camera or specified camera doesn't exist.</returns>
 */ 
stock bool: CCTV_Watch(playerid, cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || CCTV_IsPlayerWatchingAny(playerid)) {
        return false;
    }
    SetPlayerCameraPos(playerid, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);
    SetPlayerCameraLookAt(playerid, cctv_LookPos[cctvid][0], cctv_LookPos[cctvid][1], cctv_LookPos[cctvid][2], CAMERA_MOVE);
    TogglePlayerControllable(playerid, false);
    SetPVarInt(playerid, "IsWatching", cctvid);
    return true;
}

/*
 * <summary> Makes a player stop watching the camera.</summary>
 * <param name="playerid"> Player we are making to unwatch the camera.</param>
 * <param name="cctvid"> Camera we are stooping to watch.</param>
 * <seealso name="CCTV_Watch" />
 * <returns>Returns true if function is executed successfully, otherwise false, meaning the specified player isn't watching from any camera.</returns>
 */ 
stock bool: CCTV_Unwatch(playerid)
{
    if(!CCTV_IsPlayerWatchingAny(playerid)) {
        return false;
    }
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetPVarInt(playerid, "IsWatching", -1);
    return true;
}

/*
 * <summary> Destroys the camera.</summary>
 * <param name="cctvid"> Camera we are destroying.</param>
 * <seealso name="CCTV_Create" />
 * <returns>Returns true if function is executed successfully, otherwise false, meaning the specified camera doesn't exist.</returns>
 */ 
stock bool: CCTV_Destroy(cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid)) {
        return false;
    }
    Iter_Remove(cctv_Iter, cctvid);
    DestroyDynamicObject(cctv_Handle[cctvid]);
    return true;
}
