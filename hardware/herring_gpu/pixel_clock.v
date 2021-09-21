module pixel_clock(input CLK_SOURCE, output reg PIXEL_CLK = 1'b0);

    always @(posedge CLK_SOURCE) begin
        PIXEL_CLK = ~PIXEL_CLK;
    end

endmodule