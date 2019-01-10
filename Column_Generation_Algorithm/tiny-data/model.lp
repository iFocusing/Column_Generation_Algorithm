\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 100000 p_1 + 100000 p_2 + 4 p_3 + 3 p_4 + 6 p_5 + 9 p_6
Subject To
 c1: p_1 + p_3 + p_5 + p_6  = 3
 c2: p_2 + p_4  = 3
 c3: s_1 + p_6  = 4
 c4: s_2 + p_3 + p_4 + p_5  = 5
 c5: s_3 + p_4  = 4
 c6: s_4 + p_5 + p_6  = 3
 c7: s_5 + p_5  = 2
 c8: s_6 + p_3  = 2
End
