module framebuffer(
    input PIXEL_CLOCK,
    input [10:0] PX,
    input [9:0] PY,
    input ON_SCREEN,
    output reg RED, GREEN, BLUE,

    input [7:0] X_POS, Y_POS,
    input [2:0] COLOR,
    input WRITE
);

    // 200x150 resolution, 3-bit color
    parameter RESOLUTION_W = 200;
    parameter RESOLUTION_H = 150;
    parameter COLOR_DEPTH = 3;
    
    // Video RAM
    reg [COLOR_DEPTH-1:0] v_ram [0:RESOLUTION_W-1][0:RESOLUTION_H-1];

    // Show a test pattern
    initial begin
        v_ram[0][0] <= 3'b111;
        v_ram[10][10] <= 3'b001;
        v_ram[20][20] <= 3'b010;
        v_ram[30][30] <= 3'b100;
        v_ram[RESOLUTION_W-1][RESOLUTION_H-1] <= 3'b111;
    end

    wire [2:0] current_pixel = v_ram[PX[9:2]][PY[9:2]];

    always @(posedge PIXEL_CLOCK) begin
        if (ON_SCREEN) begin
                RED <= current_pixel[0];
                GREEN <= current_pixel[1];
                BLUE <= current_pixel[2];
        end
    end

    always @(posedge PIXEL_CLOCK) begin
        if (WRITE) begin
            v_ram[X_POS][Y_POS] <= COLOR;
        end
    end

endmodule
