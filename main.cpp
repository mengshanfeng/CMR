#include <iostream>
#include "CMRMesh.h"
#include "lib/CMRAbstractDomain.h"

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

void test4(Cell & cell_out,CMRMesh2DAccessor<Cell> & accessor_out,const Cell & cell_in,const CMRMesh2DAccessor<Cell> & accessor_in)
{
	const Cell & left = accessor_in.getLeft();

	cell_out.a = cell_in.a + left.a;
/*	cell_out.c = cell_in.b;// + left.b;
	cell_out.b = cell_in.c;// + left.c;*/
}

int main(int argc, char **argv)
{
	std::cout << "Hello, world!" << std::endl;

	CMRMesh2D<Cell> mesh(4096,16,1);
	mesh.apply(test0);
	mesh.apply(test1);
	mesh.apply(test2);
	mesh.apply(test3);
	mesh.apply(test4);

	return 0;
}
