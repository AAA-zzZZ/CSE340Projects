// Input: string w, parse table T
// Output: if w ∈ L(G) then a leftmost derivation of w, otherwise an error

//s = stack


// s.push($), s.push(S)
// X = s.top()
// while ( X != $ )
//     lookahead = w.peek()
//     if (X is a terminal)
//         if (X == lookahead)
//             s.pop()
//             match(lookahead)
//         else
//             error()
//     else
//         if (T[X, lookahead] == X → Y1Y2...Yk)
//             output X → Y1Y2...Yk
//             s.pop()
//             s.push(YkYk-1...Y1)
//         else if (T[X, lookahead] == empty)
//             error()
//     X = s.top()