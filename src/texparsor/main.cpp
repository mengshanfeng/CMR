/********************  HEADERS  *********************/
#include <cstdlib>
#include <iostream>
#include "project/CMRProject.h"
#include "project/CMRProjectXMLLoader.h"
#include "transformations/CMRTransformationExtractLoops.h"
#include "transformations/CMRTransformationImplicitMul.h"
#include "transformations/CMRTransformationExpandFrac.h"
#include "transformations/CMRTransformationExpandExpo.h"
#include "transformations/CMRTransformationMarkNoTransf.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
void test1(void)
{
	cout << "===================================================" << endl;
	CMRProject2 project;
	CMRProjectConstant & direction_matrix = project.addConstant("M","direction_matrix");
	direction_matrix.loadValues("0.0 & 0.0 \\\\ 1.0 & 0.0 \\\\ 0.0 & 1.0 \\\\ -1.0 & 0.0 \\\\ 0.0 & -1.0 \\\\ 1.0 & 1.0 \\\\ -1.0 & 1.0 \\\\ -1.0 & -1.0 \\\\ 1.0 & -1.0",2);
	//cst2.addIndice("k",CMR_CAPTURE_REQUIRED);
		
	CMRProjectConstant & equi_weight = project.addConstant("W","equi_weight");
	equi_weight.loadValues("4.0/9.0 & 1.0/9.0 & 1.0/9.0 & 1.0/9.0 & 1.0/9.0 & 1.0/36.0 & 1.0/36.0 & 1.0/36.0 & 1.0/36.0",1);
		
	CMRProjectConstant & opposite_of = project.addConstant("o","opposite_of");
	opposite_of.loadValues("0 & 3 & 4 & 1 & 2 & 7 & 8 & 5 & 6",1);
		
	CMRProjectConstant & reynolds = project.addConstant("R","reynolds");
	reynolds.loadValues("300",0);
	///////
		
	CMRProjectMeshVar & var2 = project.addvariable("f_{i,j,k}","pdf","double");
	var2.addDim("k",9);
		
	project.addvariable("T_{i,j}","cell_type","LBMCellType");

	project.addvariable("O_{i,j}","fileout","LBMFileEntry");
	///////
		
	project.addIterator("k","k",0,9);
	project.addIterator("l","l",0,9);
	project.addIterator("s","s",0,1);
		
	///////
	project.addDefinition("d","density","\\sum_k{f_{i,j,k}}");
		
	CMRProjectDefinition & vs = project.addDefinition("v_{i,j}","velocity","\\sum_k{f_{i,j,k} * M_{k,s}} / d");
	vs.addIndice("s",CMR_CAPTURE_REQUIRED);
		
	CMRProjectDefinition & feq = project.addDefinition("f_{eq,i,j,k}","eq_profile");
	feq.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	feq.addLocalVariable("v^2","v2","double","0");
	feq.addLocalVariable("p","p","double","0");
	//feq.addLocalVariable("f_{eq,i,j,k}","eq_profile","double","0");
	feq.addEquation("v^2","\\sum_s{v_{i,j,s} * v_{i,j,s}}");
	feq.addEquation("p","\\sum_s{M_{k,s} * v_{i,j,s}}");
	feq.addEquation("f_{eq,i,j,k}","1 + 3p + \\frac{9}{2}p^2 - \\frac{3}{2}v^2");
	feq.addEquation("f_{eq,i,j,k}","f_{eq,i,j,k} * W_k * d");
		
// 	project.addDefinition("e_{i,j}","energy","\\sum_k{D_{i,j,k}*D_{i,j,k}+5}");

	//CMRProjectAction & ac = project.addAction("UpdateEnergy","update the energy");
	//ac.addEquation("b","bip","4 * 2 + A_{eq,4}");
	//ac.addEquation("D'_{i,j,3+5}","density","e^3_{i,j} / 2A_{eq,1} A_{eq,2} + ( 6 + 7 )^2 \\frac{1}{2+4} + 4+\\sum_k{D_{i,j,k}*A_{eq,k}}+\\sum_k{D_{i+1,j-1,k}*A_{eq,k}}+\\sum_k{\\sum_l{D_{i,j,k}*l}} + b");
	
	project.printDebug();
	CMRTransformationExtractLoops extractLoops;
	project.runTransformation(extractLoops);
	//	project.replaceLoops();
	project.printDebug();
	CMRTransformationMarkNoTransf noTransf;
	project.runTransformation(noTransf);
	project.printDebug();
	CMRTransformationImplicitMul implicitMul;
	project.runTransformation(implicitMul);
	//	project.insertImplicitMul();
	project.printDebug();
	CMRTransformationExpandFrac expandFrac;
	project.runTransformation(expandFrac);
	//CMRProjectTransfExpandFrac t1;
	//project.runTransformation(t1);
	project.printDebug();
	CMRTransformationExpandExpo expandExpo;
	project.runTransformation(expandExpo);
	//CMRProjectTransfExpendExponent t2;
	//project.runTransformation(t2);
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
		
	cout << "================================================" << endl;
	project.genCCode(cout);
	cout << "================================================" << endl;
}

/*******************  FUNCTION  *********************/
void test2(std::string fname)
{
	CMRProject2 project;
	CMRProjectXMLLoader loader;
	loader.load(project,fname);
	
	project.printDebug();
	CMRTransformationExtractLoops extractLoops;
	project.runTransformation(extractLoops);
	project.printDebug();
	CMRTransformationMarkNoTransf noTransf;
	project.runTransformation(noTransf);
	project.printDebug();
	CMRTransformationImplicitMul implicitMul;
	project.runTransformation(implicitMul);
	project.printDebug();
	CMRTransformationExpandFrac expandFrac;
	project.runTransformation(expandFrac);
	project.printDebug();
	CMRTransformationExpandExpo expandExpo;
	project.runTransformation(expandExpo);
	project.printDebug();
	
	cout << "================================================" << endl;
	project.genCCode(cout);
	cout << "================================================" << endl;
}

/*******************  FUNCTION  *********************/
int main(int argc, char ** argv)
{
	if (argc == 2)
		test2(argv[1]);
	else
		test1();

	return EXIT_SUCCESS;
}
