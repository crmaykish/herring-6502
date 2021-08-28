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
	
	// 1 = 25 MHz
	// 2 = 12.5 MHz
	// 3 = 6.25 MHz
	// 4 = 3.125 MHz
	// 5 = 1.5625 MHz
	// 6 = 781.25 KHz
	// 7 = 390.63 KHz
	// 8 = 195.31 KHz
	// 9 = 97.66 KHz
	// 10 = 48.83 KHz
	// 11 = 24.41 KHz
	// 12 = 12.21 KHz
	// 13 = 6.10 KHz
	// 14 = 3.05 KHz
	// 15 = 1.53 KHz
	// 16 = 763 Hz
	// 17 = 381 Hz
	// 18 = 191 Hz
	// 19 = 95 Hz
	// 20 = 48 Hz
	// 21 = 24 Hz
	// 22 = 12 Hz
	// 23 = 6 Hz
	// 24 = 3 Hz
	// 25 = 1.5 Hz
	// 26 = 0.75 Hz
	parameter INDEX = 10;

	reg [26:0] counter;
	
	// Clock divider
	always @(posedge clk_src) begin
		counter <= counter + 1;
	end	

	assign cpu_clk_in = counter[INDEX-1];
	
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
