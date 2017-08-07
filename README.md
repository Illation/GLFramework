# GLFramework

**Work in this repository has been discontinued. The project lives on in a new repository, as it has been merged with my planet renderer:**
https://github.com/Illation/ETEngine

Framework using opengl to later use for personal projects

## Acknowledgments
Currently used libraries: SDL2, GLM, DevIL, stb_image, GLAD and Assimp

for library licenses see Library Licenses.md

Structure inspired by the OverlordEngine from [Digital Arts and Entertainment](http://www.digitalartsandentertainment.be/)

## Features:
* Basic Physically Based Rendering
* HighDynamicRange Environment maps
* Image Based Lighting
* Deferred and Forward rendering
* Lights rendered as Light Volumes
* Cascaded Shadow Maps
* Bloom with texture downsampling
* Frustum Culling
* Text Rendering
* Stereoscopic Normal map encoding and decoding for optimal use of G-Buffer
* Buffer Display mode
* Scenegraph Entity Hirachy 
* Entity Component System 
* Singleton Pattern for things like WINDOW, INPUT, TIME, CAMERA etc 
* Observer Pattern for input 
* Factory Pattern for Content Managment 
* Mesh Filter -> Vertex buffer building depending on material needs

### G-Buffer Layout
| R Channel   | G Channel   | B Channel   | A Channel   | 
|:-----------:|:-----------:|:-----------:|:-----------:| 
| Position .x | Position .y | Position .z | AO       .x |   
| Normal   .x | Normal   .y | Metalness.x | Specular .x |   
| BaseColor.r | BaseColor.g | BaseColor.b | Roughness.x |  

### Demo Video
[![Link](https://img.youtube.com/vi/W6tWtsQlZKo/0.jpg)](https://www.youtube.com/watch?v=W6tWtsQlZKo)

## Screenshots:
### Combined buffer
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/CombinedShadowBloom.JPG "")
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/CSM2.JPG "")

### IBL
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/IblCombined.jpg "")

### Roughness
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/Roughness.JPG "")

### Spamming point lights
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/LightSpam.gif "")
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/bloooooooom.JPG "")

### Buffer display mode
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/Buffers.JPG "")

### Legacy Shader:
For comparison of rendering quality between Legacy (Blinn-Phong Shading) and PBR
![alt text](https://github.com/Illation/GLFramework/blob/master/Screenshots/Combined.JPG "")
