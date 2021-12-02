module herring_decoder (
	// Clock source (50 MHz oscillator)
	input clk_src,
	
	// CPU clock lines (PHI2 and PHI2O are duplicated)
	output cpu_clk_in,
	output cpu_clk_out,
	
	// Address bus
	input [15:10] address,
	
	// Decoder outputs
	output [7:0] decoder,
	
	// RWB pin
	input rw
);

	assign cpu_clk_out = cpu_clk_in;

	cpu_clk ClockGen(clk_src, cpu_clk_in);
	
	wire [15:0] full_address = {address, 10'b0};

    // RAM-Write Flag
    assign decoder[0] = ~(cpu_clk_out & ~rw);

	// Note: Low RAM at 0x0000 to 0x7FFF (32KB)

    // ROM at 0xE000 (8KB)
    assign decoder[1] = ~(full_address >= 16'hE000);

    // ACIA Console at 0x8000 (1KB)
    assign decoder[2] = ~(full_address >= 16'h8000 && full_address < 16'h8400);

	// VIA at 0x8400 (1KB)
    assign decoder[3] = ~(full_address >= 16'h8400 && full_address < 16'h8800);
    
    // 0x8800 (1KB)
    assign decoder[4] = ~(full_address >= 16'h8800 && full_address < 16'h8C00);

    // 0x8C00 (1k)
    assign decoder[5] = ~(full_address >= 16'h8C00 && full_address < 16'h9000);

    // 0x9000 to 0xDFFF (20k)
    assign decoder[6] = ~(full_address >= 16'h9000 && full_address < 16'hE000);
	
	// Currently unused
	assign decoder[7] = 1'bz;

endmodule
