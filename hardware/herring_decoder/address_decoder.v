module address_decoder (
    input [15:10] ADDR,
    input RWB, PHI2,
    output [7:0] DECODE
);
    wire [15:0] full_address = {ADDR, 10'b0};

    // RAM-Write Flag
    assign DECODE[0] = ~(PHI2 & ~RWB);

	// Note: Low RAM at 0x0000 to 0x7FFF (32KB)

    // ROM at 0xE000 (8KB)
    assign DECODE[1] = ~(full_address >= 16'hE000);

    // ACIA Console at 0x8000 (1KB)
    assign DECODE[2] = ~(full_address >= 16'h8000 && full_address < 16'h8400);

	// VIA at 0x8400 (1KB)
    assign DECODE[3] = ~(full_address >= 16'h8400 && full_address < 16'h8800);
    
    // 0x8800 (1KB)
    assign DECODE[4] = ~(full_address >= 16'h8800 && full_address < 16'h8C00);

    // 0x8C00 (1k)
    assign DECODE[5] = ~(full_address >= 16'h8C00 && full_address < 16'h9000);

    // 0x9000 to 0xDFFF (20k)
    assign DECODE[6] = ~(full_address >= 16'h9000 && full_address < 16'hE000);

    // Bus-enable pin (always high)
    assign DECODE[7] = 1'b1;

endmodule
