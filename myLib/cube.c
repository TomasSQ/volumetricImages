#include <stdlib.h>

#include "cube.h"

void recalculateNormals(Cube cube) {
	int face;

	for (face = 0; face < cube->nFaces; face++) {
		cube->faces[face]->normal = calculateNormal(cube->faces[face]->vertices, 4);
	}
}

void defineVertices(Cube cube, Point3D origin, float size) {
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
			vertexMap[vertex][0] * size + origin->x,
			vertexMap[vertex][1] * size + origin->y,
			vertexMap[vertex][2] * size + origin->z);
	}
}

void defineFaces(Cube cube) {
	int face, vertex;
	int faceMap[6][4] = {
		{0, 1, 2, 3},
		{4, 5, 6, 7},
		{0, 4, 7, 3},
		{1, 5, 6, 2},
		{0, 4, 5, 1},
		{3, 7, 6, 2},
	};


	cube->faces = (Face*) malloc(sizeof(Face) * cube->nFaces);
	for (face = 0; face < cube->nFaces; face++) {
		Vertices faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);

		for (vertex = 0; vertex < 4; vertex++) {
			faceVertices[vertex] = cube->vertices[faceMap[face][vertex]];
		}

		cube->faces[face] = createFace(faceVertices);
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

Cube createCube(Point3D origin, float size) {
	Cube cube = (Cube) malloc(sizeof(_cube));
	cube->nFaces = 6;
	cube->nEdges = 12;
	cube->nVertices = 8;

	defineVertices(cube, origin, size);
	defineFaces(cube);
	defineEdges(cube);

	return cube;
}

void destroCube(Cube cube) {
	free(cube->vertices);
	free(cube->edges);
	free(cube->faces);

	free(cube);
}
