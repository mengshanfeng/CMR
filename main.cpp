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

class TestAbstractDomain : public CMRAbstractDomain
{
	public:
		TestAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY) : CMRAbstractDomain(typeSize,width,height,ghostDepth,originX,originY){};
		virtual bool isContiguousGhost(const CMRRect2D & rect) const {};
		virtual size_t getGhostSize(const CMRRect2D & rect) const  {};
		virtual int copyGhostToBuffer(const CMRRect2D & rect) const {};
		virtual int copyGhostFromBuffer(const CMRRect2D & rect)  {};
		virtual void * getContiguousGhost(const CMRRect2D & rect)  {};
};

int main(int argc, char **argv)
{
	std::cout << "Hello, world!" << std::endl;

	/*CMRMesh2D<Cell> mesh(4096,16,1);
	mesh.apply(test0);
	mesh.apply(test1);
	mesh.apply(test2);
	mesh.apply(test3);
	mesh.apply(test4);*/

	std::cout << "Test on 800 x 600 (+1) sync +1" << std::endl;
	TestAbstractDomain domain(8,800,600,1,0,0);
	CMRCommSchem schem;
	std::cout << "CORNER TOP LEFT" << std::endl;
	domain.fillWithUpdateComm(schem,-1,-1,1,CMR_COMM_RECV);
	domain.fillWithUpdateComm(schem,-1,-1,1,CMR_COMM_SEND);
	std::cout << "LEFT" << std::endl;
	domain.fillWithUpdateComm(schem,-1,0,1,CMR_COMM_RECV);
	domain.fillWithUpdateComm(schem,-1,0,1,CMR_COMM_SEND);
	std::cout << "CORNER BOTTOM RIGHT" << std::endl;
	domain.fillWithUpdateComm(schem,1,1,1,CMR_COMM_RECV);
	domain.fillWithUpdateComm(schem,1,1,1,CMR_COMM_SEND);
	
	std::cout << "Test on 800 x 600 (+2) sync +1" << std::endl;
	TestAbstractDomain domain2(8,800,600,2,0,0);
	CMRCommSchem schem2;
	std::cout << "CORNER TOP LEFT" << std::endl;
	domain2.fillWithUpdateComm(schem2,-1,-1,1,CMR_COMM_RECV);
	domain2.fillWithUpdateComm(schem2,-1,-1,1,CMR_COMM_SEND);
	std::cout << "LEFT" << std::endl;
	domain2.fillWithUpdateComm(schem2,-1,0,1,CMR_COMM_RECV);
	domain2.fillWithUpdateComm(schem2,-1,0,1,CMR_COMM_SEND);
	std::cout << "CORNER BOTTOM RIGHT" << std::endl;
	domain2.fillWithUpdateComm(schem2,1,1,1,CMR_COMM_RECV);
	domain2.fillWithUpdateComm(schem2,1,1,1,CMR_COMM_SEND);
	
	std::cout << "Test on 800 x 600 (+2) sync +2" << std::endl;
	TestAbstractDomain domain3(8,800,600,2,0,0);
	CMRCommSchem schem3;
	std::cout << "CORNER TOP LEFT" << std::endl;
	domain3.fillWithUpdateComm(schem3,-1,-1,2,CMR_COMM_RECV);
	domain3.fillWithUpdateComm(schem3,-1,-1,2,CMR_COMM_SEND);
	std::cout << "LEFT" << std::endl;
	domain3.fillWithUpdateComm(schem3,-1,0,2,CMR_COMM_RECV);
	domain3.fillWithUpdateComm(schem3,-1,0,2,CMR_COMM_SEND);
	std::cout << "CORNER BOTTOM RIGHT" << std::endl;
	domain3.fillWithUpdateComm(schem3,1,1,2,CMR_COMM_RECV);
	domain3.fillWithUpdateComm(schem3,1,1,2,CMR_COMM_SEND);

	return 0;
}
