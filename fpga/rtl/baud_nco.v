// baud_nco - fractional (NCO / phase-accumulator) baud strobe generator.
//
// Drop-in replacement for cores/.../baud_gen with the same ports, but instead of a
// rounded integer divider it accumulates a fixed increment every clock and emits a
// one-cycle en_16x strobe on each accumulator overflow. The AVERAGE strobe rate is

`default_nettype none
module baud_nco #(
    parameter integer CLK_HZ     = 27_000_000,
    parameter integer BAUD       = 115200,
    parameter integer OVERSAMPLE = 16,
    parameter integer ACC_W      = 24        // accumulator width; larger = more exact
) (
    input  wire clk,
    input  wire rst_n,
    output reg  en_16x
);
    // INC = round(2^ACC_W * (BAUD*OVERSAMPLE) / CLK_HZ). The 64'd1 makes the whole
    // expression evaluate in 64 bits so the 2^ACC_W * rate product can't overflow.
    localparam integer INC =
        (BAUD * OVERSAMPLE * (64'd1 << ACC_W) + CLK_HZ/2) / CLK_HZ;

    reg [ACC_W-1:0] acc;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            acc    <= {ACC_W{1'b0}};
            en_16x <= 1'b0;
        end else begin
            // Accumulate; the carry out of bit ACC_W-1 is the overflow strobe.
            {en_16x, acc} <= {1'b0, acc} + INC[ACC_W:0];
        end
    end
endmodule
`default_nettype wire
