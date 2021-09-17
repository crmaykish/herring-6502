module address_decoder (
    input [15:10] ADDR,
    input RWB, PHI2,
    output [7:0] DECODE
);
    wire [15:0] full_address = {ADDR, 10'b0};

    // RAM-Write Flag
    assign DECODE[0] = ~(PHI2 & ~RWB);

    // ROM Enable (0xE000 - 0xFFFF)
    assign DECODE[1] = ~(full_address >= 16'hE000);

    // ACIA Console (0x8000)
    assign DECODE[2] = ~(full_address >= 16'h8000 && full_address < 16'h8400);

    assign DECODE[3] = 1'b1;
    
    assign DECODE[4] = 1'b1;

    assign DECODE[5] = 1'b1;

    assign DECODE[6] = 1'b1;

    // Bus-enable pin (always high)
    assign DECODE[7] = 1'b1;

endmodule
