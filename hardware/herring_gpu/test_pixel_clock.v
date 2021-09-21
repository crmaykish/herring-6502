`include "pixel_clock.v"

module test_pixel_clock();

    reg src_clock;
    wire pclock;

    pixel_clock pixel_clock_test_inst(src_clock, pixel_clock);

    initial begin
        $dumpfile("pixel_clock.vcd");
        $dumpvars(0, test_pixel_clock);

        repeat(10) begin
            src_clock = 1'b0; #10;
            src_clock = 1'b1; #10;
        end
    end

endmodule