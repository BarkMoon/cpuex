module FPU(clk, rstn, ctl, x1, x2, y, ready, en);
    input clk, rstn;
    input [3:0] ctl;
    input [31:0] x1, x2;
    input en;
    output reg ready;
    output reg [31:0] y;

    reg [3:0] count;

    wire fadd = (ctl == 2);
    // 以下、ctlの数値は適当です。変えてください。
    wire fsub = (ctl == 3);
    wire fmul = (ctl == 4);
    wire finv = (ctl == 5);
    wire fdiv = (ctl == 6);
    wire fhalf = (ctl == 7);
    //wire ftoi = (ctl == 8);
    //wire itof = (ctl == 9);
    //wire floor = (ctl == 10);
    wire feq = (ctl == 11);
    wire fle = (ctl == 12);
    wire fabs = (ctl == 13);
    wire fneg = (ctl == 14);

    wire [31:0] fadd_y, fsub_y, fmul_y, finv_y, fdiv_y, fhalf_y, fabs_y, fneg_y;
    wire feq_y, fle_y;
    wire ovf;
    fadd fadd_instance(x1, x2, fadd_y, ovf, clk, rstn);
    fsub fsub_instance(x1, x2, fsub_y, ovf, clk, rstn);
    fmul fmul_instance(x1, x2, fmul_y, ovf, clk, rstn);
    finv finv_instance(x1, finv_y, clk, rstn);
    fdiv fdiv_instance(x1, x2, fdiv_y, clk, rstn);
    fhalf fhalf_instance(x1, fhalf_y, clk, rstn);
    feq feq_instance(x1, x2, feq_y, clk, rstn);
    fle fle_instance(x1, x2, fle_y, clk, rstn);
    fabs fabs_instance(x1, fabs_y, clk, rstn);
    fneg fneg_instance(x1, fneg_y, clk, rstn);

    localparam FADD_NSTAGE = 2;
    localparam FSUB_NSTAGE = 2;
    localparam FMUL_NSTAGE = 3;
    localparam FINV_NSTAGE = 4;
    localparam FDIV_NSTAGE = 9;
    localparam FHALF_NSTAGE = 1;
    localparam FEQ_NSTAGE = 1;
    localparam FLE_NSTAGE = 1;
    localparam FABS_NSTAGE = 1;
    localparam FNEG_NSTAGE = 1;

    always @(posedge clk) begin
        if(~rstn) begin
            ready <= 0;
            y <= 0;
            count <= 0;
        end else if (en) begin
            if(fadd) begin
                count <= FADD_NSTAGE;
                ready <= 0;
            end else if(fsub) begin
                count <= FSUB_NSTAGE;
                ready <= 0;
            end else if(fmul) begin
                count <= FMUL_NSTAGE;
                ready <= 0;
            end else if(finv) begin
                count <= FINV_NSTAGE;
                ready <= 0;
            end else if(fdiv) begin
                count <= FDIV_NSTAGE;
                ready <= 0;
            end else if(fhalf) begin
                count <= FHALF_NSTAGE;
                ready <= 0;
            end else if(feq) begin
                count <= FEQ_NSTAGE;
                ready <= 0;
            end else if(fle) begin
                count <= FLE_NSTAGE;
                ready <= 0;
            end else if(fabs) begin
                count <= FABS_NSTAGE;
                ready <= 0;
            end else if(fneg) begin
                count <= FNEG_NSTAGE;
                ready <= 0;
            end
        end else if (count == 1) begin
            if(fadd) begin
                y <= fadd_y;
            end else if (fsub) begin
                y <= fsub_y;
            end else if (fmul) begin
                y <= fmul_y;
            end else if (finv) begin
                y <= finv_y;
            end else if (fdiv) begin
                y <= fdiv_y;
            end else if (fhalf) begin
                y <= fhalf_y;
            end else if (feq) begin
                y <= {31'b0, feq_y};
            end else if (fle) begin
                y <= {31'b0, fle_y};
            end else if (fabs) begin
                y <= fabs_y;
            end else if (fneg) begin
                y <= fneg_y;
            end
            ready <= 1;
            count <= 0;
        end else if (count == 0) begin
            ready <= 0;
        end else begin
            count <= count - 1;
        end
    end
endmodule