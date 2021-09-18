module herring_gpu(
    input PIXEL_CLOCK,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC,

    // CPU Interface
    input [1:0] RS,         // Register select
    input [7:0] DATA,       // Data bus
    input CE,               // Chip-enable, active-low
    input CPU_CLOCK         // CPU system clock
);

    // Screen position and status
    wire [10:0] pixel_x;
    wire [9:0] pixel_y;
    wire on_screen;

    // Command state
    reg [7:0] x_pos = 8'b0;
    reg [7:0] y_pos = 8'b0;
    reg [2:0] color = 3'b0;
    reg write_fb = 1'b0;

    vga_timing VGATiming800x600(PIXEL_CLOCK,
                                VGA_HSYNC,
                                VGA_VSYNC,
                                pixel_x,
                                pixel_y,
                                on_screen);

    framebuffer FrameBuffer(PIXEL_CLOCK,
                            pixel_x,
                            pixel_y,
                            on_screen,
                            VGA_RED,
                            VGA_GREEN,
                            VGA_BLUE,
                            x_pos,
                            y_pos,
                            color,
                            write_fb);

    always @(negedge CPU_CLOCK) begin
        write_fb <= 1'b0;

        if (~CE) begin
            case (RS)
                2'b00: color <= DATA[2:0];
                2'b01: x_pos <= DATA;
                2'b10: y_pos <= DATA;
                2'b11: write_fb <= 1'b1;
            endcase
        end
    end

endmodule
