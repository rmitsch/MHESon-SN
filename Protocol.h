/* 
 * File:   Protocol.h
 * Author: RM
 *
 * Created on 17. August 2013, 22:53
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>

#include "Definitions.h"

using namespace std;
typedef unsigned int uint;

class Protocol
{
    public:
        Protocol();
        Protocol(const Protocol& orig);
        virtual ~Protocol();
    
        /**
         * Inits Protocol class.
         * @param filepath
         */
        static void         init(string filepath = "", string subdirectory = "", bool printToConsole = false);
        
        /**
         * Finalizes Protocol class.
         */
        static void         finalize();
        
        /**
         * Returns reference to protocol ofstream instance.
         * @return Reference to protocol file.
         */
        static ofstream&    protocol();
        
        /**
         * Decides if protocol output should be printed on console too.
         * @return Reference to described internal variable.
         */
        static bool&         printToConsole();

        /**
         * Prints header for new phase.
         * @param phase Name/title of phase.
         */
        static void         introducePhase(const string phase);
        
        /**
         * Prints footer for current phase.
         */
        static void         concludePhase(chrono::milliseconds duration);
        
    private:
        static ofstream     _protocol;
        static string       _currentPhase;
        static uint         _currentPhaseCounter;
        static bool         _printToConsole;
        static const string FILEPATH_BASE;
        
} ;

#endif	/* PROTOCOL_H */

