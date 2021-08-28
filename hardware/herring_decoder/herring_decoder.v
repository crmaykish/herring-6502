module herring_decoder (
	// Clock source (50 MHz oscillator)
	input clk_src,
	
	// CPU clock out (input from FPGA perspective)
	input cpu_clk_out,
	
	// CPU clock in (output from FPGA perspective)
	output reg cpu_clk_in,
	
	// Address bus
	input [15:10] address,
	
	// Decoder outputs
	output [7:0] decoder,
	
	// RWB pin
	input rw
);
	
	reg[27:0] counter=28'd0;
	
	// Divide the 50 MHz source clock by 25 for a 2MHz CPU clock
	parameter DIVISOR = 28'd25;
	
	always @(posedge clk_src)
	begin
		counter <= counter + 28'd1;
		
		if(counter>=(DIVISOR-1))
			counter <= 28'd0;

		cpu_clk_in <= (counter<DIVISOR/2)?1'b1:1'b0;
	end
	
	// RAM Write
	assign decoder[0] = ~(cpu_clk_out & ~rw);	// Clock high and RW low (write)
	
	// RAM High (disabled)
	assign decoder[1] = 1;
	
	assign decoder[2] = 1;
	
	assign decoder[3] = 1;
	
	assign decoder[4] = 1;
	
	// VIA 1 (0x8400)
	assign decoder[5] = ~(address[15] & ~address[14] & ~address[13] & ~address[12] & ~address[11] & address[10]);
	
	// ACIA 1 (0x8000)
	assign decoder[6] = ~(address[15] & ~address[14] & ~address[13] & ~address[12] & ~address[11] & ~address[10]);
	
	// Bus enable (enabled)
	assign decoder[7] = 1;

endmodule
