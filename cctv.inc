/*
	* Library: Samp CCTV (cctv.inc)
	* Author: Mergevos
	* Date: 5. Novembre 2020.
	* Copyright: (C) 2020
	* Credits: 
        samp-incognito - Streamer
        Y_Less - y_iterate * y_hooks
        tr1xy - help about iterating
*/

// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_cctv
    #undef _inc_cctv
#endif
// custom include-guard to ensure we don't duplicate
#if defined _cctv_included
    #endinput
#endif
#define _cctv_included


#include <a_samp>
#include <YSI_Data\y_foreach>
#include <YSI_Coding\y_hooks>
#include <streamer>
#include <logging>

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
    #if !defined CCTV_NO_LOG
        Log_ToggleHandle("CCTV", true);
    #endif
    #if !defined CCTV_NO_INFO
       printf("[CCTV version 1.0.1 by Mergevos loaded]");
    #endif
    return 1;
}

hook OnPlayerConnect(playerid)
{
    SetPVarInt(playerid, "IsWatching", -1);
    return Y_HOOKS_CONTINUE_RETURN_1;
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

stock CCTV_IsPlayerNear(playerid, cctvid, range = 2.0)
{
    return IsPlayerInRangeOfPoint(playerid, range, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);

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
    new 
        id = Iter_Alloc(cctv_Iter);
        
    if(id == INVALID_ITERATOR_SLOT) {
        Log_Error("CCTV", "Invalid iterator slot.", Log_Field_i("ID", id));
        return -1;
    }

    cctv_Handle[id] = CreateDynamicObject(2921, x, y, z, rotX, rotY, rotZ, virtualworld, interiorid);
    if(!IsValidDynamicObject(cctv_Handle[id])) {
        Log_Error("CCTV", "Invalid dynamic object.", Log_Field_i("ID", id));
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
    Log_Info("CCTV", "CCTV Created.", Log_Field_i("CCTV ID", id), Log_Field_f("X", x), Log_Field_f("Y", y), Log_Field_f("Z", z), Log_Field_f("rotX", rotX), Log_Field_f("rotY", rotY), Log_Field_f("rotZ", rotZ), Log_Field_f("lookX", lookX), Log_Field_f("lookY", lookY),  Log_Field_f("lookZ", lookZ), Log_Field_i("virtualworld", virtualworld), Log_Field_i("interiorid", interiorid));

    return id;
}

/*
 * <summary> Makes a player watch the camera</summary>
 * <param name="playerid"> Playerid we are making to watch the camera</param>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Watch(playerid, cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || CCTV_IsPlayerWatchingAny(playerid)) {
        Log_Error("CCTV", "CCTV doesn't exist or player is not watching some.", Log_Field_i("playerid", playerid), Log_Field_i("CCTV ID", cctvid));
        return -1;
    }
    SetPlayerCameraPos(playerid, cctv_Pos[cctvid][0], cctv_Pos[cctvid][1], cctv_Pos[cctvid][2]);
    SetPlayerCameraLookAt(playerid, cctv_LookPos[cctvid][0], cctv_LookPos[cctvid][1], cctv_LookPos[cctvid][2], CAMERA_CUT);
    TogglePlayerControllable(playerid, false);
    SetPVarInt(playerid, "IsWatching", cctvid);
    Log_Info("CCTV", "CCTV started being watched.", Log_Field_i("playerid", playerid), Log_Field_i("CCTV ID", cctvid));
    return 1;
}

/*
 * <summary> Makes a player stop watching the camera</summary>
 * <param name="playerid"> Playerid we are making to unwatch the camera</param>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Unwatch(playerid, cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid) || !CCTV_IsPlayerWatchingAny(playerid)) {
        Log_Error("CCTV", "CCTV doesn't exist or player is not watching any.", Log_Field_i("playerid", playerid), Log_Field_i("CCTV ID", cctvid));
        return -1;
    }
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetPVarInt(playerid, "IsWatching", -1);
    Log_Info("CCTV", "CCTV stopped being watched.", Log_Field_i("playerid", playerid), Log_Field_i("CCTV ID", cctvid));
    return 1;
}

/*
 * <summary> Destroys the camera</summary>
 * <param name="cctvid"> Camera ID</param>
 */ 

stock CCTV_Destroy(cctvid)
{
    if(!Iter_Contains(cctv_Iter, cctvid)) {
        Log_Error("CCTV", "CCTV doesn't exist.", Log_Field_i("CCTV ID", cctvid));
        return -1;
    }
    Iter_Remove(cctv_Iter, cctvid);
    DestroyDynamicObject(cctv_Handle[cctvid]);
    Log_Info("CCTV", "CCTV destroyed, object removed.", Log_Field_i("CCTV ID", cctvid));
    return 1;
}

