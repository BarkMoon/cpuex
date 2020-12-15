`timescale 1ns / 100ps
`default_nettype none

module test_fdiv
    #(parameter NSTAGE = 6,
      parameter REPEATNUM = 1000000,
      parameter RANDSEED = 2) ();

wire [31:0] x1,x2,y;
wire        ovf;
//logic [31:0] x1i,x2i;
shortreal    fx1,fx2,fy;
logic [31:0] fybit;
bit 	      fovf;
bit 	      checkovf;

logic clk, rstn;
int i, diff;

logic [31:0]	x1_reg[NSTAGE:0];
logic [31:0]	x2_reg[NSTAGE:0];
logic 	val[NSTAGE:0];

assign x1 = x1_reg[0];
assign x2 = x2_reg[0];

fdiv u1(x1,x2,y,clk,rstn);

initial begin
	// $dumpfile("test_fdiv.vcd");
	// $dumpvars(0);

    $display("start of checking module fdiv");
    $display("difference message format");
    $display("x1 = [input 1(bit)], [exponent 1(decimal)]");
    $display("x2 = [input 2(bit)], [exponent 2(decimal)]");
    $display("ref. : result(float) sign(bit),exponent(decimal),mantissa(bit)");
    $display("fdiv : result(float) sign(bit),exponent(decimal),mantissa(bit)");
    
    #1;			//t = 1ns
    rstn = 0;
    clk = 1;
    val = {default: 1'b0};
    x1_reg[0] = 0;
    x2_reg[0] = 0;
    i=0;

    #1;			//t = 2ns
    clk = 0;
    #1;			//t = 3ns
    clk = 1;
    rstn = 1;

    repeat(RANDSEED) begin
        i = $urandom();
    end

    repeat(REPEATNUM) begin
        x1_reg[0] <= $urandom();
        x2_reg[0] <= $urandom();
        val[0] <= 1;

        #1;
		clk = 0;
		#1;
		clk = 1;
    end
    repeat(NSTAGE+1) begin
        #1;
	    clk = 0;
	    #1;
	    clk = 1;
    end
    $display("end of checking module fdiv");
    $finish;
end

always @(posedge clk) begin
	x1_reg[NSTAGE:1] <= x1_reg[NSTAGE-1:0];
	x2_reg[NSTAGE:1] <= x2_reg[NSTAGE-1:0];
	val[NSTAGE:1] <= val[NSTAGE-1:0];
end
   
always @(posedge clk) begin
	if (val[NSTAGE]) begin      //ここ、ステージ分けがちゃんとしていれば別に必要ないです。
		/*fx1 = $bitstoshortreal(x1_reg[NSTAGE]);
		fx2 = $bitstoshortreal(x2_reg[NSTAGE]);
        fy = fx1 / fx2;
        fybit = $shortrealtobits(fy);
        
        diff = (fybit >= y) ? fybit - y : y - fybit;
        $display("diff = %d", diff);
   	    $display("\nx1 = %b %b %b, %3d",
	    x1_reg[NSTAGE][31], x1_reg[NSTAGE][30:23], x1_reg[NSTAGE][22:0], x1_reg[NSTAGE][30:23]);
   	    $display("x2 = %b %b %b, %3d",
	    x2_reg[NSTAGE][31], x2_reg[NSTAGE][30:23], x2_reg[NSTAGE][22:0], x2_reg[NSTAGE][30:23]);
   	    $display("%e %b,%3d,%b", fy,
	    fybit[31], fybit[30:23], fybit[22:0]);
   	    $display("%e %b,%3d,%b\n", $bitstoshortreal(y),
	    y[31], y[30:23], y[22:0]);*/
        $display("%d %d", x1_reg[NSTAGE][31:0], x2_reg[NSTAGE][31:0]);
   	    $display("%d", y[31:0]);
    end
end
endmodule

`default_nettype wire
