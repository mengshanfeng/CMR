Cartesian Mesh Runtime
======================

What it is
----------

CMR is a runtime plus a source to source compiler (DSL) to generate 2D stencil codes into C++ parallel codes (MPI, threads) 
and also some nice HTML report to descrive your scheme.

It currently support thread bases paralle code, MPI is a work in progress, get most of the code to get it but still need some work.
There is a work in progress to rewire the compiler in full JS instead of C++ to make the code smaller and more simple (see branch 
rewrite-compiler-in-js).

Pictures
--------

![Report](./src/screens/html-view.png?raw=true "An example of html report generated for LBM")

![LBM](./src/screens/lbm.png?raw=true "Example of rendering with gnuplot for LBM")

How to compiler
---------------

You can build and install with :

```sh
	mkdir build
	cd build
	../configure --prefix=$HOME/usr
	make
	make install
```

or using dirctly cmake :

```sh
	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/usr
	make
	make install
```

The idea behind CMR
-------------------

There is already plenty of DSL to do the same things :

But here I follow mostly some interesting ideas I wanted to try ;

 * Permit to explore different memory layouts. In CMR you never provide for loops, you need to consider parallel access with foreach semantic.
   The layout will be selected at compile time with some keyword.
 * Manual exploration of parameters. I don't wanted to get a fully automated approach to explore parameters at compile time but perfers
   to provide a way to split the "what" from "how" and letting the user explore the parameters by hand to understand what is good and not
   for his code.
 * Latex base. You provide you equations mostly in latex so you can copy past from/to your report. It also provide nice representaiton to
   ease checking of big equations.
 * Different parallism. The scheduling of datas and tasks is implemented into a separate class so we can easily implement different kind of
   parallism without taking care of the equations. (see runners).
 * Report generation. Thanks to the notation we used (DSL) we can generate a nice html report with all the steps and description of variables
   used by your scheme.
 * Long/short names. It is common for scientis to use variables `a`, `b`, `c`... It is fine for them but make the code impossible to be
   well understood by anybody else. In CMR we define variables with two names. A short (math) name in latex which is used in the latex equations
   and a long name which is used at compile time to generate the code.
 * Keep readable generated code. I don't want to get offuscated code as I want the user to be able to understand what I generate from the euqations
   to know how to optimize.
 * provide feedback. It is not yet implemented but I want to provide some feedback on steps to help the userd understanding which equations
   generate communications for example, so he can improve it.
 * I currently use an XML file to define the semantic but I expect to get something similar to `src/lib/tests/lbm_test.simple`.

How to run test case
--------------------

There is currenly only one test case which is LBM (Lattice Boltzmann Method) to simulate vorteces. You can find it into `examples/lbm`.
Use :

```sh
	cmr-generate lbm.cmr.xml
	make
	./lbm-release [options]
	./lbm-gen-animate-gif.sh output.raw output.gif
```

Config file
-----------

You can provide some config file to run the application and select parallism... You can get a dump of this file with :

```sh
	./lbm-release -d=config.ini
```

It looks like :

```ini
	[app]
	runner                         = CMRRunnerSeq

	[debug]
	mpi_domains                    = false

	[mesh]
	noout                          = false
	output_file                    = output.raw
	width                          = 800
	height                         = 100
	iterations                     = 1000
```

You can currently use runners `CMRRunnerSeq`, `CMRRunnerOMPForRect`, `CMRRunnerOMPLoops` or `CMRRunnerOMPTask`.

Licence & status
----------------

It is currently not finished so there is plenty of things to implement.

The licence is CeCILL-C.

