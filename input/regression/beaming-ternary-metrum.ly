
\version "2.9.10"

\header {

  texidoc = "Automatic beaming works also in ternary time sigs. In
  this case, the 8th is a beat, so the 16ths are split into two
  groups."

}

\layout { ragged-right = ##t}

\relative c'' {
  \time 6/8
  c8.[ c16 c16 c16] 
}

