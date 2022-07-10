#include "crl_permlogic.h"

#ifdef PERMLOGIC_EXISTS
#include "PermLogicCore.hxx"
#endif

double permLogic(double u1, double u2, double u3, double u4) {
#ifdef PERMLOGIC_EXISTS
    return permlogic_core<double>(u1, u2, u3, u4);
#endif
    return 1.0;
}