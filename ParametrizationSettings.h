/* 
 * File:   ParametrizationSettings.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 23:34
 */

#ifndef PARAMETRIZATIONSETTINGS_H
#define	PARAMETRIZATIONSETTINGS_H

#include <string>
#include "Protocol.h"

using namespace std;

class ParametrizationSettings
{
    public:
        ParametrizationSettings();
        ParametrizationSettings(const ParametrizationSettings& orig);
        virtual ~ParametrizationSettings();
    
        static void log();
        
        /**
         * x: Probability of any given vertex to be included in influence set.
         */
        static float            INFLUENCE_SET_PROBABILITY;
        
        /**
         * k: Number of rounds.
         */
        static uint             NUMBER_OF_ROUNDS;
        
        /**
         * l: Number of groups.
         */
        static uint             NUMBER_OF_GROUPS;
        
        /**
         * c: Defines divisor (base) for calculation of group limits
         * (see CDHS: 4.2 - Maximizing Revenue..., Definition 5).
         */
        static const uint       GROUP_LIMIT_DIVISOR             = 2;
        
        /**
         * Number of runs to be conducted before averaging results. 
         */
        static const uint       NUMBER_OF_RUNS                  = 10;
        
        /**
         * Intrinic base interest of each buyer in purchasing the product,
         * without network effects.
         * @deprecated Not considered in current version of implementation.
         */
        static constexpr float  INTRINSIC_INTEREST_VALUE        = 0;
        
        /**
         * Default myopic price, if not yet calculated. Remains zero
         * if vertex doesn't posess any incoming edges.
         */
        static constexpr float  MYOPIC_PRICE_DEFAULT            = 0;
        
        /**
         * Defines myopic price to be assigned to members of the influence set
         * (important for sorting process).
         */
        static constexpr float  MYOPIC_PRICE_INFLUENCE_SET      = -1;
        
        /**
         * Defines myopic price to be assigned to deaf vertices (important for 
         * sorting and deleting excess vertices).
         */
        static constexpr float  MYOPIC_PRICE_DEAF_NODES         = -2;
        
        /**
         * Decides about absorbing or ignoring surplus vertices in vertex 
         * group pools (NGPs).
         */
        static const bool       GM_ABSORB_SURPLUS_NODES         = true;
        
        /**
         * Determines if, during vertex-group mapping, lower limits (-> group 
         * offers) are to be adapted if group absorbs surplus vertices, which happens
         * in oversaturated NGPs.
         * @deprecated Set to false; shouldn't be changed.
         */
        static const bool       GM_DYNAMIC_SURPLUS_LIMITS       = false;
        
        /*
         * Describes probability of any adjacent vertex entering earlier than
         * examined vertex (so mentioned neighbour can exert influence; purchase
         * provided). For derivation of formula see documentation / paper.
         */
        static float            ANCDHS_POST_ROUND_PROBABILITY;
        
        /**
         * Adjusts calculated emmetropic prices. Not theoretically motivated,
         * more of a best practice thing.
         */
        static constexpr float  ANCHDS_EP_ADJUSTMENT_FACTOR     = static_cast<float>(1) / 2;
        
        /**
         * Indicates implementation version.
         */
        static constexpr float  META_VERSION                    = 0.9;
        
        /**
         * Indicates date of last change.
         */
        static const string     META_LAST_CHANGE;
        
        /**
         * Calculates post round probability used in AN-CDHS GMH.
         * Has to be called once the number of rounds is specified (and 
         * different from 1).
         * @return Calculated post round probability.
         */
        static float calculatePostRoundProbability();
        
    private:
        

} ;

#endif	/* PARAMETRIZATIONSETTINGS_H */

