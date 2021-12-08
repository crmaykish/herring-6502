module fishtank(
    inout [7:0] DATA,
    input [15:0] ADDR,
    input RW,
    output DEC0,
    output DEC1,
    output DEC2,
    output DEC3,
    output CLK_CPU,
    input CLK
);
    /* === CPU CLOCK GENERATOR === */

    // 0.75 MHz clock from 12 MHz oscillator
    reg [20:0] clock_counter = 0;
    assign CLK_CPU = clock_counter[8];

    always @(posedge CLK) begin
        clock_counter <= clock_counter + 1;
    end

    /* === ADDRESS DECODER === */

    // RAM Write Enable
    assign DEC0 = ~(CLK_CPU && ~RW);

    // ROM Enable
    assign DEC1 = ~(ADDR >= 16'hE000);
    
    // ACIA Enable
    assign DEC2 = ~(ADDR >= 16'h8000 && ADDR < 16'h8400);

    // VIA Enable
    assign DEC3 = ~(ADDR >= 16'h9000 && ADDR < 16'hE000);


    /* === MEMORY === */

    reg [7:0] ram [0:63];


    initial begin
        ram[0] = 8'hAB;
        ram[1] = 8'hCD;
        ram[2] = 8'hEF;
    end

    wire [7:0] dout  = ram[ADDR[5:0]];

    assign DATA = ~DEC3 && RW ? dout : {8{1'bz}};

    always @(negedge CLK_CPU) begin
        if (~DEC3 && ~RW)
            ram[ADDR[5:0]] <= DATA;
    end

endmodule
