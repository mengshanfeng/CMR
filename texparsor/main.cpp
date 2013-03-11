#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include "CMRLatexEntity.h"
#include "CMRParsorBasics.h"
#include "CMRTexParsor.h"
#include "CMREqDepMatrix.h"
#include "CMREntityMatching.h"

using namespace std;

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	//string value = "v'_{l} = \\frac{\\sum_{k}{v_{i,j,k} * D_{l,k}}}{d}";
	//string value = "d = \\sum_{l=1}^{9}{D_{i,j,k}}";
	//string value = "f_{ eq } = (1.0 + (3 * p) + ((9/2) * p^2 - (3/2) * v^2)) * W_{k} * d";
// 	string value = "D'_{i,j} = D_{i+1,j+2}+\\sum_{l=1}^{9}{D_{i,j,k}}";
	assert(argc == 3);
	string dep= argv[1];
	string value = argv[2];
	CMRLatexFormulas f;
	/*CMRLatexEntity * entity = cmrParseLatexEntity(value,start);
	if (entity != NULL)
		cmrPrintLatexEntity(*entity,0,0);*/
	cmrParseLatexFormula(f,value);
	cmrPrintFormula(f,0);
	
	printf("================================================\n");

	CMRLatexEntity entity;
	entity.name = "f";
	entity.subscriptTotalValue = "eq";
	entity.print(0,0);
	printf(" => matching : %d\n",cmrEntityExactMatch(*f[0],entity));
	
	printf("================================================\n");
	
	CMREqDepMatrix depMatrix;
	//CMREntityCellMatching match = cmrEntityCellMatch(*f[0],"D");
	//match.printDebug();
	//depMatrix.markAccess(match.dx,match.dy,match.write);
	cmrExtractDeps(depMatrix,f,dep);
	printf("=============== DEP MATRIX ON %s ===============\n",dep.c_str());
	depMatrix.printDebug();
	printf("================================================\n");

	return 0;
}
