`default_nettype none
module finv #(parameter NSTAGE = 4)(
    input wire [31:0] x,
    output wire [31:0] y,
    //output wire ovf,
    input wire clk,
    input wire rstn); 

// stage = 0 (x)

// stage = 1 (xr[0] -> csr, grd) 

reg[31:0] xr[3:0];

wire [9:0] a0 = xr[0][22:13];

wire [57:0] cst;
wire [34:0] grd;

finv_load_const_table u1 (a0, cst, clk, rstn);
finv_load_grad_table u2 (a0, grd, clk, rstn);

// stage = 2 (cstr[0], grdr -> a1grd)

reg [57:0] cstr[1:0];
reg [34:0] grdr;

wire [12:0] a1 = xr[1][12:0];
wire [47:0] a1grd = a1 * grdr;

// stage = 3 (a1grdr -> mtmp)  //ここ軽いから分けなくていいかも

reg [47:0] a1grdr;

wire [57:0] mtmp = cstr[1] - a1grdr;

// stage = 4 (xr[3], mtmpr -> y)

reg [57:0] mtmpr;

wire s = xr[3][31];
wire [7:0] e = xr[3][30:23];
wire [22:0] m = xr[3][22:0];

wire ys = s;
wire [7:0] ye = (m == 0) ? 254 - e : 253 - e;  //e = 254がアヤシイ
wire [22:0] ym = (m == 0) ? 23'b0 : mtmpr[56:34] + mtmpr[33];  //適当に丸めている

always @(posedge clk) begin
    if(~rstn) begin
        xr[0] <= 'b0;
        cstr[0] <= 'b0;
        grdr <= 'b0;
        a1grdr <= 'b0;
        mtmpr <= 'b0;
    end else begin
        xr[0] <= x;
        cstr[0] <= cst;
        grdr <= grd;
        a1grdr <= a1grd;
        mtmpr <= mtmp;
    end
end

always @(posedge clk) begin
    xr[3:1] <= xr[2:0];
    cstr[1] <= cstr[0];
end

assign y = {ys, ye, ym};

endmodule
`default_nettype wire