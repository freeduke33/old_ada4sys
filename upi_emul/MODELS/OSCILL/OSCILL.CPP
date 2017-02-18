#include <Oscill.hpp>

TransferOscill::TransferOscill(void)
{
 tik=0;
 Freq=13.5;
 Betta=0.27;
 return;
}

void _HUGE TransferOscill::Reset(void)
{
 tik=0;
 return;
}

int TransferOscill::ParamSize(void) { return sizeof(struct _Oscill); }


