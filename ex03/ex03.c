#include <stdlib.h>

#include "ex03.h"

#define EXTRA_STEP_FACTOR 2

typedef struct {
	Point3D inc;
	int n;
} Inc;

Inc getInc(Point3D start, Point3D end, bool ignoreZ) {
	float deltaX, deltaY, deltaZ;

	Inc inc;
	inc.n = 0;
	inc.inc = createPoint3D(0, 0, 0);

	if (samePoint(start, end)) {
		return inc;
	}

	deltaX = end->x - start->x;
	deltaY = end->y - start->y;
	deltaZ = end->z - start->z;

	if (ABS(deltaX) >= ABS(deltaY) && (ignoreZ || ABS(deltaX) >= ABS(deltaZ))) {
		inc.n = ABS(deltaX) + 1;
		inc.inc->x = SIGN(deltaX);
		inc.inc->y = inc.inc->x * deltaY / deltaX;
		inc.inc->z = inc.inc->x * deltaZ / deltaX;
	} else if (ABS(deltaY) >= ABS(deltaX) && (ignoreZ || ABS(deltaY) >= ABS(deltaZ))) {
		inc.n = ABS(deltaY) + 1;
		inc.inc->y = SIGN(deltaY);
		inc.inc->x = inc.inc->y * deltaX / deltaY;
		inc.inc->z = inc.inc->y * deltaZ / deltaY;
	} else if (!ignoreZ) {
		inc.n = ABS(deltaZ) + 1;
		inc.inc->z = SIGN(deltaZ);
		inc.inc->x = inc.inc->z * deltaX / deltaZ;
		inc.inc->y = inc.inc->z * deltaY / deltaZ;
	}

	return inc;
}

Point3D reverseRotation(Point3D p, Point3D origin, Vector3D rotation) {
	Point3D aux = copy(createPoint3D(0, 0, 0), p);

	if (rotation->y != 0) {
		aux = rotateY(aux, origin, rotation->y, true);
	}
	if (rotation->x != 0) {
		aux = rotateX(aux, origin, rotation->x, true);
	}
	if (rotation->z != 0) {
		aux = rotateZ(aux, origin, rotation->z, true);
	}

	return aux;
}

void drawLineOfSquare(Image2D image, Point3D start, Inc inc, Image* img, Vector3D rotation, Point3D origin) {
	int i, x, y, voxel;
	Point3D p = createPoint3D(start->x, start->y, start->z);
	Point3D aux;

	for (i = 0; i < inc.n * EXTRA_STEP_FACTOR; i++) {
		if (0 <= p->y && p->y < image->height && 0 <= p->x && p->x < image->width) {
			voxel = (((int) p->z) * img->xsize * img->ysize + ((int) p->y) * img->xsize + ((int) p->x));
			if (0 > p->z || p->z > img->zsize || 0 > p->y || p->y > img->ysize || 0 > p->x || p->x > img->xsize) {
				voxel = img->n + 1;
			}
			if (rotation == NULL) {
				x = p->x;
				y = p->y;
			} else {
				aux = reverseRotation(p, origin, rotation);
				x = aux->x;
				y = aux->y;
				free(aux);
			}
			if (0 < x && x < image->width && 0 < y && y < image->height) {
				image->img[y][x] = voxel > 0 && voxel < img->n ? img->val[voxel] : 0;
			}
		}

		p->x += inc.inc->x / EXTRA_STEP_FACTOR;
		p->y += inc.inc->y / EXTRA_STEP_FACTOR;
		p->z += inc.inc->z / EXTRA_STEP_FACTOR;
	}

	free(p);
}

void drawSquare(Image2D image, Vertices vertices, Image* img, Vector3D reverseRotation, Point3D origin) {
	Vertex start, end;
	Inc inc;
	int i;

	inc = getInc(vertices[0], vertices[3], false);
	start = copy(createPoint3D(0, 0, 0), vertices[0]);
	end = copy(createPoint3D(0, 0, 0), vertices[1]);

	for (i = 0; i < inc.n * EXTRA_STEP_FACTOR; i++) {
		drawLineOfSquare(image, start, getInc(start, end, false), img, reverseRotation, origin);
		start->x += inc.inc->x / EXTRA_STEP_FACTOR;
		start->y += inc.inc->y / EXTRA_STEP_FACTOR;
		start->z += inc.inc->z / EXTRA_STEP_FACTOR;
		end->x += inc.inc->x / EXTRA_STEP_FACTOR;
		end->y += inc.inc->y / EXTRA_STEP_FACTOR;
		end->z += inc.inc->z / EXTRA_STEP_FACTOR;
	}

	free(start);
	free(end);
}

Image2D render(Vector3D planeRotation, Cube cube, Image* img) {
	int i, f;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = newImage2D(img->xsize, img->ysize);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawSquare(image, cube->faces[i]->vertices, img, NULL, NULL);
			for (f = 0; f < 3; f++) {
				drawLine(image, cube->faces[i]->vertices[f], cube->faces[i]->vertices[f + 1], 255, false);
			}
			drawLine(image, cube->faces[i]->vertices[3], cube->faces[i]->vertices[0], 255, false);
		}
	}

	free(visible);

	return image;
}

Image2D getSlice(Point3D origin, Vector3D normal, Image* image) {
	int imageSize = MAX(MAX(image->xsize, image->ysize), image->zsize);
	Image2D slice = newImage2D(imageSize, imageSize);
	Vector3D cubeScale = createVector3D(image->xsize, image->ysize, 10);
	Vector3D rotation = NULL;

	Cube cube = createCube(origin, cubeScale);
	rotation = alignCube(cube, normal);

	drawSquare(slice, cube->faces[0]->vertices, image, rotation, origin);

	destroyCube(cube);

	return slice;
}

void visibleFaces(Vector3D planeRotation, Face* faces, bool* visibleFaces, int nFaces) {
	int i = 0;

	Vector3D normal = createVector3D(0, 0, -1);

	for (i = 0; i < nFaces; i++) {
		visibleFaces[i] = innerProduct(normal, faces[i]->normal) > 0;
	}
}

void draw(Image2D image, Point3D start, Inc inc, int intensity) {
	int i;
	Point3D p = createPoint3D(start->x, start->y, start->z);

	for (i = 0; i < inc.n; i++) {
		if (0 <= p->y && p->y < image->height && 0 <= p->x && p->x < image->width) {
			image->img[(int) p->y][(int) p->x] = intensity;
		}

		p->x += inc.inc->x;
		p->y += inc.inc->y;
		p->z += inc.inc->z;
	}
}

void drawLine2D(Image2D image, Point3D start, Point3D end, int intensity) {
	drawLine(image, start, end, intensity, true);
}

void drawLine(Image2D image, Point3D start, Point3D end, int intensity, bool ignoreZ) {
	draw(image, start, getInc(start, end, ignoreZ), intensity);
}
