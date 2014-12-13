/* 
 * File:   ParametrizationSettings.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 23:34
 */

#include "ParametrizationSettings.h"

// Default settings for various settings
const string    ParametrizationSettings::META_LAST_CHANGE               = "2013-12-05";
float           ParametrizationSettings::INFLUENCE_SET_PROBABILITY      = 0.5;
uint            ParametrizationSettings::NUMBER_OF_ROUNDS               = 1;
uint            ParametrizationSettings::NUMBER_OF_GROUPS               = 8;
float           ParametrizationSettings::ANCDHS_POST_ROUND_PROBABILITY  = (ParametrizationSettings::NUMBER_OF_ROUNDS - 1) / (2 * static_cast<float>(ParametrizationSettings::NUMBER_OF_ROUNDS));

        
ParametrizationSettings::ParametrizationSettings()
{
}

ParametrizationSettings::ParametrizationSettings(const ParametrizationSettings& orig)
{
}

ParametrizationSettings::~ParametrizationSettings()
{
}

void ParametrizationSettings::log()
{
    ofstream& protocol = Protocol::protocol();
    
    // Logging to file
    protocol << endl;
    protocol << "================================================" << endl;
    protocol << "*** PARAMETRIZATION SETTINGS ***" << endl;
    protocol << "------------------------------------------------" << endl << endl;
    
    protocol << "Printing parametrization settings. " << endl;
    protocol << "For further information on parameters and their usage see documentation." << endl << endl;
    
    protocol << "  :: INFLUENCE_SET_PROBABILITY\t\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
    protocol << "  :: NUMBER_OF_ROUND\t\t\t\t" << ParametrizationSettings::NUMBER_OF_ROUNDS << endl;
    protocol << "  :: NUMBER_OF_GROUPS\t\t\t\t" << ParametrizationSettings::NUMBER_OF_GROUPS << endl;
    protocol << "  :: GROUP_LIMIT_DIVISOR\t\t\t" << ParametrizationSettings::GROUP_LIMIT_DIVISOR << endl;
    protocol << "  :: INTRINSIC_INTEREST_VALUE\t\t" << ParametrizationSettings::INTRINSIC_INTEREST_VALUE << endl;
    protocol << "  :: MYOPIC_PRICE_DEFAULT\t\t\t" << ParametrizationSettings::MYOPIC_PRICE_DEFAULT << endl;
    protocol << "  :: MYOPIC_PRICE_INFLUENCE_SET\t\t" << ParametrizationSettings::MYOPIC_PRICE_INFLUENCE_SET << endl;
    protocol << "  :: MYOPIC_PRICE_DEAF_NODES\t\t" << ParametrizationSettings::MYOPIC_PRICE_DEAF_NODES << endl;
    protocol << "  :: GM_ABSORB_SURPLUS_NODES\t\t" << ParametrizationSettings::GM_ABSORB_SURPLUS_NODES << endl;
    protocol << "  :: GM_DYNAMIC_SURPLUS_LIMITS\t\t" << ParametrizationSettings::GM_DYNAMIC_SURPLUS_LIMITS << endl;
    protocol << "  :: ANCDHS_POST_ROUND_PROBABILITY\t" << ParametrizationSettings::ANCDHS_POST_ROUND_PROBABILITY << endl;
    protocol << "  :: ANCHDS_EP_ADJUSTMENT_FACTOR\t" << ParametrizationSettings::ANCHDS_EP_ADJUSTMENT_FACTOR << endl;
    protocol << "  :: META_VERSION\t\t\t\t\t" << ParametrizationSettings::META_VERSION << endl;
    protocol << "  :: META_LAST_CHANGE\t\t\t\t" << ParametrizationSettings::META_LAST_CHANGE << endl;
    
    protocol << endl;
    protocol << "------------------------------------------------" << endl;
    protocol << "*** Printed parametrization settings ***" << endl;
    protocol << "================================================" << endl << endl << endl;
    
    // Logging to console
    cout << endl;
    cout << "================================================" << endl;
    cout << "*** PARAMETRIZATION SETTINGS ***" << endl;
    cout << "------------------------------------------------" << endl << endl;
    
    cout << "Printing parametrization settings. " << endl;
    cout << "For further information on parameters and their usage see documentation." << endl << endl;
    
    cout << "  :: INFLUENCE_SET_PROBABILITY\t\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
    cout << "  :: NUMBER_OF_ROUNDS\t\t\t" << ParametrizationSettings::NUMBER_OF_ROUNDS << endl;
    cout << "  :: NUMBER_OF_GROUPS\t\t\t" << ParametrizationSettings::NUMBER_OF_GROUPS << endl;
    cout << "  :: GROUP_LIMIT_DIVISOR\t\t" << ParametrizationSettings::GROUP_LIMIT_DIVISOR << endl;
    cout << "  :: INTRINSIC_INTEREST_VALUE\t\t" << ParametrizationSettings::INTRINSIC_INTEREST_VALUE << endl;
    cout << "  :: MYOPIC_PRICE_DEFAULT\t\t" << ParametrizationSettings::MYOPIC_PRICE_DEFAULT << endl;
    cout << "  :: MYOPIC_PRICE_INFLUENCE_SET\t\t" << ParametrizationSettings::MYOPIC_PRICE_INFLUENCE_SET << endl;
    cout << "  :: MYOPIC_PRICE_DEAF_NODES\t\t" << ParametrizationSettings::MYOPIC_PRICE_DEAF_NODES << endl;
    cout << "  :: GM_ABSORB_SURPLUS_NODES\t\t" << ParametrizationSettings::GM_ABSORB_SURPLUS_NODES << endl;
    cout << "  :: GM_DYNAMIC_SURPLUS_LIMITS\t\t" << ParametrizationSettings::GM_DYNAMIC_SURPLUS_LIMITS << endl;
    cout << "  :: ANCDHS_POST_ROUND_PROBABILITY\t" << ParametrizationSettings::ANCDHS_POST_ROUND_PROBABILITY << endl;
    cout << "  :: ANCHDS_EP_ADJUSTMENT_FACTOR\t" << ParametrizationSettings::ANCHDS_EP_ADJUSTMENT_FACTOR << endl;    
    cout << "  :: META_VERSION\t\t\t" << ParametrizationSettings::META_VERSION << endl;
    cout << "  :: META_LAST_CHANGE\t\t\t" << ParametrizationSettings::META_LAST_CHANGE << endl;
    
    cout << endl;
    cout << "------------------------------------------------" << endl;
    cout << "*** Printed parametrization settings. ***" << endl;
    cout << "================================================" << endl << endl << endl;
}

float ParametrizationSettings::calculatePostRoundProbability()
{
    ParametrizationSettings::ANCDHS_POST_ROUND_PROBABILITY  =   (ParametrizationSettings::NUMBER_OF_ROUNDS - 1) / 
                                                                (2 * static_cast<float>(ParametrizationSettings::NUMBER_OF_ROUNDS));
}