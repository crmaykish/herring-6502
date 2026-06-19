module herring_f_top (
    input  wire       clk_27,
    output wire [5:0] led,

    input  wire       uart_rx,
    output wire       uart_tx
);

wire clk_6502;
wire [15:0] address_bus;
reg [7:0] data_bus_in;
wire [7:0] data_bus_out;
wire we;

// Clock Divider
reg [15:0] clk_cnt = 16'b0;
always @(posedge clk_27) begin
    clk_cnt <= clk_cnt + 1'b1;
end
assign clk_6502 = clk_cnt[15];  // Super slow CPU clock

// Reset counter
reg rst = 1'b1;
reg [7:0] rst_cnt = 8'b0;
always @(posedge clk_6502) begin
    if (rst_cnt == 8'd100) rst <= 1'b0;
    else rst_cnt <= rst_cnt + 1'b1;
end

// ROM (8KB)
reg [7:0] rom[0:8191];
reg [7:0] rom_out;
initial $readmemh("rom.hex", rom);
always @(posedge clk_6502) rom_out <= rom[address_bus[12:0]];

// GPIO
reg [7:0] gpio = 8'h00;
reg [7:0] gpio_out;
always @(posedge clk_6502) gpio_out <= gpio;

// CPU Core
cpu c1(
    .clk(clk_6502),
    .reset(rst),
    .AB(address_bus),
    .DI(data_bus_in),
    .DO(data_bus_out),
    .WE(we),
    .IRQ(1'b0),
    .NMI(1'b0),
    .RDY(1'b1)
);

// Memory Map
wire cs_ram  = ~address_bus[15];                  // 0x0000-0x7FFF  RAM       32 KB
wire cs_bank =  address_bus[15:14] == 2'b10;      // 0x8000-0xBFFF  RAM bank  16 KB
wire cs_io   =  address_bus[15:13] == 3'b110;     // 0xC000-0xDFFF  I/O        8 KB
wire cs_rom  =  address_bus[15:13] == 3'b111;     // 0xE000-0xFFFF  ROM        8 KB

// I/O Space
wire [2:0] io_slot = address_bus[9:7];
wire cs_sys  = cs_io && io_slot == 3'd0;    // 0xC000: System Register (bank control)
wire cs_gpio = cs_io && io_slot == 3'd1;    // 0xC080: GPIO (LEDs)
wire cs_acia = cs_io && io_slot == 3'd2;    // 0xC100: UART

// Registered Chip-select Lines (delayed one cycle to match the registered reads)
reg cs_ram_d, cs_bank_d, cs_rom_d;
reg cs_sys_d, cs_gpio_d, cs_acia_d;
always @(posedge clk_6502) begin
    cs_ram_d  <= cs_ram;
    cs_bank_d <= cs_bank;
    cs_rom_d  <= cs_rom;
    cs_sys_d  <= cs_sys;
    cs_gpio_d <= cs_gpio;
    cs_acia_d <= cs_acia;
end

// Data Bus Read Mux
always @(*) begin
    if (cs_ram_d) data_bus_in = 8'h00;          // TODO: RAM
    else if (cs_bank_d) data_bus_in = 8'h00;    // TODO: RAM bank
    else if (cs_sys_d) data_bus_in = 8'h00;     // TODO: system register
    else if (cs_gpio_d) data_bus_in = gpio_out;
    else if (cs_acia_d) data_bus_in = 8'h00;    // TODO: UART
    else if (cs_rom_d) data_bus_in = rom_out;
    else data_bus_in = 8'b00;
end

// Data Bus Writes
always @(posedge clk_6502) begin
    if (we) begin
        if (cs_gpio) gpio <= data_bus_out;
        // if (cs_sys)  sysreg <= data_bus_out;   // TODO: bank ctrl / rom paging
        // if (cs_acia) ...                        // TODO: UART
    end
end

assign led[5:0] = ~gpio[5:0];

endmodule
