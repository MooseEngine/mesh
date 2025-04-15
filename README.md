# Mesh
The goal of this project is to take our ability to render a single triangle in a 3d space and now render multiple triangles together in the form of a **mesh**. We're going to start with a cube, which I believe should be 12 triangles (2 for each side). Or, perhaps it will be just 8 vertices? I know not, but we shall find out.

To do this, we're going to need to extend our code a bit:
* Compose a *mesh* instead of just a single triangle.
* I'll also implement **backface culling**, which is a method of skipping rendering triangles that are facing away from the camera, thereby reducing load on the rasterizer and preventing visual artifacts.
* **Depth buffering**, which is a method of determining whether one pixel should be drawn above another.

## Mesh Composition

## Depth buffering

## Backface Culling

