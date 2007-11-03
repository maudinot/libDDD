/* -*- C++ -*- */
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "DDD.h"
#include "DED.h"
#include "Hom.h"
#include "SDD.h"
#include "SDED.h"
#include "SHom.h"

/// This class defines a few utility functions common to DDD.
/// Note that all functions are declared static, so this is more of a namespace than a class.
/// One important function is garbage(), only this MemoryManager::garbage() should be caled :
/// avoid GHom::garbage(), DED::garbage(), GDDD::garbage() (and SDD versions of the same,
/// they should not be called directly.
class MemoryManager{
public:
  /* Mesure*/
  /// Returns the size of the unicity table for DDD.
  static unsigned int nbDDD(){return GDDD::statistics();};
  /// Returns the size of the cache unicity table for DDD. 
  static unsigned int nbDED(){return DED::statistics();};
  /// Returns the size of the unicity table for DDD Homomorphisms.
  static unsigned int nbHom(){return GHom::statistics();};
  /// Returns the size of the unicity table for SDD.
  static unsigned int nbSDD(){return GSDD::statistics();};
  /// Returns the size of the cache unicity table for SDD. 
  static unsigned int nbSDED(){return SDED::statistics();};
  /// Returns the size of the unicity table for SDD Homomorphisms.
  static unsigned int nbShom(){return GShom::statistics();};
  /* Garbage Collector */
  /// Convenience function to mark a node as non collectible.
  /// \todo : track usage and check whether this is useful, SDD version undefined.  
  static void mark(const GDDD &g){g.mark();};
  /// Convenience function to mark a Hom as non collectible.
  /// \todo : track usage and check whether this is useful, SDD version undefined.  
  static void mark(const GHom &h){h.mark();};

  /// Garbage collection function. 
  /// Call this to reclaim intermediate nodes, unused operations and related cache.
  /// Note that this function is quite costly, and it totally destroys the cache unless OTF 
  /// garbage collection is activated.
  static void garbage(){
    // FIXME : if you dont use SDD suppress the following
    SDED::garbage();
    GShom::garbage();
    GSDD::garbage();
    // END FIXME 
    DED::garbage();
    GHom::garbage();
    GDDD::garbage();
  };

  /// Prints some statistics about use of unicity tables, also reinitializes peak sizes.
  /// Side effect in INST_STL version is to reinitialize rebound counters.
  static void pstats(bool reinit=true){
    //cout << " Memory Usage " << MemUsage() << " %" << endl;
    
    // FIXME : if you dont use SDD suppress the following
    SDED::pstats(reinit);
    GShom::pstats(reinit);
    GSDD::pstats(reinit);    
    // END FIXME 

    DED::pstats(reinit);
    GHom::pstats(reinit);
    GDDD::pstats(reinit);    
  }
  
};
#endif