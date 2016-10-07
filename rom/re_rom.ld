MEMORY
{
  rom_seg :                        org = 0x40000000, len = 65536
}


SECTIONS
{
	.rom 0x40000000 :
	{
	} > rom_seg
}
