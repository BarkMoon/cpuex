`default_nettype none

module fadd (
  output logic [31:0]  answer,
  output logic         answer_ready,
  input logic [31:0]   operand,
  input logic          input_ready,
  input wire           rstn);      // ネガティブでOK?

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
      if (status == s_idle) begin
        if (input_ready == 1) begin
          if(operand <= ) begin
            status <= s_start_wait;
          end
        end else if (status == s_start_wait) begin
          if (half) begin
            rst_ctr <= 1;
            status <= s_start_bit;
          end
        end
      end
  end
   
endmodule
`default_nettype wire