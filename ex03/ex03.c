#include <stdlib.h>

#include "ex03.h"

#define EXTRA_STEP_FACTOR 2

Point3D reverseRotation(Point3D aux, Point3D origin, Vector3D rotation) {
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

void drawLineOfSquare(Image2D image, Point3D start, Inc inc, Image* img, Vector3D rotation, Point3D origin, Point3D dp) {
	int i, x, y, voxel;
	Point3D p = createPoint3D(start->x, start->y, start->z);
	Point3D aux;

	for (i = 0; i < inc.n * EXTRA_STEP_FACTOR; i++) {
		if (0 <= p->y && p->y < image->height && 0 <= p->x && p->x < image->width) {
			if (img != NULL) {
				voxel = (((int) p->z) * img->xsize * img->ysize + ((int) p->y) * img->xsize + ((int) p->x));
				if (0 > p->z || p->z > img->zsize || 0 > p->y || p->y > img->ysize || 0 > p->x || p->x > img->xsize) {
					voxel = img->n + 1;
				}
			} else {
				voxel = -1;
			}
			if (dp == NULL) {
				x = p->x;
				y = p->y;
			} else {
				aux = copy(createVector3D(0, 0, 0), p);
				aux = reverseRotation(aux, dp, rotation);
				aux = translate(aux, dp, false);
				// aux = translate(aux, dp, true);
				x = aux->x + image->width / 2 * 0;
				y = aux->y + image->height / 2 * 0;
				free(aux);
			}
			if (0 < x && x < image->width && 0 < y && y < image->height) {
				image->img[y][x] = voxel > 0 && voxel < img->n ? img->val[voxel] : 0;
			}
		}

		p->x += inc.x / EXTRA_STEP_FACTOR;
		p->y += inc.y / EXTRA_STEP_FACTOR;
		p->z += inc.z / EXTRA_STEP_FACTOR;
	}

	free(p);
}

void drawSquare(Image2D image, Vertices vertices, Image* img, Vector3D reverseRotation, Point3D origin, Point3D dp) {
	Vertex start, end;
	Inc inc;
	int i;

	inc = getInc(vertices[0], vertices[3], false);
	start = copy(createPoint3D(0, 0, 0), vertices[0]);
	end = copy(createPoint3D(0, 0, 0), vertices[1]);

	for (i = 0; i < inc.n * EXTRA_STEP_FACTOR; i++) {
		drawLineOfSquare(image, start, getInc(start, end, false), img, reverseRotation, origin, dp);
		start->x += inc.x / EXTRA_STEP_FACTOR;
		start->y += inc.y / EXTRA_STEP_FACTOR;
		start->z += inc.z / EXTRA_STEP_FACTOR;
		end->x += inc.x / EXTRA_STEP_FACTOR;
		end->y += inc.y / EXTRA_STEP_FACTOR;
		end->z += inc.z / EXTRA_STEP_FACTOR;
	}

	free(start);
	free(end);
}

Image2D render(Vector3D planeRotation, Cube cube, Image* img) {
	int i, f;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = img == NULL ? newImage2D(200, 200) : newImage2D(img->xsize, img->ysize);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawSquare(image, cube->faces[i]->vertices, img, NULL, NULL, NULL);
			for (f = 0; f < 3; f++) {
				drawLine(image, cube->faces[i]->vertices[f], cube->faces[i]->vertices[f + 1], 255, false);
			}
			drawLine(image, cube->faces[i]->vertices[3], cube->faces[i]->vertices[0], 255, false);
		}
	}

	free(visible);

	return image;
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

		p->x += inc.x;
		p->y += inc.y;
		p->z += inc.z;
	}
}

void drawLine2D(Image2D image, Point3D start, Point3D end, int intensity) {
	drawLine(image, start, end, intensity, true);
}

void drawLine(Image2D image, Point3D start, Point3D end, int intensity, bool ignoreZ) {
	draw(image, start, getInc(start, end, ignoreZ), intensity);
}
