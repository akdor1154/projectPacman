
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 08/20/2014 at 10:13
// Component: PixelRegControl
module PixelRegControl (
	output wire [7:0] d_out,
    output wire out_clk_reg1,
    output wire out_clk_reg2,
    output wire ready_int,
	input wire [7:0] d_in,
	input wire href,
	input wire pclk
);


//`#start body` -- edit after this line, do not edit this line

reg outSelect;
reg out_reg1_enable;
reg out_reg2_enable;
reg ready_int_enable;

assign d_out = d_in;

assign out_clk_reg1 = out_reg1_enable & pclk;
assign out_clk_reg2 = out_reg2_enable & pclk;
assign ready_int = ready_int & pclk;

always @(posedge pclk) begin
    if (href) begin
        outSelect <= ~outSelect;
        case(outSelect)
            1'b0: begin
                out_reg1_enable <= 1'b1;
                out_reg2_enable <= 1'b0;
                ready_int_enable <= 1'b0;
            end
            1'b1: begin
                out_reg1_enable <= 1'b0;
                out_reg2_enable <= 1'b1;
                ready_int_enable <= 1'b1;
            end
            default: begin
                out_reg1_enable <= 1'b0;
                out_reg2_enable <= 1'b0;
                ready_int_enable <= 1'b0;
            end
        endcase
    end else begin
        outSelect <= outSelect;
        out_reg1_enable <= 1'b0;
        out_reg2_enable <= 1'b0;
        ready_int_enable <= 1'b0;
    end
end


//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
