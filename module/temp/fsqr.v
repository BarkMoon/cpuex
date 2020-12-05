`default_nettype none
module fsqr (
    input wire [31:0] x1,
    output wire y,
    input wire clk,
    input wire rstn);

wire ovf;
fmul u1(x1, x1, y, ovf, clk, rstn);

endmodule
`default_nettype wire