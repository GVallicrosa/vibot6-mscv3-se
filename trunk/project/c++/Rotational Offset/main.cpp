#include "RotationalOffset.h"

int main(void)
{
	Contour C[5] ={{1,2,5},{1,5,5},{50,97,5},{1,31,5},{20,87,5}} ;
	IRO *RO = new cRotationalOffset();

	RO->GetMinRadius(C);
	
	system("pause");

	return(0);
}
