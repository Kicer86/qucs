module main(P1, P1);
Port #(.Num(2), .Type(analog)) P2(P1, P1);
Port #(.Num(1), .Type(analog)) P1(P1, P1);
R #(.R(0 Ohm), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(US)) R1(P1, P1);
GND #() *(gnd);
C #(.C(1 pF), .V(), .Symbol(neutral)) C1(gnd, P1);
endmodule // main

Sub #(.File()) Sub(unknown_net);
//else?
module :SymbolSection:();
painting incomplete
// .port_ x=-30 y=0 n=1 label=:port2
painting incomplete
painting incomplete
painting incomplete
painting incomplete
painting incomplete
painting incomplete
painting incomplete
// .port_ x=30 y=0 n=0 label=:port1
painting incomplete
painting incomplete
endmodule // :SymbolSection:

//else?
module :Paintings:();
endmodule // :Paintings:

