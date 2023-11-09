#include"ClientOp.h"

int main()
{
	ClientOp c;
	int sel = 1;
	while (1)
	{
		int sel=c.usage();
		if (sel == 0)
		{
			break;
		}
	}
	return 0;
}