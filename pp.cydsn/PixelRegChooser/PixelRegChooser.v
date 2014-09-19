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
module PixelRegChooser (
    output wire out[7:0];
    output wire outClock1;
    output wire outClock2;
    output wire outInterrupt;
    reg outSelect = 1'b0;
    input wire hsync;
    input wire pclk;
    input wire d[7:0];
)

//`#start body` -- edit after this line, do not edit this line

assign d <= out;


always @posedge hsync begin
  assign outSelect <= 1'b1;
end
  
  
always @posedge pclk begin
    if (hsync) begin
        assign outSelect <= ~outSelect;
        case(outSelect)
            1'b0: begin
                assign outClock1 <= 1'b1;
            end
            1'b1: begin
                assign outClock2 <= 1'b1;
                assign outInterrupt <= 1'b1;
            end
        endcase
    end
end

always @negedge pclk begin
    assign outClock1 <= 1'b0;
    assign outClock2 <= 1'b0;
    assign outInterrupt <= 1'b0;
end


    
//on rising edge of href
//set output to out1

//on rising edge of pclk
//set output to other output
//if select out2, gen interrupt





//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line


//[] END OF FILE
