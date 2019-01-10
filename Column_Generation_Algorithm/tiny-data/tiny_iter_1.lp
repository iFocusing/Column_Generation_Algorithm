\ENCODING=ISO-8859-1
\Problem name: tiny

Minimize
 obj: 100 x1 + 4 x2 + 100 x3 + 3 x4
Subject To
 c1:                                 <= 4
 c2:             x2             + x4 <= 5
 c3:                            + x4 <= 4
 c4:                                 <= 3
 c5:                                 <= 2
 c6:             x2                  <= 2
 c7:     x1    + x2                   = 3
 c8:                      x3    + x4  = 3 
 Bounds
 x1 >= 0
 x2 >= 0
 x3 >= 0
 x4 >= 0
End
