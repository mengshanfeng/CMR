#include <iostream>
#include "CMRMesh.h"

struct Cell
{
	int a;
	int b;
	int c;
};

void test0(Cell & cell,CMRMesh2DAccessor<Cell> & accessor)
{
	cell.a = 0;
	cell.c = 1.0;
	cell.b = 2.0;
}

void test1(Cell & cell,CMRMesh2DAccessor<Cell> & accessor)
{
	cell.a = cell.c + 2 * cell.b;
}

void test2(Cell & cell,CMRMesh2DAccessor<Cell> & accessor)
{
	cell.a = cell.a + 2;
}

void test3(Cell & cell,CMRMesh2DAccessor<Cell> & accessor)
{
	Cell & left = accessor.getLeft();

	left.a = 0;
	left.b = 1.0;
	left.c = 2.0;
}

int main(int argc, char **argv)
{
	std::cout << "Hello, world!" << std::endl;

	CMRMesh2D<Cell> mesh(4096,16,1);
	mesh.apply(test0);
	mesh.apply(test1);
	mesh.apply(test2);
	mesh.apply(test3);

	return 0;
}
