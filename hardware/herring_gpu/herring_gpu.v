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
                                VGA_RED, 
                                VGA_GREEN, 
                                VGA_BLUE,
                                VGA_HSYNC,
                                VGA_VSYNC,
                                pixel_x,
                                pixel_y,
                                on_screen);

    assign VGA_RED = on_screen & (pixel_x > 100 && pixel_x < 200 && pixel_y > 100 && pixel_y < 200);
    assign VGA_GREEN = on_screen & (pixel_x > 150 && pixel_x < 250 && pixel_y > 150 && pixel_y < 250);
    assign VGA_BLUE = on_screen & (pixel_x > 200 && pixel_x < 300 && pixel_y > 200 && pixel_y < 300);
    
endmodule