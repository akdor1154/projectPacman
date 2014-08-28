
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
    output reg [7:0] out_y,
    output reg [7:0] out_cb,
    output reg [7:0] out_cr,
    output reg inframe,
    output wire ready_int,
	input wire [7:0] d_in,
	input wire href,
	input wire pclk
);


//`#start body` -- edit after this line, do not edit this line

reg [1:0] outSelect;
reg ready_int_enable;
reg lastHref;

assign ready_int = ready_int_enable & ~pclk & href;



always @(posedge pclk) begin
    lastHref <= href;
    if (lastHref != href) begin
        outSelect <= 2'b00;
        inframe <= 1'b0;
        ready_int_enable <= 1'b0;
    end else if (href) begin
        case(outSelect)
            2'b00: begin
                out_cb <= d_in;
                ready_int_enable <= 1'b0;
                outSelect <= 2'b01;
            end
            2'b01: begin
                out_y <= d_in;
                if (inframe)
                    ready_int_enable <= 1'b1;
                else
                    ready_int_enable <= 1'b0;
                outSelect <= 2'b10;
            end
            2'b10: begin
                out_cr <= d_in;
                ready_int_enable <= 1'b0;
                outSelect <= 2'b11;
            end
            2'b11: begin
                out_y <= d_in;
                inframe <= 1'b1;
                ready_int_enable <= 1'b1;
                outSelect <= 2'b00;
            end
        endcase
    end else begin
        outSelect <= outSelect;
        inframe <= 1'b0;
        ready_int_enable <= 1'b0;
    end
end


//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
