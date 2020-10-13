`default_nettype none

module uart_rx #(CLK_PER_HALF_BIT = 5208) (
               output logic [7:0] rdata,
               output logic       rdata_ready,
               output logic       ferr,
               input wire         rxd,
               input wire         clk,
               input wire         rstn);

   localparam e_clk_half_bit = CLK_PER_HALF_BIT - 1;
   localparam e_clk_bit = CLK_PER_HALF_BIT * 2 - 1;
   
   (* ASYNC_REG = "true" *) reg [2:0] sync_reg;
   logic [3:0]                  status;
   logic [31:0]                 counter;
   logic                        next;
   logic                        half;
   logic                        rst_ctr;
   
   localparam s_idle = 0;
   localparam s_start_wait = 1;
   localparam s_start_bit = 2;
   localparam s_bit_0 = 3;
   localparam s_bit_1 = 4;
   localparam s_bit_2 = 5;
   localparam s_bit_3 = 6;
   localparam s_bit_4 = 7;
   localparam s_bit_5 = 8;
   localparam s_bit_6 = 9;
   localparam s_bit_7 = 10;
   localparam s_stop_bit = 11;
   
   always @(posedge clk) begin
      if (~rstn) begin
         counter <= 0;
         half <= 0;
         next <= 0;
      end else begin
         if (counter == e_clk_bit || rst_ctr) begin
            counter <= 0;
         end else begin
            counter <= counter + 1;
         end
         if (~rst_ctr && counter == e_clk_half_bit) begin
            half <= 1;
         end else begin
            half <= 0;
         end
         if (~rst_ctr && counter == e_clk_bit) begin
            next <= 1;
         end else begin
            next <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (~rstn) begin
         rdata <= 8'b0;
         status <= s_idle;
         ferr <= 0;
         rst_ctr <= 0;
         rdata_ready <= 0;
      end else begin
         rst_ctr <= 0;
         rdata_ready <= 0;
         sync_reg[0] <= rxd;
         sync_reg[2:1] <= sync_reg[1:0];

         if (status == s_idle) begin
            if (sync_reg[2] == 0) begin
               ferr <= 0;
               rst_ctr <= 1;
               status <= s_start_wait;
            end
         end else if (status == s_start_wait) begin
            if (half) begin
               rst_ctr <= 1;
               status <= s_start_bit;
            end
         end else if (status == s_stop_bit) begin
            if (half) begin
               rdata_ready <= 1;
               status <= s_idle;
            end
         end else if (next) begin
            if (status == s_bit_7) begin
               status <= s_stop_bit;
               if (sync_reg[2] == 0)  begin
                  ferr <= 1;
               end
            end else begin
               rdata[7] <= sync_reg[2];
               rdata[6:0] <= rdata[7:1];
               status <= status + 1;
            end
         end
      end
   end
   
endmodule
`default_nettype wire
