void
hex_print(char* data, int length)
{
	int ptr = 0;
	for(;ptr < length;ptr++)
	{
		printf("0x%02x ",(unsigned char)*(data+ptr));
	}
	printf("\n");
}
void
bit_print(char* data, int length)
{
	unsigned char mask = 0x01;
	int ptr = 0;
	int bit = 0;
	for(;ptr < length;ptr++)
	{
		for(bit = 7;bit >= 0;bit--)
		{
			if ((mask << bit) & (unsigned char)*(data+ptr))
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
		}
		printf(" ");
	}
	printf("\n");
}

