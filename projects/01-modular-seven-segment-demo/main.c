#include <REGX52.H>
#include "Delayms.h"
#include "Nixie.h"

void main()
{
	while(1)
	{
		P2_0 = 1;
		Delayms(500);
		P2_0 = 0;
		Delayms(500);
	}
}