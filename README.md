# samp-cctv

[![Mergevos](https://img.shields.io/badge/Mergevos-samp--cctv-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/samp-cctv)

## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/samp-cctv
```

Include in your code and begin using the library:

```pawn
#include <cctv>
```

## Usage

There're not too many functions. They're listed below.

This function creates a camera.

```c
CCTV_Create(Float: x, Float: y,  Float: z, Float: rotX, Float: rotY, Float: rotZ, Float: lookX, Float: lookY, Float: lookZ, virtualworld, interiorid)
```

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


## Testing

To test, configure pawn.json and test.pwn then simply run the package:

```bash
sampctl package run
```
