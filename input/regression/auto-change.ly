
\version "2.19.21"

\header {

texidoc=" Auto change piano staff switches voices between up
and down staves automatically; rests are switched along with the coming
note. When central C is reached, staff is not yet switched (by default).

"
}
\layout { ragged-right= ##t }

\context PianoStaff <<
  \context Staff = "up" {
    \autochange  \new Voice << \relative { g4 c e d c8 r r4 a g } >>
  }
  \context Staff = "down" {
    \clef bass 
    s1*2
  }

>>

