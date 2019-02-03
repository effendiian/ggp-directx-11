# Game Graphics Programming - Starter Code #

## About the Project ##

This repository contains code created as a part of Foundations of Game Graphics Programming ([IGME 540](https://www.rit.edu/gccis/igm/)), offered by [Professor Chris Cascioli](https://www.rit.edu/gccis/igm/christopher-cascioli) at the [Rochester Institute of Technology](https://www.rit.edu).

Students were provided with basic DirectX 11 functionality, including initial versions of the following classes:

- `DXCore` class

- `Game` class

- `Main` class

- `SimpleShader` class

The project also included basic vertex and fragment/pixel shaders for basic functionality:

- `VertexShader.hlsl`

- `PixelShader.hlsl`

Throughout the semester, students were assigned different sections of the code to rewrite, refactor, revise, or build upon and expand.

> Students will explore the use of an advanced graphics API to access hardware-accelerated graphics in a real-time graphics engine context.  The course will involve discussion of scene graphs, optimizations, and integration with the API object structure, as well as input schemes, content pipelines, and 2D and 3D rendering techniques.  Students will also explore the advanced use of the API calls in production code to construct environments capable of real-time performance.  Students will construct from scratch a fully functional graphics engine, with library construction for game development.

*Course syllabus.*

## Master ##

The master branch will always be the most feature complete; the most recent assignment will be merged back into the master after submission.

## Assignment 3 - Making Things Move ##

This branch contains the work for [Assignment 3: Making Things Move](https://mycourses.rit.edu/d2l/le/content/736702/viewContent/5582574/View) (link requires access to MyCourses).

### Overview ###

[x] Create a class that represents individual **game entities** and their transformations.

[x] Ensure that Meshes can be **shared** among multiple entities.

[x] **Create** and **draw** at least 5 shared entities, with several sharing the same Mesh object.

[x] **Update** your entity transformations each frame so the entities move/scale/etc.

[x] Ensure you have no **warnings**, **memory leaks** or **DX resource leaks**.

## Binaries Not Included ##

This code allows you to build the project from your local repository after cloning. This is for a university course - this open source code is made available under an [MIT license](LICENSE).