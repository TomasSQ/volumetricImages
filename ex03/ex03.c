#include <stdlib.h>

#include "ex03.h"

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
