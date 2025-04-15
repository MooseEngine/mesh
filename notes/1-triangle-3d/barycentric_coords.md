# Barycentric Coordinates
Barycentric coordinates are a way to express the position of a point P, which lies inside a triangle, as a weighted sum of the triangle's vertices $A$, $B$, and $C$.

Formally, *if*

$$$

P=\alpha{A} + \beta{B} + \gamma{C}

$$$

Then $\alpha$, $\beta$, and $\gamma$ (which are called the *barycentric weights/coordinates*) have the following properties:
* They are non negative if $P$ lies inside or on the edge of the triangle.
* They sum to 1 ($\alpha+\beta+\gamma=1$)

Each weight corresponds to the "influence" of its vertex on the point $P$.

## Computing Barycentric Coordinates
To compute these, one common method is to use the areas (determinants) of sub-triangles. Assuming:
* $A=(A_x, A_y)$
* $B=(B_x, B_y)$
* $C=(C_x, C_y)$

We want to find, for a point $P(P_x, P_y)$, the barycentric coordinates $\alpha$, $\beta$, and $\gamma$. They are:

$$
D = (y_B - y_C)(x_A - x_C) + (x_C - x_B)(y_A - y_C)
$$

$$
\alpha = \frac{(y_B - y_C)(x - x_C) + (x_C - x_B)(y - y_C)}{D}
$$

$$
\beta = \frac{(y_C - y_A)(x - x_C) + (x_A - x_C)(y - y_C)}{D}
$$

$$
\gamma = 1 - \alpha - \beta
$$

$$
P = \alpha A + \beta B + \gamma C
$$

Or, in code:
```c
float denom = ((B_y - C_y) * (A_x - C_x) + (C_x - B_x) * (A_y - C_y));

float alpha = ((B_y - C_y) * (P_x - C_x) + (C_x - B_x) * (P_y - C_y)) / denom;
float beta  = ((C_y - A_y) * (P_x - C_x) + (A_x - C_x) * (P_y - C_y)) / denom;
float gamma = 1.0f - alpha - beta;
```

## Interpolating Colors Using Barycentrics
One use case for this is interpolating colors. Interpolating just means... <!-- TODO: figure this out haha -->

Say you want each vertex of your triangle to be a different color, and you want the colors to blend as it goes from one vertex to the other. Let these colors be:
* $\text{Color}_A=(R_A, G_A, B_A)$
* $\text{Color}_B=(R_B, G_B, B_B)$
* $\text{Color}_C=(R_C, G_C, B_C)$

Then to have them blend, you can calculate each vertex's color channels:

```c
float R_P = alpha * R_A + beta * R_B + gamma * R_C;
float G_P = alpha * G_A + beta * G_B + gamma * G_C;
float B_P = alpha * B_A + beta * B_B + gamma * B_C;
```

Then, you can load it into your frame buffer like so:
```c
framebuffer[i] = (R_P << 16) | (G << 8) | B; // each color is 8 bytes (two for each of ARGB)
```
## Interpolating Colors in 3d (Persepective-Correct Interpolation)
The issue here is, of course, that we may have a shape in 3 dimensions. Simply applying the strategy above to the projection won't correctly respresent perspective. To do this, we'll need to do the following:

1. Store the reciprocal of the depth factor (i.e. $\frac{1}{w}).
2. Multiply the intended vertex attribute (in this case, color) by the stored reciprocal.
3. Interpolate in the screen space (**not** in projection space).
4. Recover the perspective-correct attribute (again, in this case it's color).




