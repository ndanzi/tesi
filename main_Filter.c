

// Filter FIR-like, 8 taps, self checking
// Assuming 12-bit input and output values, and "int" represented on 32-bits.
// Author: Gabriele Boschi.


//Example signal to be filtered adn related expected output
int input_vector[16] = {0, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095};
int expected_output_vector[16] = {0, 255, 511, 767, 1023, 1279, 1535, 1791, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047};

// Filter coefficients
int coeff[8] = {2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048};

// Filter state variable
int filter_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};



int MyFilter(int InputVal)
{
	int ind_taps;
	int tmp_val;
 
	// Shift the input patterns
  for (ind_taps=8; ind_taps>0; ind_taps--)
	{
		filter_state[ind_taps-1] = filter_state[ind_taps-2];
	}
	// Load new input value
	filter_state[0] = InputVal;
	
	// Apply filter and re-scale the result to fit the input resolution
	tmp_val=0;
	for (ind_taps=0; ind_taps<8; ind_taps++)
  {
		tmp_val=tmp_val + (filter_state[ind_taps]*coeff[ind_taps]);
	}
	tmp_val = tmp_val / (4096*8);
	return tmp_val;
  	
}


int main(void)
{
  int index; 
	int passed;
	
	// Output vector calculated on-the-fly
  int calc_output_vector[16];
  
	// Apply filter to input_vector
	for (index=0; index<16; index++)
  {
		calc_output_vector[index] = MyFilter(input_vector[index]);
	}


	// Check filter (self-checking)
	passed = 0xABCD0123;
	for (index=0; index<16; index++)
  {
		if (calc_output_vector[index] != expected_output_vector[index])
		{
			passed = 0;
		}
	}

	if (passed != 0xABCD0123)
	{
		// TEST FAILED
		// Put here specific code to signal test failed.
		//while (1);
	}
 
    
  //while (1); /* Never get out from here */

  return 0; /* This statement is unreachable. It is OK. */
}
