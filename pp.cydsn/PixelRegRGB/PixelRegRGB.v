
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
// Generated on 08/29/2014 at 17:58
// Component: PixelRegRGB
module PixelRegRGB (
	output reg [4:0] b,
	output reg [5:0] g,
	output reg [4:0] r,
	output ready_int,
	input  [7:0] d_in,
	input   href,
	input   pclk
);

//`#start body` -- edit after this line, do not edit this line


reg outSelect;
reg ready_int_enable;
reg lastHref;

assign ready_int = ready_int_enable & ~pclk & href;



always @(posedge pclk) begin
    lastHref <= href;
    if ((lastHref == 1'b0) & (href == 1'b1)) begin
        r <= d_in[7:3];
        g[5:3] <= d_in[2:0];
        ready_int_enable <= 1'b0;
        outSelect <= 1'b1;
    end else if (href) begin
        case(outSelect)
            1'b0: begin
                r <= d_in[7:3];
                g[5:3] <= d_in[2:0];
                ready_int_enable <= 1'b0;
                outSelect <= 1'b1;
            end
            1'b1: begin
                g[2:0] <= d_in[7:5];
                b <= d_in[4:0];
                ready_int_enable <= 1'b1;
                outSelect <= 1'b0;
            end
        endcase
    end else begin
        outSelect <= outSelect;
        ready_int_enable <= 1'b0;
    end
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
