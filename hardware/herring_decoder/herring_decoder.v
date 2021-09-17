module herring_decoder (
	// Clock source (50 MHz oscillator)
	input clk_src,
	
	// CPU clock out (input from FPGA perspective)
	input cpu_clk_out,
	
	// CPU clock in (output from FPGA perspective)
	output cpu_clk_in,
	
	// Address bus
	input [15:10] address,
	
	// Decoder outputs
	output [7:0] decoder,
	
	// RWB pin
	input rw
);
	cpu_clk ClockGen(clk_src, cpu_clk_in);

	address_decoder AddressDecoder(address, rw, cpu_clk_out, decoder);

endmodule
