\version "2.17.6"

\header {
  texidoc = "minimum-distance within a system is correctly accounted for in page breaking."
}

\layout {
  \context {
    \Score
    \override VerticalAxisGroup.staff-staff-spacing =
      #'((minimum-distance . 20))
  }
}

music = { c'1 \break c'1 }

\book {
  \paper {
    paper-height = 9\cm
  }

  \score { << \new Staff \music \new Staff \music >> }
}

