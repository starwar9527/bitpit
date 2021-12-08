/*---------------------------------------------------------------------------*\
 *
 *  bitpit
 *
 *  Copyright (C) 2015-2021 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of bitpit.
 *
 *  bitpit is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  bitpit is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with bitpit. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/

#if BITPIT_ENABLE_MPI==1
#include <mpi.h>
#endif

#include "bitpit_PABLO.hpp"

using namespace std;
using namespace bitpit;

// =================================================================================== //
/*!
	\example PABLO_example_00001.cpp

	\brief 2D adaptive mesh refinement (AMR) using PABLO

	This example creates a 2D Octree mesh on the square domain [0,1]x[0,1].

	The domain is refined globally one time,
	then refined iteratively using two different refinement criteria.
	At the end of the iterative refinement, one global refinement is performed again.

	In the first criterion, each octant generated by a previous refinement iteration is
	marked for further refinement if its center is within a circle with a specified
	radius. Iterative refinement stops when a fixed number of iteration is reached.

	In the second criterion, the refinement is performed using the same rules of
	the first criterion. However, this refinement is performed until the flag
	returned by the adapt method is true (for further details about adapt method,
	please visit documentation). Video below illustrates the iterative refinement.
	Each frame show the result of a refinement iteration starting from the ancestor
	quadrant up to the last global refinement.

	The upper part of the domain is adapted by a refinement procedure using the first criterion,
	 while the bottom part by a refinement procedure using the second criterion.

	 Moreover, in the right hand side of the domain, the 2:1 balancing is deactivated.

	<b>To run</b>: ./PABLO_example_00001 \n

	<b>To see the result visit</b>: <a href="http://optimad.github.io/PABLO/">PABLO website</a> \n

*/
// =================================================================================== //

/**
 * Run the example.
 */
void run()
{
	/**<Instantation of a 2D pablo uniform object.*/
	PabloUniform pablo1(2);

	/**<Compute the connectivity and write the octree.*/
	pablo1.computeConnectivity();
	pablo1.write("pablo00001_iter0");

	/**<Refine globally one level and write the octree.*/
	pablo1.adaptGlobalRefine();
	pablo1.updateConnectivity();
	pablo1.write("pablo00001_iter1");

	/**<Define a center point.*/
	double xc, yc;
	xc = yc = 0.5;

	/**<Set 2:1 balance through faces.*/
	pablo1.setBalanceCodimension(1);

	/**<Set NO 2:1 balance in the right side of domain.*/
	uint32_t nocts = pablo1.getNumOctants();
	for (unsigned int i=0; i<nocts; i++){
		array<double,3> center = pablo1.getCenter(i);
		double x = center[0];
		if (x>xc)
			pablo1.setBalance(i,false);
	}

	/**<Define a radius.*/
	double radius = 0.4;

	/**<Simple adapt() nref1 times in lower area of domain.*/
	int nref1 = 6;
	for (int iter=0; iter<nref1; iter++){
		nocts = pablo1.getNumOctants();
		for (unsigned int i=0; i<nocts; i++){
			/**<Extract Octant (pointer use).*/
			Octant *oct = pablo1.getOctant(i);
			/**<Compute center of the octant.*/
			array<double,3> center = pablo1.getCenter(oct);
			double x = center[0];
			double y = center[1];

			/**<Set refinement marker=1 for octants inside a circle.*/
			if ((pow((x-xc),2.0)+pow((y-yc),2.0) < pow(radius,2.0)) &&
					(y<yc)){
				pablo1.setMarker(oct, 1);
			}
		}
		/**<Adapt octree, update connectivity and write.*/
		pablo1.adapt();
		pablo1.updateConnectivity();
		pablo1.write("pablo00001_iter"+to_string(static_cast<unsigned long long>(iter+2)));
	}

	/**<While adapt() nref2 times in the upper area of domain.
		* (Useful if you work with center of octants) */
	int nref2 = 5;
	int iter = 0;
	bool done = true;
	while(iter<=nref2){
		done = true;
		while(done)
		{
			nocts = pablo1.getNumOctants();
			for (unsigned int i=0; i<nocts; i++){
				/**<Compute center of the octant (index use).*/
				array<double,3> center = pablo1.getCenter(i);
				double x = center[0];
				double y = center[1];
				if ((pow((x-xc),2.0)+pow((y-yc),2.0) < pow(radius,2.0)) &&
						(y>yc) && iter<=nref2 && pablo1.getLevel(i)<=iter+1){

					/**<Set refinement marker=1 for octants inside a circle.*/
					pablo1.setMarker(i, 1);
				}
			}
			done = pablo1.adapt();
			pablo1.updateConnectivity();
			pablo1.write("pablo00001_iter"+to_string(static_cast<unsigned long long>(iter+nref1+2)));
		}
		iter++;
	}
	/**<Globally refine one level, update the connectivity and write the octree.*/
	pablo1.adaptGlobalRefine();
	pablo1.updateConnectivity();
	pablo1.write("pablo00001_iter"+to_string(static_cast<unsigned long long>(iter+nref1+3)));
}

/*!
* Main program.
*/
int main(int argc, char *argv[])
{
#if BITPIT_ENABLE_MPI==1
	MPI_Init(&argc,&argv);
#else
	BITPIT_UNUSED(argc);
	BITPIT_UNUSED(argv);
#endif

	int nProcs;
	int rank;
#if BITPIT_ENABLE_MPI==1
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
	nProcs = 1;
	rank   = 0;
#endif

	// Initialize the logger
	log::manager().initialize(log::SEPARATE, false, nProcs, rank);
	log::cout() << log::fileVerbosity(log::INFO);
	log::cout() << log::consoleVerbosity(log::QUIET);

	// Run the example
	try {
		run();
	} catch (const std::exception &exception) {
		log::cout() << exception.what();
		exit(1);
	}

#if BITPIT_ENABLE_MPI==1
	MPI_Finalize();
#endif
}
