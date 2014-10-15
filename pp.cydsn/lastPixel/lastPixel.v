
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
// Generated on 10/15/2014 at 09:55
// Component: lastPixel
module lastPixel (
	input   href,
	input   pixelReady,
	input   vsync,
	input  [7:0] inputBlue,
	input  [7:0] inputGreen,
	input  [7:0] inputRed,
	input  [7:0] targetBlue,
	input  [7:0] targetGreen,
	input  [7:0] targetRed,
    
	output reg [7:0] lastX,
	output reg [7:0] lastY
);

//`#start body` -- edit after this line, do not edit this line

reg [7:0] y;
reg [7:0] x;
reg [7:0] runningY;
reg [7:0] runningX;

// or posedge pixelReady or posedge href
always @(posedge vsync) begin
      if (vsync) begin
            y <= 0;            
            x <= 0;
            lastY <= runningY;
            lastX <= runningX;
            runningX <= 0;
            runningY <= 0;
      end else if (pixelReady) begin
            if (
                  ((targetRed>inputRed)?(targetRed - inputRed):(inputRed-targetRed))<20 
                  & ((targetGreen>inputGreen)?(targetGreen - inputGreen):(inputGreen-targetGreen))<20
                  & ((targetBlue>inputBlue)?(targetBlue - inputBlue):(inputBlue-targetBlue))<20
                  ) begin
                  runningX <= x;
                  runningY <= y;
            end
            x <= x+1;
      end else if (href) begin
            y <= y+1;
      end else begin
      end
end



//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
