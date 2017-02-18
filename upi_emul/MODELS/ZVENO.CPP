#include <stdlib.h>
#include <zveno.hpp>
#include <link.hpp>

RegList *RegList::Ptr=NULL;

LINK_ZVENO(NilZveno,"Ничего")

void  _HUGE NilZveno::Reset(void) { return; }
float _HUGE NilZveno::Calc(float val) { return val; }
int   _HUGE NilZveno::Build(void) { return 1; }

int NilZveno::UserEnter(void) { return 1; }



