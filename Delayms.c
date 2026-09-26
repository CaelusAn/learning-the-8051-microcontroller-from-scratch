void Delayms(unsigned int tms)		//@12.000MHz
{
	unsigned char i, j;
	while(tms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}