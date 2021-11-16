module new_gpu(
    input CLK_PIXEL,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);
    wire [10:0] pixel_x;
    wire [10:0] pixel_y;
    wire on_screen;

    vga_timing VGATiming(CLK_PIXEL,
                         VGA_HSYNC,
                         VGA_VSYNC,
                         pixel_x,
                         pixel_y,
                         on_screen);

    assign VGA_RED = on_screen && (((pixel_x & 7) == 0) || ((pixel_y & 7) == 0));
    assign VGA_GREEN = on_screen && pixel_y[4];
    assign VGA_BLUE = on_screen && pixel_x[4];

endmodule
