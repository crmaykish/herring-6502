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

    reg [24:0] clk_counter = 0;
    wire CLK_SLOW = clk_counter[22];

    reg [7:0] sprite [0:2][0:7];
    reg [10:0] sprite_x = 20;
    reg [10:0] sprite_y = 40;


    reg [1:0] framebuffer[0:99][0:74];

    initial begin
        // TODO: move font to a memory file
        sprite[1][0] = 8'b11111111;
        sprite[1][1] = 8'b10001111;
        sprite[1][2] = 8'b10001111;
        sprite[1][3] = 8'b10001111;
        sprite[1][4] = 8'b11110001;
        sprite[1][5] = 8'b11110001;
        sprite[1][6] = 8'b11110001;
        sprite[1][7] = 8'b11111111;

        sprite[2][0] = 8'b10000001;
        sprite[2][1] = 8'b01000010;
        sprite[2][2] = 8'b00100100;
        sprite[2][3] = 8'b00011000;
        sprite[2][4] = 8'b00011000;
        sprite[2][5] = 8'b00100100;
        sprite[2][6] = 8'b01000010;
        sprite[2][7] = 8'b10000001;

        // NOTE: initializing the framebuffer seems very important
        $readmemh("framebuffer.txt", framebuffer);
    end

    // always @(posedge CLK_PIXEL) begin
    //     clk_counter <= clk_counter + 1;
    // end

    // always @(posedge CLK_SLOW) begin
    //     sprite_x <= sprite_x + 1;
    //     sprite_y <= sprite_y + 1;

    //     if (sprite_x >= 100) sprite_x <= 0;
    //     if (sprite_y >= 75) sprite_y <= 0;

    //     // framebuffer[sprite_x][sprite_y] <= 1'b1;
    // end

    // which framebuffer cell is the pixel in? is there a sprite there? if yes, find the pixel in the sprite

    wire [7:0] fb_x = pixel_x[10:3];
    wire [7:0] fb_y = pixel_y[10:3];

    wire [2:0] sprite_index = framebuffer[fb_x][fb_y];

    // TODO: This does seem to be required (at least as long as the memory is not BRAM)
    wire in_sprite = (sprite_index > 0);

    // indices into the sprite table
    wire [2:0] sp_x = pixel_x - (fb_x << 3);
    wire [2:0] sp_y = pixel_y - (fb_y << 3);

    // TODO: sprite is flipped horizontally

    // Is the exact pixel in the sprite on or off?
    wire sprite_pixel = sprite[sprite_index][sp_y][sp_x];

    assign VGA_RED = on_screen && sprite_index == 2 && sprite_pixel;
    assign VGA_GREEN = on_screen && sprite_index == 1 && sprite_pixel;
    // assign VGA_BLUE = on_screen && pixel_x[4];

    // TODO: currently, this is not using any BRAM. I think it's because all of this is combinatorial. BRAM probably needs a clock

endmodule
