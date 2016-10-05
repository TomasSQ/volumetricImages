#include <stdlib.h>

#include "cube.h"

Cube translateCube(Cube cube, Point3D p) {
	int i;

	translate(cube->origin, p, false);
	for (i = 0; i < cube->nVertices; i++) {
		translate(cube->vertices[i], p, false);
	}

	return cube;
}

Cube scaleCube(Cube cube, Vector3D scaleFactor) {
	int i;

	for (i = 0; i < cube->nVertices; i++) {
		translate(scale(translate(cube->vertices[i], cube->origin, true), scaleFactor, false), cube->origin, false);
	}
	scale(cube->size, scaleFactor, false);

	return cube;
}

Cube rotateCube(Cube cube, Vector3D rotation) {
	int i;

	for (i = 0; i < cube->nVertices; i++) {
		rotateX(cube->vertices[i], cube->origin, rotation->x, false);
		rotateY(cube->vertices[i], cube->origin, rotation->y, false);
		rotateZ(cube->vertices[i], cube->origin, rotation->z, false);
	}

	updateCube(cube);

	return cube;
}

Vector3D alignCube(Cube cube, Vector3D normal) {
	int i;
	float rotationX, rotationY, rotationZ;

	Vector3D aux1 = copy(createVector3D(0, 0, 0), normal);
	Vector3D aux2 = copy(createVector3D(0, 0, 0), cube->faces[0]->normal);
	aux1->x = 0;
	aux2->x = 0;
	rotationX = angleBetweenVectors(aux1, aux2);

	aux1 = copy(aux1, normal);
	aux2 = copy(aux2, cube->faces[0]->normal);
	aux1->y = 0;
	aux2->y = 0;
	rotationY = angleBetweenVectors(aux1, aux2);

	aux1 = copy(aux1, normal);
	aux2 = copy(aux2, cube->faces[0]->normal);
	aux1->z = 0;
	aux2->z = 0;
	rotationZ = angleBetweenVectors(aux1, aux2);

	for (i = 0; i < cube->nVertices; i++) {
		if (rotationX) {
			rotateX(cube->vertices[i], cube->origin, rotationX, false);
		}
		if (rotationY) {
			rotateY(cube->vertices[i], cube->origin, rotationY, false);
		}
		if (rotationZ) {
			rotateZ(cube->vertices[i], cube->origin, rotationZ, false);
		}
	}

	updateCube(cube);

	free(aux1);
	free(aux2);

	return createVector3D(rotationX, rotationY, rotationZ);
}

void defineVertices(Cube cube) {
	int vertex;
	int vertexMap[8][3] = {
		{-1,  1,  1},
		{ 1,  1,  1},
		{ 1, -1,  1},
		{-1, -1,  1},
		{-1,  1, -1},
		{ 1,  1, -1},
		{ 1, -1, -1},
		{-1, -1, -1},
	};

	cube->vertices = (Vertices) malloc(sizeof(Vertex) * cube->nVertices);
	for (vertex = 0; vertex < cube->nVertices; vertex++) {
		cube->vertices[vertex] = createVertex(
			vertexMap[vertex][0],
			vertexMap[vertex][1],
			vertexMap[vertex][2]);
		cube->vertices[vertex] = translate(scale(cube->vertices[vertex], cube->size, false), cube->origin, false);
	}
}

void defineFaces(Cube cube) {
	int face, vertex;
	int faceMap[6][5] = {
		{0, 1, 2, 3, 0},
		{4, 5, 6, 7, 1},
		{0, 4, 7, 3, 1},
		{1, 5, 6, 2, 0},
		{0, 4, 5, 1, 0},
		{3, 7, 6, 2, 1},
	};


	cube->faces = (Face*) malloc(sizeof(Face) * cube->nFaces);
	for (face = 0; face < cube->nFaces; face++) {
		Vertices faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);

		for (vertex = 0; vertex < 4; vertex++) {
			faceVertices[vertex] = cube->vertices[faceMap[face][vertex]];
		}

		cube->faces[face] = createFace(faceVertices, faceMap[face][4]);
	}
}

void defineEdges(Cube cube) {
	int edge;
	int edgeMap[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},
		{4, 5}, {5, 6}, {6, 7}, {7, 8},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};

	cube->edges = (Edge*) malloc(sizeof(Edge) * cube->nEdges);
	for (edge = 0; edge < cube->nEdges; edge++) {
		cube->edges[edge] = createEdge(cube->vertices[edgeMap[edge][0]], cube->vertices[edgeMap[edge][1]]);
	}
}

Cube createCube(Point3D origin, Vector3D size) {
	Cube cube = (Cube) malloc(sizeof(_cube));
	cube->origin = createPoint3D(origin->x, origin->y, origin->z);
	cube->size = createVector3D(size->x, size->y, size->z);
	cube->nFaces = 6;
	cube->nEdges = 12;
	cube->nVertices = 8;

	defineVertices(cube);
	defineFaces(cube);
	defineEdges(cube);

	return cube;
}

void updateCube(Cube cube) {
	int face;

	for (face = 0; face < cube->nFaces; face++) {
		cube->faces[face]->normal = calculateFaceNormal(cube->faces[face]);
	}
}

void destroyCube(Cube cube) {
	free(cube->vertices);
	free(cube->edges);
	free(cube->faces);
	free(cube->size);
	free(cube->origin);

	free(cube);
}
