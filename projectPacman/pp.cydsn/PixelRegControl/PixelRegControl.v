
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
    output wire out_clk_y,
    output wire out_clk_cb,
    output wire out_clk_cr,
    output reg [7:0] out_y;
    output reg [7:0] out_cb;
    output reg [7:0] out_cr;
    output wire ready_int,
	input wire [7:0] d_in,
	input wire href,
	input wire pclk
);


//`#start body` -- edit after this line, do not edit this line

reg [1:0] outSelect;
reg out_y_enable;
reg out_cb_enable;
reg out_cr_enable;
reg ready_int_enable;
reg lastHref;
reg diffHref;
reg inframe;

assign d_out = d_in;

assign out_clk_y = out_y_enable & pclk & href;
assign out_clk_cb = out_cb_enable & pclk & href;
assign out_clk_cr = out_cr_enable & pclk & href;
assign ready_int = ready_int_enable & pclk & href;



always @(posedge pclk) begin
    lastHref <= href;
    if (lastHref != href) begin
        diffHref <= 1'b1;
    end else begin
        diffHref <= 1'b0;
    end
    if (diffHref == 1'b1) begin
        outSelect <= 2'b00;
        inframe <= 1'b0;
        out_y_enable <= 1'b0;
        out_cb_enable <= 1'b0;
        out_cr_enable <= 1'b0;
        ready_int_enable <= 1'b0;
    end else if (href) begin
        case(outSelect)
            2'b00: begin
                out_y_enable <= 1'b0;
                out_cb_enable <= 1'b1;
                out_cr_enable <= 1'b0;
                ready_int_enable <= 1'b0;
                outSelect <= 2'b01;
            end
            2'b01: begin
                out_y_enable <= 1'b1;
                out_cb_enable <= 1'b0;
                out_cr_enable <= 1'b0;
                if (inframe) begin
                    ready_int_enable <= 1'b1;
                end else begin
                    ready_int_enable <= 1'b0;
                end
                outSelect <= 2'b10;
            end
            2'b10: begin
                out_y_enable <= 1'b0;
                out_cb_enable <= 1'b0;
                out_cr_enable <= 1'b1;
                ready_int_enable <= 1'b0;
                outSelect <= 2'b11;
            end
            2'b11: begin
                out_y_enable <= 1'b1;
                out_cb_enable <= 1'b0;
                out_cr_enable <= 1'b0;
                ready_int_enable <= 1'b1;
                inframe <= 1'b1;
                outSelect <= 2'b00;
            end
        endcase
    end else begin
        outSelect <= outSelect;
        out_y_enable <= 1'b0;
        out_cb_enable <= 1'b0;
        out_cr_enable <= 1'b0;
        ready_int_enable <= 1'b0;
    end
end


//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
