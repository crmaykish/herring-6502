module new_gpu(
    input CLK_50MHz,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);
    wire [10:0] pixel_x;
    wire [10:0] pixel_y;
    wire on_screen;

    vga_timing VGATiming(CLK_50MHz,
                         VGA_HSYNC,
                         VGA_VSYNC,
                         pixel_x,
                         pixel_y,
                         on_screen);

    assign VGA_RED = on_screen & pixel_x < 100 & pixel_y < 100;
    assign VGA_GREEN = on_screen & pixel_x < 200 & pixel_y < 200;
    assign VGA_BLUE = on_screen & pixel_x < 400 & pixel_y < 400;

endmodule
