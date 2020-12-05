`timescale 1ns / 100ps
`default_nettype none

module test_FPU
    #(parameter MAX_NSTAGE = 10,
      parameter REPEATNUM = 50,
      parameter RANDSEED = 2) ();

wire [31:0] x1, x2, y;
wire        ovf;
shortreal    fx1, fx2, fy, fl, absfy;
int ix;
logic [31:0] fybit;
logic [31:0] absx, absfybit;
bit 	      fovf;
bit 	      checkovf;
int i;
logic [31:0] r, x1d;

logic clk, rstn;
logic [4:0] ctl;
logic en, ready;
int diff;
int totalclk;

logic [31:0] x1_reg[MAX_NSTAGE:0];
logic [31:0] x2_reg[MAX_NSTAGE:0];
logic [4:0] wait_clock;

//localparam FADD_NSTAGE = 2;

assign x1 = x1_reg[0];
assign x2 = x2_reg[0];

FPU u1(clk,rstn,ctl,x1,x2,y,ready,en);

initial begin
	// $dumpfile("test_FPU.vcd");
	// $dumpvars(0);

    $display("start of checking module FPU");
    $display("difference message format");
    $display("x1 = [input 1(bit)], [exponent 1(decimal)]");
    $display("x2 = [input 2(bit)], [exponent 2(decimal)]");
    $display("ref. : result(float) sign(bit),exponent(decimal),mantissa(bit)");
    $display("FPU : result(float) sign(bit),exponent(decimal),mantissa(bit)");

    #1;			//t = 1ns
    rstn = 0;
    clk = 1;
    x1_reg[0] = 0;
    x2_reg[0] = 0;
    i=0;
    wait_clock = 0;
    totalclk = 0;

    #1;			//t = 2ns
    clk = 0;
    #1;			//t = 3ns
    clk = 1;
    rstn = 1;

    repeat(RANDSEED * REPEATNUM) begin
        i = $urandom();
    end

    repeat(REPEATNUM) begin
        r = $urandom();
        case (r[4:0])
            5'b00000: ctl <= 0;
            5'b00001: ctl <= 1;
            5'b00010: ctl <= 2;
            5'b00011: ctl <= 3;
            5'b00100: ctl <= 4;
            5'b00101: ctl <= 5;
            5'b00110: ctl <= 6;
            5'b00111: ctl <= 7;
            5'b01000: ctl <= 8;
            5'b01001: ctl <= 9;
            5'b01010: ctl <= 10;
            5'b01011: ctl <= 11;
            5'b01100: ctl <= 12;
            5'b01101: ctl <= 13;
            5'b01110: ctl <= 14;
            5'b01111: ctl <= 15;
            5'b10000: ctl <= 16;
            5'b10001: ctl <= 17;
            5'b10010: ctl <= 18;
            default: ctl <= 17;
        endcase
        
        //ctl <= 6;
        //x1_reg[0] <= {x1d[31], 8'b10001111, x1d[22:0]};
        //ctl <= 7;
        //x1_reg[0] <= {20'b0, x1d[11:0]};

        //ctl <= 3;
        x1d = $urandom();
        //x1_reg[0] <= (r[4:0] == 5'b10001) ? {1'b0, x1d[30:0]} : x1d;
        x1_reg[0] <= x1d;
        x2_reg[0] <= $urandom();
        en <= 1;

        #1;
		clk = 0;
        #1;
		clk = 1;
        en <= 0;

        // ラッパーのモジュールにポンポン値を入れてしまうと、答えの衝突と何の答えか分からなくなることが起こるため、結果が出るまではストール。
        while (~ready) begin
            #1;
		    clk = 0;
		    #1;
		    clk = 1;
        end
        /*repeat (MAX_NSTAGE) begin
            #1;
		    clk = 0;
		    #1;
		    clk = 1;
        end*/
    end
    repeat(MAX_NSTAGE) begin
        #1;
	    clk = 0;
	    #1;
	    clk = 1;
    end
    $display("end of checking module FPU");
    $finish;
end

always @(posedge clk) begin
    totalclk = totalclk+1;
    if(~rstn) begin
	    x1_reg[MAX_NSTAGE:0] <= {default: 32'b0};
	    x2_reg[MAX_NSTAGE:0] <= {default: 32'b0};
        wait_clock <= 0;
    end else begin
        x2_reg[MAX_NSTAGE:1] <= x2_reg[MAX_NSTAGE-1:0];
        x1_reg[MAX_NSTAGE:1] <= x1_reg[MAX_NSTAGE-1:0];
        if(en) begin
            wait_clock <= 1;
        end else begin
            wait_clock <= wait_clock + 1;
        end
    end
end
   
always @(posedge clk) begin
	if (ready) begin
		fx1 = $bitstoshortreal(x1_reg[wait_clock]);
		fx2 = $bitstoshortreal(x2_reg[wait_clock]);
        case (ctl)
            0: fy = fx1 + fx2;
            1: fy = fx1 - fx2;
            2: fy = fx1 * fx2;
            3: fy = 1.0 / fx1;
            4: fy = fx1 / fx2;
            5: fy = fx1 / 2;
            8: fy = $floor(fx1);
            11: fy = (fx1 < 0) ? -fx1 : fx1;
            12: fy = -fx1;
            17: fy = $sqrt(fx1);
            18: fy = fx1 * fx1;
            default: fy = 0;
        endcase
        
        if(ctl == 6) begin
            ix = $rtoi(fx1);
            fl = $itor(ix);
            fybit = (ix == {1'b1, 31'b0}) ? ix :
                    (fx1 - fl >= 0.5) ? ix + 1 :
                    (fx1 - fl <= -0.5) ? ix - 1 : ix ;
        end else if (ctl == 7) begin
            absx = (x1_reg[wait_clock][31] == 1) ? (~x1_reg[wait_clock]) + 1 : x1_reg[wait_clock];
            absfy = $itor(absx);
            absfybit = $shortrealtobits(absfy);
            fybit = {x1_reg[wait_clock][31], absfybit[30:0]};
            fy = $bitstoshortreal(fybit);
        end else begin
            fybit = $shortrealtobits(fy);
        end
        
        if (ctl == 9) begin
            fybit[0] = (fx1 == fx2);
        end else if (ctl == 10) begin
            fybit[0] = (fx1 <= fx2);
        end else if (ctl == 13) begin
            fybit[0] = (fx1 == 0);
        end else if (ctl == 14) begin
            fybit[0] = (fx1 > 0);
        end else if (ctl == 15) begin
            fybit[0] = (fx1 < 0);
        end else if (ctl == 16) begin
            fybit[0] = (fx1 < fx2);
        end
        
        $display("");
        case (ctl)
            0: $display("fadd");
            1: $display("fsub");
            2: $display("fmul");
            3: $display("finv");
            4: $display("fdiv");
            5: $display("fhalf");
            6: $display("ftoi");
            7: $display("itof");
            8: $display("floor");
            9: $display("feq");
            10: $display("fle");
            11: $display("fabs");
            12: $display("fneg");
            13: $display("fiszero");
            14: $display("fispos");
            15: $display("fisneg");
            16: $display("fless");
            17: $display("sqrt");
            18: $display("fsqr");
        endcase
        //$display("wait_clock = %d", wait_clock);

        diff = (fybit >= y) ? fybit - y : y - fybit;
        $display("diff = %d", diff);
        if(ctl == 6 || ctl == 13 || ctl == 14 || ctl == 15) begin
            $display("x = %b %b %b, %3d %.15f",
	        x1_reg[wait_clock][31], x1_reg[wait_clock][30:23], x1_reg[wait_clock][22:0], x1_reg[wait_clock][30:23], fx1);
   	        $display("%d %b", $signed(fybit), fybit);
   	        $display("%d %b\n", $signed(y), y);
        end else if(ctl == 9 || ctl == 10 || ctl == 16) begin
            $display("x1 = %b %b %b, %3d %f",
	        x1_reg[wait_clock][31], x1_reg[wait_clock][30:23], x1_reg[wait_clock][22:0], x1_reg[wait_clock][30:23], fx1);
   	        $display("x2 = %b %b %b, %3d %f",
	        x2_reg[wait_clock][31], x2_reg[wait_clock][30:23], x2_reg[wait_clock][22:0], x2_reg[wait_clock][30:23], fx2);
   	        $display("%d %b", $signed(fybit), fybit);
   	        $display("%d %b\n", $signed(y), y);
        end else if (ctl == 7) begin
            $display("x = %b, %d",
	        x1_reg[wait_clock], $signed(x1_reg[wait_clock]));
   	        $display("%.15f %b,%3d,%b", fy,
	        fybit[31], fybit[30:23], fybit[22:0]);
   	        $display("%.15f %b,%3d,%b\n", $bitstoshortreal(y),
	        y[31], y[30:23], y[22:0]);
        end else if (ctl == 3 || ctl == 5 || ctl == 8 || ctl == 17 || ctl == 18) begin
   	        $display("x1 = %b %b %b, %3d %f",
	        x1_reg[wait_clock][31], x1_reg[wait_clock][30:23], x1_reg[wait_clock][22:0], x1_reg[wait_clock][30:23], fx1);
   	        $display("%e %b,%3d,%b", fy,
	        fybit[31], fybit[30:23], fybit[22:0]);
   	        $display("%e %b,%3d,%b\n", $bitstoshortreal(y),
	        y[31], y[30:23], y[22:0]);
        end else begin
   	        $display("x1 = %b %b %b, %3d %f",
	        x1_reg[wait_clock][31], x1_reg[wait_clock][30:23], x1_reg[wait_clock][22:0], x1_reg[wait_clock][30:23], fx1);
   	        $display("x2 = %b %b %b, %3d %f",
	        x2_reg[wait_clock][31], x2_reg[wait_clock][30:23], x2_reg[wait_clock][22:0], x2_reg[wait_clock][30:23], fx2);
   	        $display("%e %b,%3d,%b", fy,
	        fybit[31], fybit[30:23], fybit[22:0]);
   	        $display("%e %b,%3d,%b\n", $bitstoshortreal(y),
	        y[31], y[30:23], y[22:0]);
        end
    end
    $display("total clocks = %d", totalclk);
end
endmodule

`default_nettype wire
