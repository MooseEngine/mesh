# Matrices
## Homogenous Coordinates
<!-- TODO: learn about these boyos -->
Honestly , I have no idea what the deal is with these, but this is the general idea:
* We express a 3D point $(x, y, z)$ as a 4D vector $(x, y, z, w)$.
* Expressing it as a 4D vector makes it easier to perform **perspective division** later on.
    * This converts your stuff back to 3D (or 2D after projection)

## Transformation Matrices
There are 3 main types of transformations we'll be using in this project:
1. Model Transformations
2. View Transformation (Camera)
3. Projection Transformation

### 1. Model Transformations
Converts object (model) space into world space. As an example, you might want to model a cube. To do this, you might give it vertices at the following positions:
* (-1, -1, -1)
* (1, -1, -1)
* (1, 1, -1)
* (-1, 1, -1)
* (-1, -1, 1)
* (1, -1, 1)
* (1, 1, 1)
* (-1, 1, 1)

These vertices define its shape, but only relative to its own coordinate system. But consider that you want this cube to be in a different coordinate system, or rotated 32.4 degrees in some direction. All of a sudden, these coodinates don't work anymore. To make it the way you want, you'll use **Model Transformations**:

**Translation**: If you want to move the cube so it's center is at some new position $(tx, ty, tz)$ rather than the origin.

$$$
\[
T = \begin{bmatrix}
1 & 0 & 0 & t_x \\
0 & 1 & 0 & t_y \\
0 & 0 & 1 & t_z \\
0 & 0 & 0 & 1 
\end{bmatrix}
\]
$$$

**Scaling**: If you want the cube to be a different size, multiply them by a factor of $s_x, s_y,$ or $x_z$.

$$$
\[
S = \begin{bmatrix}
s_x & 0   & 0   & 0 \\
0   & s_y & 0   & 0 \\
0   & 0   & s_z & 0 \\
0   & 0   & 0   & 1 
\end{bmatrix}
\]
$$$

**Rotation**: If you want to rotate the cube by some amount.

$$$
\[
R_z = \begin{bmatrix}
\cos \theta & -\sin \theta & 0 & 0 \\
\sin \theta & \cos \theta  & 0 & 0 \\
0           & 0            & 1 & 0 \\
0           & 0            & 0 & 1 
\end{bmatrix}
\]
$$$

**Shearing**: This matrix skews an object by a shear factor $k$ along the x axis.

$$$
\[
H_x = \begin{bmatrix}
1 & k & 0 \\
0 & 1 & 0 \\
0 & 0 & 1 
\end{bmatrix}
\]
$$$

### 2. View Transformation (Camera)
This one positions and orients the camera in the world. You build this from a `LookAt` matrix, which is built from:
* Camera's position
* A target
* An "up" vector

### 3. Projection Transformations
So now you have a 3D camera space, but you need to display it on a 2D screen and preserve perspective (objects farther away appear smaller, etc).

A common matrix for for a field-of-view $\theta$, aspect ratio $\text{aspect}$, near clipping plane distance $n$, and far clipping plane distance $f$:

$$$
\[
P = \begin{pmatrix}
\frac{1}{\text{aspect} \, \tan\left(\frac{\theta}{2}\right)} & 0 & 0 & 0 \\
0 & \frac{1}{\tan\left(\frac{\theta}{2}\right)} & 0 & 0 \\
0 & 0 & -\frac{f+n}{f-n} & -\frac{2fn}{f-n} \\
0 & 0 & -1 & 0
\end{pmatrix}
\]
$$$

This maps your space to a coordinates in the range $[-1,1]$.

### 4. NDC (Normalized Device Coordinates) to Screen Coordinates
For a screen of width $W$ and height $H$:
* $\text{screenX} = (\frac{x_{NDC}+1}{2})\times{W}$
* $\text{screenY} = (1 - \frac{y_{NDC}+1}{2})\times{H}$


