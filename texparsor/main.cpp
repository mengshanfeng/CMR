#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include "parsor/CMRLatexEntity.h"
#include "parsor/CMRParsorBasics.h"
#include "parsor/CMRTexParsor.h"
#include "parsor/CMREqDepMatrix.h"
#include "parsor/CMREntityMatching.h"
#include "CMRProjectEntity.h"
#include "CMRProjectConstant.h"
#include "CMRProjectEquation.h"
#include "CMRProjectIterator.h"
#include "CMRProjectVariable.h"
#include "CMRProjectAction.h"
#include "CMRProject.h"
#include "CMRCodeTree.h"
#include "CMRProjectTransformation.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectEquation * convertLoopToEq(CMRLatexEntity * entity)
{
	//nothing to do
	if (entity == NULL)
		return NULL;
	//extract the loop id
	string id = entity->subscriptTotalValue;
	cout << "===> loop id = " << id << endl;
	//extract inner term
	return new CMRProjectEquation("Z_1","CMR_inner_loop_1",entity->params[0]->string);
}

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
		///////////////////////////////////////////////////////////////////////////////////////////////////
// 		CMREntityConstant cst("A_{eq,i}^{2*4}","toto");
// 		cst.addIndice("k",CMR_CAPTURE_REQUIRED);
// 		//cst.loadValues("1.1",0);
// 		//cst.loadValues("1.1 ; 2.2 ; 3.3",1);
// 		cst.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
// 		cst.printDebug();
// 		cst.printCPPCode();
// 		
// 		CMRLatexFormulas f;
// 		cmrParseLatexFormula(f,"A^2_{eq,i+1,4,j,k}");
// 		
// 		CMRIndiceCaptureMap capture;
// 		printf("Matching : %d\n",cst.match(*f.childs[0],capture));
// 		for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
// 		{
// 			printf("Capture %s => %s\n",it->first.c_str(),it->second.c_str());
// 		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
// 		CMRProjectVariable var("D_{i,j,k}","Directions","int");
// 		var.addDim(9,"k",1);
// 		var.printDebug();
// 		var.printCPPCode();

		///////////////////////////////////////////////////////////////////////////////////////////////////
// 		CMRProjectIterator iter("k","k",1,9);
// 		iter.printDebug();
// 		iter.printCPPCode();
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
// 		CMRProjectEquation eq("d_{i,j}","density","\\sum_k{D_{i,j,k}}");
// 		eq.printDebug();
// 		CMRLatexEntity * term = eq.extractNextInnerLoop();
// 		if (term == NULL)
// 		{
// 			cout << "None" << endl;
// 		} else {
// 			cout << "Capture inner loop : " << term->totalValue << endl;
// 		}
// 		convertLoopToEq(term)->printDebug();
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//assemble into project
		cout << "===================================================" << endl;
		CMRProject project;
		CMREntityConstant & cst2 = project.addConstant("A_{eq}","toto");
// 		cst2.addIndice("k",CMR_CAPTURE_REQUIRED);
		//cst2.loadValues("1.1",0);
		cst2.loadValues("1.1 ; 2.2 ; 3.3 ; 4.4 ; 5.5 ; 6.6 ; 7.7 ; 8.8 ; 9.9",1);
		//cst2.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
		
		CMRProjectVariable & var2 = project.addvariable("D_{i,j,k}","directions","int");
		var2.addDim(9,"k");
		
		project.addIterator("k","k",0,9);
		project.addIterator("l","l",0,9);
		
		project.addDefinition("e_{i,j}","energy","\\sum_k{D_{i,j,k}*D_{i,j,k}+5}");

		CMRProjectAction & ac = project.addAction("UpdateEnergy","update the energy");
		ac.addEquation("b","bip","4 * 2 + A_{eq,4}");
		ac.addEquation("D'_{i,j,3+5}","density","e_{i,j} / 2A_{eq,1} A_{eq,2} + ( 6 + 7 ) \\frac{1}{2+4} + 4+\\sum_k{D_{i,j,k}*A_{eq,k}}+\\sum_k{D_{i+1,j-1,k}*A_{eq,k}}+\\sum_k{\\sum_l{D_{i,j,k}*l}} + b");
		project.printDebug();
		project.replaceLoops();
		project.printDebug();
		project.insertImplicitMul();
		project.printDebug();
		CMRProjectTransfExpandFrac t1;
		project.runTransformation(t1);
		project.printDebug();
		
		/**
		 *  TODO :
		 *    - Redefinir les Context attaché aux actions
		 *    - Iterateur locaux
		 *    - Remplacement des exposants
		 *    - Propagation de type
		 *    - Notation vectoriel
		 *    - Definir un 2eme type d'alias grace auquel on peut figer certains des parametres attendues
		 *    - Support - 1 en début d'équation (pb à cause de - pour le remplacement des * implicite)
		 * 
		 *  TODO clean :
		 *    - Séparé ProjectAction en deux classes
		 **/
		
		printf("================================================\n");
		project.genCCode(cout);
		printf("================================================\n");
	}

	return 0;
}
