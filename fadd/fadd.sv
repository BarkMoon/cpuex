`default_nettype none

module fadd #(CLK_PER_HALF_BIT = 5208) (
               output logic [31:0]  answer,
               output logic         answer_ready,
               input logic [31:0]   operand1,
               input logic [31:0]   operand2,
               input logic          input_ready,
               input logic          received,
               input wire           rstn);      // ネガティブでOK?

   //(* ASYNC_REG = "true" *) reg [2:0] sync_reg;
   logic [3:0]    status;
   logic [31:0]   lf;
   logic [31:0]   sf;
   logic          l28;
   logic          s28;
   logic          rst_ctr;
   
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
         answer <= 32'b0;
         status <= s_idle;
         answer_ready <= 0;
      end else begin
         //answer_ready <= 0;     // readyを上げた1clock以外は常に0にするなら
         /*sync_reg[0] <= rxd;
         sync_reg[2:1] <= sync_reg[1:0];*/

         if (status == s_idle) begin
            if (input_ready == 1) begin
               if(operand1 <= )
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
