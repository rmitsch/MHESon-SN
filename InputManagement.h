/* 
 * File:   InputManagement.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 22:46
 */

#ifndef INPUTMANAGEMENT_H
#define	INPUTMANAGEMENT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <cstdlib>
#include <time.h>
#include <utility>

#include "Definitions.h"
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"
#include "FileMetadata.h"

class InputManagement
{
    public:
        InputManagement();
        InputManagement(const InputManagement& orig);
        virtual ~InputManagement();
        
        /**
         * 
         * @param inputPath
         * @param fileType
         * @param graph
         * @return 
         */
        static int parseInputFile(string inputPath, FileMetadataConstants::DataFileType fileType, Graph& graph);
    
        /**
         * Preprocesses raw KONECT-DBLP data (sorts and weigts edges, removes
         * unnecessary data).
         * Writes processed data in new file.
         * @param inputPath Raw KONECT-DBLP data.
         * @param outputPath File for preprocessed data.
         */
        static void prepareDBLP(string inputPath, string outputPath);
        
    private:
        /* --- Variables --- */
        
        // Defines number of different random values to be created for
        // assignment to edge weights of SNAP datasets.
        static const uint SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS = 1000;
        
        /* --- Methods --- */
        
        /**
         * 
         * @param inputPath
         * @param graph
         * @return 
         */
        static uint parseSNAPFile(ifstream& inputFile, Graph& graph);
        
        /**
         * 
         * @param inputFile
         * @param graph
         * @return 
         */
        static uint parseDBLPFile(ifstream& inputFile, Graph& graph);
        
        /**
         * 
         * @param inputFile
         * @param graph
         * @return 
         */
        static uint parseADVOGATOFile(ifstream& inputFile, Graph& graph);
};

#endif	/* INPUTMANAGEMENT_H */

