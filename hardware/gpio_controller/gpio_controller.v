module GPIO_Latch(DATA, CLK, EN, IO);
    input [7:0] DATA;
    input CLK, EN;
    output reg [7:0] IO;

    // Note: Neg edge triggering is a bit of a hack since the clock input is PHIO and not PHI2
    // Triggering on the posedge is too fast. The data bus is not ready yet.
    always @(negedge CLK) begin
        if (~EN)
            IO <= DATA;
    end

endmodule