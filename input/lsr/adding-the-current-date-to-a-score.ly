%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.52"

\header {
  lsrtags = "titles"

  texidoces = "
Con algo de código de Scheme, se puede añadir fácilmente la fecha
actual a una partitura.

"
  doctitlees = "Añadir la fecha actual a una partitura"

  texidoc = "
With a little Scheme code, the current date can easily be added to a
score.

"
  doctitle = "Adding the current date to a score"
} % begin verbatim
% first, define a variable to hold the formatted date:
date = #(strftime "%d-%m-%Y" (localtime (current-time)))

% use it in the title block:
\header {
  title = "Including the date!"
  subtitle = \date
}

\score {
  \relative c'' {
    c4 c c c
  }
}
% and use it in a \markup block:
\markup {
  \date
}
