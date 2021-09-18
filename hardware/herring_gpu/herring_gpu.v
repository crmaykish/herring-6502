module herring_gpu(
    input PIXEL_CLOCK,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);

    // Screen position and status
    wire [10:0] pixel_x;
    wire [9:0] pixel_y;
    wire on_screen;

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
                            VGA_BLUE);

endmodule
