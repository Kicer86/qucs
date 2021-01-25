// VS::printSubckt main
module main(P1, P2);
Port #(.$xposition(400), .$yposition(180), .$tx(4), .$ty(4), .Num(2), .Type(analog)) P2(P2, net_400_180);
Port #(.$xposition(180), .$yposition(180), .$tx(-23), .$ty(-23), .Num(1), .Type(analog)) P1(P1, net_180_180);
GND #(.$xposition(380), .$yposition(360), .$tx(0), .$ty(0)) anonymous_gnd_hack_0(net_380_360);
C #(.$xposition(380), .$yposition(280), .$tx(17), .$ty(17), .C(1 pF), .V(), .Symbol(neutral)) C1(net_380_310, net_380_250);
Sub$nest2 #(.$xposition($xposition), .$yposition($yposition), .$tx($tx), .$ty($ty), .File(nest2.sch)) SC2(net_280_180, net_340_180);
R #(.$xposition(230), .$yposition(180), .$tx(-26), .$ty(-26), .R(0 Ohm), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(US)) R1(net_200_180, net_260_180);
wire #(.$xposition(380), .$yposition(180)) noname(net_380_180, net_400_180);
wire #(.$xposition(380), .$yposition(180)) noname(net_380_180, net_380_250);
wire #(.$xposition(380), .$yposition(310)) noname(net_380_310, net_380_360);
wire #(.$xposition(340), .$yposition(180)) noname(net_340_180, net_380_180);
wire #(.$xposition(180), .$yposition(180)) noname(net_180_180, net_200_180);
wire #(.$xposition(260), .$yposition(180)) noname(net_260_180, net_280_180);
place #(.$xposition(400), .$yposition(180)) net_400_180(net_400_180);
place #(.$xposition(180), .$yposition(180)) net_180_180(net_180_180);
place #(.$xposition(380), .$yposition(360)) net_380_360(net_380_360);
place #(.$xposition(380), .$yposition(310)) net_380_310(net_380_310);
place #(.$xposition(380), .$yposition(250)) net_380_250(net_380_250);
place #(.$xposition(280), .$yposition(180)) net_280_180(net_280_180);
place #(.$xposition(340), .$yposition(180)) net_340_180(net_340_180);
place #(.$xposition(200), .$yposition(180)) net_200_180(net_200_180);
place #(.$xposition(260), .$yposition(180)) net_260_180(net_260_180);
place #(.$xposition(380), .$yposition(180)) net_380_180(net_380_180);
endmodule // main

