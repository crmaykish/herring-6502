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

    reg [4:0] sprite [0:4];
    reg [10:0] sprite_x = 20;
    reg [10:0] sprite_y = 40;

    initial begin
        sprite[0] = 5'b10000;
        sprite[1] = 5'b10000;
        sprite[2] = 5'b11111;
        sprite[3] = 5'b10001;
        sprite[4] = 5'b10001;
    end

    always @(posedge CLK_PIXEL) begin
        clk_counter <= clk_counter + 1;
    end

    always @(posedge CLK_SLOW) begin
        sprite_x <= sprite_x + 2;
        sprite_y <= sprite_y + 3;

        if (sprite_x >= 800) sprite_x <= 0;
        if (sprite_y >= 600) sprite_y <= 0;
    end

    // enabled if the current VGA pixel is within the bounds of the sprite
    wire in_sprite = pixel_x >= sprite_x && pixel_x < sprite_x + 5 && pixel_y >= sprite_y && pixel_y < sprite_y + 5;

    // indicies into the sprite table
    wire [2:0] sp_x = pixel_x - sprite_x;
    wire [2:0] sp_y = pixel_y - sprite_y;

    // TODO: sprite is flipped horizontally

    // Is the exact pixel in the sprite on or off?
    wire sprite_pixel = sprite[sp_y][sp_x];

    assign VGA_RED = on_screen && in_sprite && sprite_pixel;
    // assign VGA_GREEN = on_screen && pixel_y[4];
    // assign VGA_BLUE = on_screen && pixel_x[4];

endmodule
