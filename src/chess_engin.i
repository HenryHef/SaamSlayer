 // pair.i - SWIG interface
 %module chess_engin
 
%include "std_vector.i"
%template(VecInt) std::vector<int>;
 
 %{
 #include "basedef.h"
 #include "move.h"
 #include "ChessGameBoardState.h"
 #include "ChessGame.h"
 %}

%ignore operator<<(std::ostream&, const Player);
%ignore operator<<(std::ostream&, const PieceType);
%ignore operator<<(std::ostream&, const Move);
%ignore operator<<(std::ostream&, const Board);
%rename (opp) operator!(const Player);

%include "basedef.h"
%include "move.h"
%include "ChessGameBoardState.h"
%include "ChessGame.h"