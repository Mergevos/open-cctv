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

<summary> Creates a camera</summary>
<param name="x"> The X coordinate of the camera</param>
<param name="y"> The Y coordinate of the camera</param>
<param name="z"> The Z coordinate of the camera</param>
<param name="rotX"> The X rotation of the camera</param>
<param name="rotY"> The Y rotation of the camera</param>
<param name="rotZ"> The Z rotation of the camera</param>
<param name="lookX"> The X coordinate of the camera looking</param>
<param name="lookY"> The Y coordinate of the camera looking</param>
<param name="lookX"> The Z coordinate of the camera looking</param>
<param name="virtualworld"> The virtualworld of the camera</param>
<param name="interiorid"> The interior of the camera</param>


stock CCTV_Create(Float: x, Float: y,  Float: z, Float: rotX, Float: rotY, Float: rotZ, Float: lookX, Float: lookY, Float: lookZ, virtualworld, interiorid)


## Testing

To test, configure pawn.json and test.pwn then simply run the package:

```bash
sampctl package run
```
