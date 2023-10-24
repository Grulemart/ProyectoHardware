
// Set the processor into idle state 
void power_hal_wait (void)  {
	PCON |= 0x01; 
}
