# open-cctv

[![Mergevos](https://img.shields.io/badge/Mergevos-open--cctv-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/open-cctv)

This library allows you to create a CCTV cameras and watch them.

## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/open-cctv
```

Include in your code and begin using the library:

```pawn
#include <cctv>
```

## Usage

There are not too many functions. They are listed below.

This function creates a camera.

```c
bool: CCTV_Create(Float: x, Float: y,  Float: z, Float: rotX, Float: rotY, Float: rotZ, Float: lookX, Float: lookY, Float: lookZ, virtualworld, interiorid)
```

Where arguments above are:
```
x - The X coordinate of the camera
y - The Y coordinate of the camera
z - The Z coordinate of the camera
rotX - The X rotation of the camera
rotY - The Y rotation of the camera
rotZ - The Z rotation of the camera
lookX - The X coordinate of the camera looking
lookY - The Y coordinate of the camera looking
lookX - The Z coordinate of the camera looking
virtualworld - The virtualworld of the camera
interiorid - The interior of the camera
```

This function destroys a camera.

```c
CCTV_Destroy(cctvid)
```

```
cctvid - ID of the camera
```

Makes a player watch the camera

```c
CCTV_Watch(playerid, cctvid)
```

```
playerid - Playerid we are making to watch the camera
cctvid - Camera ID
```

```c
CCTV_Unwatch(playerid, cctvid)
```

```
Makes a player stop watching the camera
playerid"> Playerid we are making to unwatch the camera
cctvid - Camera ID 
```

## Testing

To test, configure pawn.json and test.pwn then simply run the package:

```bash
sampctl package run
```
