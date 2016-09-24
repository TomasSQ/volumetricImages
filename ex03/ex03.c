#include <stdlib.h>

#include "ex03.h"

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

void drawSquare(Image2D image, Vertices vertices, int intensity) {
	Vertex start, end;
	Inc inc;
	int i;
	int extraStepsFactor = 10;

	inc = getInc(vertices[0], vertices[3], false);
	start = copy(createPoint3D(0, 0, 0), vertices[0]);
	end = copy(createPoint3D(0, 0, 0), vertices[1]);

	for (i = 0; i < inc.n * extraStepsFactor; i++) {
		drawLine2D(image, start, end, intensity);
		start->x += inc.inc->x / extraStepsFactor;
		start->y += inc.inc->y / extraStepsFactor;
		start->z += inc.inc->z / extraStepsFactor;
		end->x += inc.inc->x / extraStepsFactor;
		end->y += inc.inc->y / extraStepsFactor;
		end->z += inc.inc->z / extraStepsFactor;
	}
}

void render(char* name, Vector3D planeRotation, Cube cube) {
	int i;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = newImage2D(500, 500);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawSquare(image, cube->faces[i]->vertices, 255 / (i + 1));
		}
	}

	saveImage(name, image->img, image->width, image->height);
	freeImage2D(image);
	free(visible);
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
