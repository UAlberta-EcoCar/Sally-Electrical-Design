[Transient Analysis]
{
   Npanes: 6
   Active Pane: 5
   {
      traces: 2 {524290,0,"V(angle_in)"} {524291,0,"V(angle)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: ('m',0,-0.09,0.09,0.9)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: ('m',0,0,0,-0.09,0.09,0.9)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 1 {524292,0,"V(speed)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: (' ',0,-100,20,100)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: (' ',0,0,0,-100,20,100)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 1 {524293,0,"V(accel)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: ('K',0,-30000,6000,30000)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: ('K',0,0,0,-30000,6000,30000)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 2 {524294,0,"V(bemf_a)"} {524295,0,"V(bemf_b)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: ('m',0,-0.48,0.08,0.4)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: ('m',0,0,0,-0.48,0.08,0.4)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 2 {34603016,0,"I(L2)"} {34603017,0,"I(L1)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: ('m',0,-0.025,0.005,0.025)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Amps: ('m',0,0,0,-0.025,0.005,0.025)
      Log: 0 0 0
      GridStyle: 1
   },
   {
      traces: 2 {524298,0,"V(va)"} {524299,0,"V(vb)"}
      X: ('m',0,0,0.06,0.6)
      Y[0]: (' ',0,-6,1,5)
      Y[1]: ('_',0,1e+308,0,-1e+308)
      Volts: (' ',0,0,0,-6,1,5)
      Log: 0 0 0
      GridStyle: 1
   }
}
