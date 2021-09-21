`include "video_ram.v"

module test_video_ram();

    reg clock, we;
    reg [13:0] address;
    reg [7:0] data_in;
    wire [7:0] data_out;

    video_ram vram_test_inst(clock, we, address, data_in, data_out);

    initial begin
        $dumpfile("vram.vcd");
        $dumpvars(0, test_video_ram);

        address <= 13'h0000;
        we <= 1'b0;
        data_in <= 8'h00;

        clock <= 1'b0; #10;
        clock <= 1'b1; #10;

        address <= 13'h0001;
        data_in <= 8'hAB;
        we <= 1'b1;

        clock <= 1'b0; #10;
        clock <= 1'b1; #10;

        we <= 1'b0;

        clock <= 1'b0; #10;
        clock <= 1'b1; #10;

        address <= 13'h0002;

        clock <= 1'b0; #10;
        clock <= 1'b1; #10;

        clock <= 1'b0; #10;
        clock <= 1'b1; #10;

    end

endmodule