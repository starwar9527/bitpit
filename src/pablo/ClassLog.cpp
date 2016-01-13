/*
 * ClassLog.cpp
 *
 *  Created on: 3 dec 2014
 *      Author: marco
 */

#include "ClassLog.hpp"
#include <fstream>

using namespace std;

#if ENABLE_MPI
ClassLog::ClassLog(string filename_,MPI_Comm comm_) : m_filename(filename_),m_comm(comm_) {};
#else
ClassLog::ClassLog(string filename_) : m_filename(filename_) {};
#endif

ClassLog::~ClassLog() {};

// ----------------------------------------------------------------------------------- //
void ClassLog::writeLog(string msg) {

	// =================================================================================== //
	// void Write_Log(string msg)                                                          //
	//                                                                                     //
	// Append message into a .log file                                                     //
	// =================================================================================== //
	// INPUT                                                                               //
	// =================================================================================== //
	// - msg    : string, message to be appended into the .log file.                       //
	// =================================================================================== //
	// OUTPUT                                                                              //
	// =================================================================================== //
	// - none                                                                              //
	// =================================================================================== //

	// =================================================================================== //
	// VARIABLES DECLARATION                                                               //
	// =================================================================================== //

	// Local variables
	ofstream              file_handle;

	// Counters
	// none

	// =================================================================================== //
	// APPEND MESSAGE TO THE LOG FILE                                                      //
	// =================================================================================== //

	int rank = 0;

#if ENABLE_MPI
	bool flag = MPI::Is_finalized();
	if (!(flag))
		int error_flag = MPI_Comm_rank(m_comm,&rank);
#endif
	if(rank == 0){
		// Open the .log file
		file_handle.open(m_filename.c_str(), ifstream::app);
		if(!file_handle.is_open())
			exit(1);

		// Append message to the .log file
		file_handle << msg << endl;

		// Close file
		file_handle.close();
	}
//#if ENABLE_MPI
//	error_flag = MPI_Barrier(comm);
//#endif
	return; };
