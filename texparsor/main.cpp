#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include "CMRLatexEntity.h"
#include "CMRParsorBasics.h"
#include "CMRTexParsor.h"
#include "CMREqDepMatrix.h"
#include "CMREntityMatching.h"
#include "CMRProjectEntity.h"
#include "CMRProjectConstant.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*********************  CLASS  **********************/
class CMRProject
{
	
};

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	//string value = "v'_{l} = \\frac{\\sum_{k}{v_{i,j,k} * D_{l,k}}}{d}";
	//string value = "d = \\sum_{l=1}^{9}{D_{i,j,k}}";
	//string value = "f_{ eq } = (1.0 + (3 * p) + ((9/2) * p^2 - (3/2) * v^2)) * W_{k} * d";
// 	string value = "D'_{i,j} = D_{i+1,j+2}+\\sum_{l=1}^{9}{D_{i,j,k}}";
	if (argc == 3)
	{
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
		printf(" => matching : %d\n",cmrEntityExactMatch(*f.childs[0],entity));
		
		printf("================================================\n");
		
		CMREqDepMatrix depMatrix;
		//CMREntityCellMatching match = cmrEntityCellMatch(*f[0],"D");
		//match.printDebug();
		//depMatrix.markAccess(match.dx,match.dy,match.write);
		cmrExtractDeps(depMatrix,f,dep);
		printf("=============== DEP MATRIX ON %s ===============\n",dep.c_str());
		depMatrix.printDebug();
		printf("================================================\n");
	} else {
		CMREntityConstant cst("A_{eq,i}^{2*4}","toto");
		cst.addIndice("k",CMR_CAPTURE_REQUIRED);
		//cst.loadValues("1.1",0);
		//cst.loadValues("1.1 ; 2.2 ; 3.3",1);
		cst.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
		cst.printDebug();
		cst.printCPPCode();
		
		CMRLatexFormulas f;
		cmrParseLatexFormula(f,"A_{eq,i+1,4,j,k}");
		
		CMRIndiceCaptureMap capture;
		printf("Matching : %d\n",cst.match(*f.childs[0],capture));
		for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
		{
			printf("Capture %s => %s\n",it->first.c_str(),it->second.c_str());
		}
	}

	return 0;
}
