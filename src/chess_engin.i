 // pair.i - SWIG interface
 %module chess_engin
 %{
 #include "ChessGame.h"
 #include "basedef.h"
 #include "move.h"
 %}
 
%include "std_vector.i"
namespace std {
  %template(VecInt) vector<int>;
  %template(VecVecInt) vector< vector<int> >;
}

%ignore operator<<(std::ostream&, const Player);
%ignore operator<<(std::ostream&, const PieceType);
%ignore operator<<(std::ostream&, const Move);
%ignore operator<<(std::ostream&, const Board);
%rename (opp) operator!(const Player);

 %include "ChessGame.h"
 %include "basedef.h"
 %include "move.h"