/* 
 * File:   Protocol.cpp
 * Author: RM
 * 
 * Created on 17. August 2013, 22:53
 */

#include "Protocol.h"
#include "MHESonCore.h"

const string    Protocol::FILEPATH_BASE         = "MHESonProtocol_";
ofstream        Protocol::_protocol;
string          Protocol::_currentPhase         = "";
uint            Protocol::_currentPhaseCounter  = 0;
bool            Protocol::_printToConsole       = false;

Protocol::Protocol()
{
}

Protocol::Protocol(const Protocol& orig)
{
}

Protocol::~Protocol()
{
}

void Protocol::init(string filepath, string subdirectory, bool printToConsole)
{
    _printToConsole = printToConsole;
    
    string completeFilepath;
    if (filepath == "") {
        subdirectory        = subdirectory != "" ? subdirectory + "/" : "";
        completeFilepath    = subdirectory + Protocol::FILEPATH_BASE;
        
        /*
        // Getting current date in C++11: See http://stackoverflow.com/questions/16357999/current-date-and-time-as-string
        auto t  = time(nullptr);
        auto tm = *localtime(&t);
        std::cout << put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;
        */
        
        // Get current date, doing it it ye olde way (at this time, the cygwin 
        // repository doesn't offer g++ 4.8, which would allow us to operate
        // with dates, strings and numbers in a way sane people would expect).
        time_t t        = time(0);
        struct tm* now  = localtime( &t );
        
        string date;
        // Append leading zeros, if necessary
        string leadingZeroMo    = (now->tm_mon + 1)  < 10 ? "0"  : "";
        string leadingZeroD     = (now->tm_mday + 1) < 10 ? "0"  : "";
        string leadingZeroH     = (now->tm_hour + 1) < 10 ? "0"  : "";
        string leadingZeroMi    = (now->tm_min + 1) < 10 ?  "0"  : "";
        string leadingZeroSec   = (now->tm_sec) < 10 ?  "0"  : "";
        
        // Concatenate to date string
        stringstream stream;
        stream << (now->tm_year + 1900) << '-'
             << leadingZeroMo   << (now->tm_mon + 1) << '-'
             << leadingZeroD    << now->tm_mday << "_"
             << leadingZeroH    << now->tm_hour << "-"
             << leadingZeroMi   << now->tm_min << "-"
             << leadingZeroSec  << now->tm_sec
             << endl;
        
        // Redirect stream into date string
        stream >> date;
        
        // Create full file path
        completeFilepath += date + ".txt";
    }
    
    else
        completeFilepath = filepath;        
    
    // Open date-stamped protocol file
    Protocol::_protocol.open(completeFilepath, ios::out);
    
    // Print file header
    _protocol << "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°" << endl;
    _protocol << "°                MHESon " << ParametrizationSettings::META_VERSION << "                     °" << endl;
    _protocol << "°         Last change: " << ParametrizationSettings::META_LAST_CHANGE << "               °" << endl;
    _protocol << "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°" << endl;
    
    if (_printToConsole) {
        cout << "*************************************************" << endl;
        cout << "*                MHESon " << ParametrizationSettings::META_VERSION << "                     *" << endl;
        cout << "*         Last change: " << ParametrizationSettings::META_LAST_CHANGE << "               *" << endl;
        cout << "*************************************************" << endl;        
    }
}

void Protocol::finalize()
{
    _currentPhase           = "";
    _currentPhaseCounter    = 0;
    _protocol.close();
}

void Protocol::introducePhase(const string phase)
{
    _currentPhase = phase;
    
    _protocol << endl;
    _protocol << "================================================" << endl;
    _protocol << "*** PHASE " << _currentPhaseCounter << ": " << _currentPhase << " ***" << endl;
    _protocol << "------------------------------------------------" << endl << endl;
    
    if (_printToConsole) {
        cout << endl;
        cout << "================================================" << endl;
        cout << "*** PHASE " << _currentPhaseCounter << ": " << _currentPhase << " ***" << endl;
        cout << "------------------------------------------------" << endl << endl;      
    }
}

void Protocol::concludePhase(chrono::milliseconds duration)
{
    _protocol << endl << "  ## Time elapsed: " << duration.count() << " ms" << endl;
    _protocol << endl;
    _protocol << "------------------------------------------------" << endl;
    _protocol << "*** Finished phase " << _currentPhaseCounter << " ***" << endl;
    _protocol << "================================================" << endl << endl << endl;
    
    if (_printToConsole) {
        cout << endl << "  ## Time elapsed: " << duration.count() << " ms" << endl;
        cout << endl;
        cout << "------------------------------------------------" << endl;
        cout << "*** Finished phase " << _currentPhaseCounter << " ***" << endl;
        cout << "================================================" << endl << endl << endl;
    }    
    
    _currentPhaseCounter++;
}

ofstream& Protocol::protocol()
{
    return _protocol;
}

bool& Protocol::printToConsole()
{
    return _printToConsole;
}