#include "Nat_PlusZero.hpp"
#include "Nat_Const.hpp"
#include "Hom_Select.hpp"

SDD applyZeroPlusX (const SDD & d) {
  GShom select_zero_plus_test = select_hom(NAT, &natPlus, select_hom(LEFT, &SDDnatZero, GShom::id));
  GSDD d1 = select_zero_plus_test(d);
  return extract_value (RIGHT) (d1) 
    + (d-d1);
}

SDD applyXPlusZero (const SDD & d) {
  GShom select_plus_zero_test = select_hom(NAT, &natPlus, select_hom(RIGHT, &SDDnatZero, GShom::id));
  GSDD d1 = select_plus_zero_test(d);
  return extract_value (LEFT) (d1) 
    + (d-d1);
}