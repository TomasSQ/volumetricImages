#include <stdlib.h>

#include "ex03.h"

void drawSquare(Image2D image, Vertices vertices) {
	int i;

	for (i = 0; i < 3; i++) {
		drawLine(image, vertices[i], vertices[i + 1], 255);
	}

	drawLine(image, vertices[3], vertices[0], 255);
}

void render(char* name, Vector3D planeRotation, Cube cube) {
	int i;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = newImage2D(500, 500);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawSquare(image, cube->faces[i]->vertices);
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

void draw(Image2D image, Point3D start, float dx, float dy, int n, int intensity) {
	int i;
	Point3D p = createPoint3D(start->x, start->y, start->z);

	for (i = 0; i < n; i++) {
		if (0 <= p->y && p->y < image->height && 0 <= p->x && p->x < image->width) {
			image->img[(int) p->y][(int) p->x] = intensity;
		}

		p->x += dx;
		p->y += dy;
	}
}

void drawLine(Image2D image, Point3D start, Point3D end, int intensity) {
	int n;
	float deltaX, deltaY, dx, dy;

	if (samePoint(start, end)) {
		n = 0;
		dx = 0;
		dy = 0;
	} else {
		deltaX = end->x - start->x;
		deltaY = end->y - start->y;

		if (ABS(deltaX) >= ABS(deltaY)) {
			n = ABS(deltaX) + 1;
			dx = SIGN(deltaX);
			dy = dx * deltaY / deltaX;
		} else {
			n = ABS(deltaY) + 1;
			dy = SIGN(deltaY);
			dx = dy * deltaX / deltaY;
		}
	}

	draw(image, start, dx, dy, n, intensity);
}
