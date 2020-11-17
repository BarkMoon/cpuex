`default_nettype none
module fless (
    input wire [31:0] x1,
    input wire [31:0] x2,
    output wire y,
    input wire clk,
    input wire rstn);

wire s1 = x1[31];
wire [30:0] abs1 = x1[30:0];
wire s2 = x2[31];
wire [30:0] abs2 = x2[30:0];

wire absless;

assign absless = (abs1 < abs2);
assign y = ((s1 & ~s2) || ((~s1 & ~s2) & absless) || ((s1 & s2) & ~absless));

endmodule
`default_nettype wire: