% DO NOT EDIT this file manually; it is automatically
% generated from Documentation/snippets/new
% Make any changes in Documentation/snippets/new/
% and then run scripts/auxiliar/makelsr.py
%
% This file is in the public domain.
%% Note: this file works from version 2.19.56
\version "2.19.56"

\header {
  lsrtags = "keyboards, really-simple, template"

  texidoc = "
Here is a simple piano staff with some notes.

"
  doctitle = "Piano template (simple)"
} % begin verbatim

upper = \relative c'' {
  \clef treble
  \key c \major
  \time 4/4

  a4 b c d
} % begin verbatim


lower = \relative c {
  \clef bass
  \key c \major
  \time 4/4

  a2 c
}

\score {
  \new PianoStaff \with { instrumentName = "Piano" }
  <<
    \new Staff = "upper" \upper
    \new Staff = "lower" \lower
  >>
  \layout { }
  \midi { }
}
