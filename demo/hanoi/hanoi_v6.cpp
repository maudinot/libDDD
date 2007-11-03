/** An example resolution of the famous towers of Hanoi puzzle. *
 *  v5 : This variant uses only DDD and  saturation,  *
 *  it exploits the fact that the same transition relation is applied to each ring to remove all args from move_ring
 *  v6 : saturation improved : the test no_ring_above resaturates before returning
*/


#include <cstring>
#include <string>
#include <iostream>
using namespace std;

#include "DDD.h"
#include "DED.h"
#include "MemoryManager.h"

// we use one DDD variable per ring, ring 0 is the topmost, 
// and is stored at the bottom of the DDD
static int NB_RINGS= 5;
// Each variable domain is {0,1,2} expressing the pole the variable is on
static int NB_POLES= 10;

void initName() {
  char buff [12];
  for (int i=0; i< NB_RINGS; i++) {
    sprintf(buff,"ring %d",i);
    DDD::varName(i,buff);
  }
}


// predeclaration
GHom saturate ();

// Removes any path such that one of the variables takes value i or value j
// Not test on variable number means this operation should be used from "mid-height"
class _no_ring_above : public StrongHom {
  // the 2 poles that have to be clear
  int i_,j_;
public :
  _no_ring_above (int i, int j) { 
    /// force to have ii < jj since the operation is commutable,  
    // _no_ring_above(i,j) = _no_ring_above(j,i)
    // hence we have a canonical form
    if (i < j) 
      {
	i_ = i; 
	j_ = j; 
      }
    else
      {
	i_ = j ;
	j_ = i ;
      }
  }

  GDDD phiOne() const {
    return GDDD::one;
  }     
  
  // reject any path with ANY ring that is on pole i or pole j
  GHom phi(int vr, int vl) const {
    if ( vl == i_ || vl == j_ )
      // cut this branch and exploration
      return GDDD::null;
    else
      // propagate this test AND (re)saturate resulting nodes
      return GHom(vr,vl, saturate() &GHom(this));
  }

  size_t hash() const {
    return (i_ &  j_<<16)  *  9749;
  }

  bool operator==(const StrongHom &s) const {
    	_no_ring_above* ps = (_no_ring_above*)&s;
	return i_ == ps->i_ && j_ == ps->j_ ;
  }
  
};

// generic version no ring specified, just apply to current ring
class _move_ring : public StrongHom {
  
public :
  
  GDDD phiOne() const {
    return GDDD::one;
  }                   
  
  GHom phi(int vr, int vl) const {
    // ring reached 
    // try to move to all new positions
    // Initialize res with Id
    GHom res = GHom(vr,vl) ;
    for (int i=0 ; i <NB_POLES ; i++) {
      // test all possible moves from current position = vl
      if (i != vl) {
	// first of all saturate successor node then
	// update ring position and test no ring above
	// no_ring_above propagates on the bottom of the DDD ; it returns 0 if preconditions are not met 
	// or a DDD with only paths where the move was legal
	// Additionnally we resaturate the results of this test before using them
	res = (res + ( GHom (vr , i) & saturate() & new _no_ring_above(i , vl) )) & saturate()  ;
      }
    }
    return res ;
  }
  
  size_t hash() const {
    return 6961;
  }
  
  bool operator==(const StrongHom &s) const {
    return true;
  }
  
};

// to be more pleasant for users  
GHom move_ring ( ) {
  return new _move_ring ();
}

// "saturate" fires all events that can be fired from a given node to
//  the leaves and returns a saturated node (� la Ciardo's RecFireAndSat).
GHom saturate () {
  return fixpoint(move_ring());
}
  
  
int main(int argc, char **argv){
  if (argc == 2) {
    NB_RINGS = atoi(argv[1]);
  }

  // Define a name for each variable
  initName();

  // The initial state
  // User program variables should be DDD not GDDD, to prevent their garbage collection
  DDD M0 = GDDD::one ;
  // construct an initial state for the problem, all rings are on pole 0
  for (int i=0; i<NB_RINGS ; i++ ) {
    // note the use of left-concat (adding at the top of the structure), 
    M0 = DDD(i,0, M0);
    // expression is equivalent to : DDD(i,0) ^ MO
    // less expensive than right-concat which forces to recanonize nodes
    // would be written : for ( i--) M0 = M0 ^ DDD(i,0);
  }

  // Consider one single saturate event that recursively fires all events 
  // Saturate topmost node <=> reach fixpoint over transition relation
  DDD ss = saturate() (M0);

  // stats
  cout << "Number of states : " << ss.nbStates() << endl ;
  cout << "Final/Peak nodes : " << ss.size() << "/" << DDD::peak() << endl;
  cout << "Cache entries : " << MemoryManager::nbDED() <<endl ;
}