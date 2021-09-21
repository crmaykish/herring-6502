module video_ram(
    input CLK,
    input WE,
    input [13:0] ADDR,
    input [7:0] DATA_IN,
    output reg [7:0] DATA_OUT = 8'b0
);

    reg [7:0] v_ram[0:16383];

    always @(posedge CLK) begin
        if (WE) v_ram[ADDR] <= DATA_IN;
        DATA_OUT <= v_ram[ADDR];
    end

    // SB_SPRAM256KA spram_inst (
    //     .ADDRESS(ADDR),
    //     .DATAIN(DATA_IN),
    //     .MASKWREN({WE, WE, WE, WE}),
    //     .WREN(WE),
    //     .CHIPSELECT(1'b1),
    //     .CLOCK(CLK),
    //     .STANDBY(1'b0),
    //     .SLEEP(1'b0),
    //     .POWEROFF(1'b1),
    //     .DATAOUT(DATA_OUT)
    // );

endmodule