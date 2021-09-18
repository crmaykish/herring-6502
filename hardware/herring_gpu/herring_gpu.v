module herring_gpu(
    input PIXEL_CLOCK,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);

    vga_timing VGATiming800x600(PIXEL_CLOCK,
                                VGA_RED, 
                                VGA_GREEN, 
                                VGA_BLUE,
                                VGA_HSYNC,
                                VGA_VSYNC);



endmodule