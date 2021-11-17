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

    // Font ROM (4 x 8 x 8)
    reg [7:0] font [0:3][0:7];

    // Character buffer
    reg [1:0] framebuffer[0:74][0:99];

    initial begin
        // Initialize the font "ROM" and character buffer
        $readmemb("font.txt", font);
        $readmemh("framebuffer.txt", framebuffer);
    end

    // which framebuffer cell is the pixel in? is there a sprite there? if yes, find the pixel in the sprite

    wire [7:0] fb_x = pixel_x[10:3];
    wire [7:0] fb_y = pixel_y[10:3];

    wire [2:0] sprite_index = framebuffer[fb_y][fb_x];

    // indices into the sprite table
    wire [2:0] sp_x = pixel_x - (fb_x << 3);
    wire [2:0] sp_y = pixel_y - (fb_y << 3);

    // Is the exact pixel in the sprite on or off?
    wire sprite_pixel = font[sprite_index][sp_x][sp_y];

    assign VGA_RED = on_screen && (sprite_index == 2'b10) && sprite_pixel;
    assign VGA_GREEN = on_screen && (sprite_index == 2'b01) && sprite_pixel;
    assign VGA_BLUE = on_screen && (sprite_index == 2'b11) && sprite_pixel;

    // TODO: currently, this is not using any BRAM. I think it's because all of this is combinatorial. BRAM probably needs a clock

endmodule
